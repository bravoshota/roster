#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_rosterModel(nullptr)
{
    ui->setupUi(this);
    connect(&m_rosterModel, SIGNAL(newDataFetched(QString)), SLOT(newDataFetched(QString)));

    m_activePalette = new QPalette(ui->filterEdit->palette());
    m_grayPalette = new QPalette();
    m_grayPalette->setColor(QPalette::Text, Qt::gray);
    ui->filterEdit->setPalette(*m_grayPalette);

    const auto &origFont = ui->filterEdit->font();
    m_activeFont = new QFont(origFont.family(), 10);
    m_grayFont = new QFont(origFont.family(), 10, -1, true);
    ui->filterEdit->setFont(*m_grayFont);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_grayPalette;
    delete m_activePalette;
    delete m_grayFont;
    delete m_activeFont;
}

void MainWindow::on_reloadButton_clicked()
{
    ui->tableView->setModel(nullptr);
    m_rosterModel.reload();
    ui->tableView->setModel(&m_rosterModel);

    if (m_rosterModel.columnCount() == 4)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
        ui->tableView->setColumnWidth(0, 10);
    }
}

void MainWindow::on_filterEdit_textChanged(const QString &text)
{
    if (text.isEmpty())
    {
        ui->filterEdit->setPalette(*m_grayPalette);
        ui->filterEdit->setFont(*m_grayFont);
    }
    else
    {
        ui->filterEdit->setPalette(*m_activePalette);
        ui->filterEdit->setFont(*m_activeFont);
    }
}

void MainWindow::newDataFetched(QString text)
{
    ui->label->setText(text);
}
