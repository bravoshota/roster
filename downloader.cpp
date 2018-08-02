#include "UI/progressdialog.h"
#include "downloader.h"
#include <QtCore>
#include <QNetworkReply>
#include <QMessageBox>
#include <QAuthenticator>

Downloader::Downloader(const QString &url, const QString &outputFileName)
    : m_reply(nullptr)
    , m_outputFile(outputFileName)
{
    m_url = QUrl::fromEncoded(url.toUtf8());
    Q_ASSERT(m_url.isValid());
    connect(&m_manager, &QNetworkAccessManager::finished,
            this,       &Downloader::slotFinished);
    connect(&m_manager, &QNetworkAccessManager::sslErrors,
            this,       &Downloader::slotSslErrors);
}

void Downloader::execute()
{
    if (m_outputFile.exists())
        m_outputFile.remove();

    if (!m_outputFile.open(QIODevice::WriteOnly))
    {
        QMessageBox(QMessageBox::Warning, "File open error", m_outputFile.fileName()).exec();
        return;
    }

    QNetworkRequest networkRequest(m_url);
    m_reply = m_manager.get(networkRequest);

    ProgressDialog *progressDialog = new ProgressDialog(m_url);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(progressDialog, &QProgressDialog::canceled,
            this,           &Downloader::cancelDownload);
    connect(m_reply,        &QNetworkReply::downloadProgress,
            progressDialog, &ProgressDialog::networkReplyProgress);
    connect(m_reply,        &QIODevice::readyRead,
            this,           &Downloader::slotReadyRead);
    connect(&m_manager,     &QNetworkAccessManager::finished,
            progressDialog, &ProgressDialog::close);
    progressDialog->show();
}

void Downloader::slotSslErrors(QNetworkReply */*reply*/, const QList<QSslError> &errors)
{
    QString str;
    for (const QSslError &error : errors)
        str += (error.errorString() + "; ");

    QMessageBox(QMessageBox::Warning, "SSL Error", str).exec();
}

void Downloader::slotReadyRead()
{
    if (m_outputFile.isOpen())
        m_outputFile.write(m_reply->readAll());
}

void Downloader::slotFinished(QNetworkReply */*reply*/)
{
    m_outputFile.close();
    if (m_outputFile.size() == 0)
        m_outputFile.remove();

    bool success = true;

    if (m_reply->error())
    {
        QMessageBox(QMessageBox::Warning, "Http error: ", m_reply->errorString()).exec();
        success = false;
    }

    if (success)
    {
        int replyCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        switch (replyCode)
        {
        case 301:
        case 302:
        case 303:
        case 304:
        case 305:
        case 307:
        case 308:
            QMessageBox(QMessageBox::Warning, "Http error: ",
                        QString("HttpStatusCodeAttribute = ") + replyCode).exec();
            success = false;
            break;
        }
    }

    emit downloadFinished(success);
    m_reply->deleteLater();
    m_reply = nullptr;
}

void Downloader::cancelDownload()
{
    if (m_reply)
        m_reply->abort();
}
