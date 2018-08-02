#pragma once

#include <QProgressDialog>

class ProgressDialog : public QProgressDialog
{
    Q_OBJECT
public:
    explicit ProgressDialog(const QUrl &url);

public slots:
   void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};
