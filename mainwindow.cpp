#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "downloader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_refreshButton_clicked()
{
    Downloader downloader(Config::downloadURL(), Config::rosterFileName());
    downloader.execute();
}
