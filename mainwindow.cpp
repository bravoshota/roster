#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "downloader.h"

namespace
{
const QStringList columnHeaders
{
    {""}, {"First Name"}, {"Last Name"}, {"Group"}
};
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(columnHeaders.size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_reloadButton_clicked()
{
    Downloader downloader(Config::downloadURL(), Config::rosterFileName());
    downloader.execute();

    m_rosterLoader.load();

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
        for (int j = 0; j < ui->tableWidget->columnCount(); ++j)
            delete ui->tableWidget->item(i, j);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(m_rosterLoader.rosters().count());

    const auto &rosters = m_rosterLoader.rosters();
    const auto &groups = m_rosterLoader.groups();
    int currentRow = 0;
    for (const Roster &roster : rosters)
    {
        QString avatar;
        if (!roster.account.firstName.isEmpty() &&
            !roster.account.lastName.isEmpty())
        {
            avatar.push_back(roster.account.firstName[0].toUpper());
            avatar.push_back(roster.account.lastName[0].toUpper());
        }
        ui->tableWidget->setItem(currentRow, 0, new QTableWidgetItem(avatar));
        ui->tableWidget->setItem(currentRow, 1, new QTableWidgetItem(roster.account.firstName));
        ui->tableWidget->setItem(currentRow, 2, new QTableWidgetItem(roster.account.lastName));
        ui->tableWidget->setItem(currentRow, 3, new QTableWidgetItem(groups[roster.groupIndex].name));
        ++currentRow;
    }

    ui->tableWidget->setHorizontalHeaderLabels(columnHeaders);
}
