#include "downloader.h"
#include "progressdialog.h"
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

    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this,    SLOT(slotError(QNetworkReply::NetworkError)));
    connect(m_reply, &QIODevice::readyRead,
            this,    &Downloader::slotReadyRead);

    ProgressDialog *progressDialog = new ProgressDialog(m_url);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_reply,        &QNetworkReply::downloadProgress,
            progressDialog, &ProgressDialog::networkReplyProgress);
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

void Downloader::slotError(QNetworkReply::NetworkError networkError)
{
    QMessageBox(QMessageBox::Warning, "Network Error", QString::number(networkError)).exec();
}

void Downloader::slotReadyRead()
{
    if (m_outputFile.isOpen())
        m_outputFile.write(m_reply->readAll());
}

void Downloader::slotFinished(QNetworkReply *reply)
{
    m_outputFile.close();

    bool success = true;

    if (reply->error())
        success = false;

    if (success)
    {
        int replyCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        switch (replyCode)
        {
        case 301:
        case 302:
        case 303:
        case 304:
        case 305:
        case 307:
        case 308:
            success = false;
            break;
        }
    }

    emit downloadFinished(success);
    reply->deleteLater();
}
