#pragma once

#include <QObject>
#include <QtNetwork/QSslError>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class QNetworkReply;

class Downloader : public QObject
{
    Q_OBJECT
public:
    Downloader(const QString &url, const QString &outputFile);

    void execute();

public slots:
    void downloadFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);
    void error(QNetworkReply::NetworkError networkError);

private:
    QNetworkAccessManager m_manager;
    QNetworkReply *m_reply;
    QUrl m_url;
    const QString &m_outputFile;

};
