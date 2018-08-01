#include "dialog.h"
#include "ui_dialog.h"
#include "common.h"

namespace
{
const QStringList g_displayRows
{
    "Id: ",
    "Username: ",
    "First name: ",
    "Last name: ",
    "Sex: ",
    "Country: ",
    "Timezone: ",
    "Language: ",
    "Badge: ",
    "Birthday: ",
    "Create time: "
};
}

int Dialog::m_dialogPos[2] {-1, -1};

Dialog::Dialog(QWidget *parent, const Roster &roster)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , m_roster(roster)
{
    ui->setupUi(this);

    QPixmap pixMap = roster.createPixmap(QSize(128, 128), palette().color(backgroundRole()));
    ui->label->setPixmap(pixMap);

    ui->tableWidget->setColumnWidth(0, 120);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    for (int i = 0; i < g_displayRows.size(); ++i)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(g_displayRows[i]));
        ui->tableWidget->item(i, 0)->setTextAlignment(Qt::AlignRight);
    }

    ui->tableWidget->setItem(0 , 1, new QTableWidgetItem(roster.account.id));
    ui->tableWidget->setItem(1 , 1, new QTableWidgetItem(roster.account.userName));
    ui->tableWidget->setItem(2 , 1, new QTableWidgetItem(roster.account.firstName));
    ui->tableWidget->setItem(3 , 1, new QTableWidgetItem(roster.account.lastName));
    ui->tableWidget->setItem(4 , 1, new QTableWidgetItem(toString(roster.account.sex)));
    ui->tableWidget->setItem(5 , 1, new QTableWidgetItem(roster.account.country));
    ui->tableWidget->setItem(6 , 1, new QTableWidgetItem(roster.account.timezone));
    ui->tableWidget->setItem(7 , 1, new QTableWidgetItem(roster.account.language));
    ui->tableWidget->setItem(8 , 1, new QTableWidgetItem(roster.account.badge));
    ui->tableWidget->setItem(9 , 1, new QTableWidgetItem(roster.account.birthday.toString("dd-MMM-yyyy")));
    ui->tableWidget->setItem(10, 1, new QTableWidgetItem(roster.account.createTime.toString("dd-MMM-yyyy HH:mm")));

    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    setWindowTitle(roster.account.firstName + " " +
                   roster.account.lastName + "'s details");

    // move dialog to last stored location
    if (m_dialogPos[0] > -1 && m_dialogPos[1] > -1)
        move(m_dialogPos[0], m_dialogPos[1]);
}

Dialog::~Dialog()
{
    // store positions to open dialog at the same location next time
    m_dialogPos[0] = pos().rx();
    m_dialogPos[1] = pos().ry();

    ui->tableWidget->clear();

    delete ui;
}

void Dialog::on_closeButton_clicked()
{
    done(0);
}
