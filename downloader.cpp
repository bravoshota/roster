#include "downloader.h"
#include <QFile>
#include <QtCore>
#include <QNetworkReply>
#include <QMessageBox>

Downloader::Downloader(const QString &url, const QString &outputFile)
    : m_reply(nullptr)
    , m_outputFile(outputFile)
{
    m_url = QUrl::fromEncoded(url.toUtf8());
    connect(&m_manager, SIGNAL(finished(QNetworkReply *)),
             SLOT(downloadFinished(QNetworkReply *)));
}

void Downloader::execute()
{
    QNetworkRequest networkRequest(m_url);
    m_reply = m_manager.get(networkRequest);

    connect(m_reply, SIGNAL(sslErrors(const QList<QSslError> &)),
            SLOT(sslErrors(const QList<QSslError> &)));
}

void Downloader::sslErrors(const QList<QSslError> &errors)
{
    QString str;
    for (const QSslError &error : errors)
    {
        str += error.errorString() + "/r/n";
    }

    QMessageBox(QMessageBox::Warning, "SSL Error", str);
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        QMessageBox(QMessageBox::Warning, "Network error", reply->errorString());
        return;
    }

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
        QMessageBox(QMessageBox::Warning, "Network status error", QString("code = %1").arg(replyCode));
        return;
    }

    QByteArray downloadedData = reply->readAll();
    if (downloadedData.isEmpty())
    {
        QMessageBox(QMessageBox::Warning, "Error", "Unknown error was occured.");
        return;
    }

    QFile file(m_outputFile);
    if (file.exists())
        file.remove();

    file.write(downloadedData);
    file.close();

    reply->deleteLater();
}
