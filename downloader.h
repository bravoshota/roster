#pragma once

#include <QFile>
#include <QObject>
#include <QtNetwork/QSslError>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class QNetworkReply;

class Downloader : public QObject
{
    Q_OBJECT
public:
    Downloader(const QString &url, const QString &outputFileName);

    void execute();

signals:
    void downloadFinished(bool success);

protected slots:
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slotReadyRead();
    void slotFinished(QNetworkReply *reply);
    void cancelDownload();

private:
    QNetworkAccessManager m_manager;
    QNetworkReply *m_reply;
    QFile m_outputFile;
    QUrl m_url;
};
