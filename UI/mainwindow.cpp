#include "ui_mainwindow.h"

#include "config.h"
#include "dialog.h"
#include "mainwindow.h"
#include "rostertablemodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_rosterDialog(nullptr)
    , m_tableModel(new RosterTableModel(nullptr))
{
    ui->setupUi(this);

    setWindowTitle("Roster Main Window");

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_tableModel, SIGNAL(newDataFetched(QString)),
            this        , SLOT(newDataFetched(QString)));
    connect(m_tableModel, &RosterTableModel::updateWindow,
            this,         &MainWindow::updateWindow);

    m_tableModel->init();

    ui->fetchButton->setToolTip(QString("Fetch from \"%1\"").arg(Config::downloadURL()));
}

MainWindow::~MainWindow()
{
    if (m_rosterDialog)
        delete m_rosterDialog;

    delete ui;
    delete m_tableModel;
}

void MainWindow::resetTableView()
{
    ui->tableView->setModel(nullptr);
    ui->tableView->setModel(m_tableModel);

    if (m_tableModel->columnCount() == 4)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        ui->tableView->setColumnWidth(0, 10);
    }
}

void MainWindow::on_fetchButton_clicked()
{
    destroyDialog();
    ui->fetchButton->setEnabled(false);
    ui->filterEdit->clear();
    m_tableModel->clear();
    m_tableModel->download();
}

void MainWindow::on_filterEdit_textEdited(const QString &text)
{
    if (text.size() == 1 || text.size() == 2)
        return;

    m_tableModel->setFilter(text);
    resetTableView();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    destroyDialog();

    m_rosterDialog = new Dialog(this, m_tableModel->getRoster(index.row()));
    m_rosterDialog->show();

    connect(m_rosterDialog, SIGNAL(finished(int)), SLOT(destroyDialog(int)));
}

void MainWindow::newDataFetched(QString text)
{
    ui->label->setText(text);
}

void MainWindow::destroyDialog(int /*result*/)
{
    if (m_rosterDialog != nullptr)
    {
        delete m_rosterDialog;
        m_rosterDialog = nullptr;
    }
}

void MainWindow::updateWindow(bool resetTable)
{
    ui->fetchButton->setEnabled(true);
    if (resetTable)
        resetTableView();
}
