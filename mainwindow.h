#pragma once

#include "rostermodel.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_reloadButton_clicked();
    void on_filterEdit_textChanged(const QString &text);

    void newDataFetched(QString text);

private:
    Ui::MainWindow *ui;
    RosterModel m_rosterModel;
    QPalette *m_grayPalette;
    QPalette *m_activePalette;
    QFont *m_grayFont;
    QFont *m_activeFont;
};
