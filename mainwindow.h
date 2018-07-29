#pragma once

#include <QMainWindow>
#include "rosterloader.h"

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

private:
    Ui::MainWindow *ui;
    RosterLoader m_rosterLoader;
};
