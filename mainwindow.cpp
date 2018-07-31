#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rostermodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_rosterModel(new RosterModel(nullptr))
{
    ui->setupUi(this);
    connect(m_rosterModel, SIGNAL(newDataFetched(QString)), SLOT(newDataFetched(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_rosterModel;
}

void MainWindow::resetTableView()
{
    ui->tableView->setModel(nullptr);
    ui->tableView->setModel(m_rosterModel);

    if (m_rosterModel->columnCount() == 4)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        ui->tableView->setColumnWidth(0, 10);
    }
}

void MainWindow::on_updateButton_clicked()
{
    ui->filterEdit->clear();
    m_rosterModel->update();
    resetTableView();
}

void MainWindow::on_filterEdit_textEdited(const QString &text)
{
    if (text.size() == 1 || text.size() == 2)
        return;

    m_rosterModel->setFilter(text);
    resetTableView();
}

void MainWindow::newDataFetched(QString text)
{
    ui->label->setText(text);
}
