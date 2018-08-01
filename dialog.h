#pragma once

#include <QDialog>

namespace Ui
{
class Dialog;
}

struct Roster;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent, const Roster &roster);
    ~Dialog();

private slots:
    void on_closeButton_clicked();

private:
    Ui::Dialog *ui;
    const Roster &m_roster;
    static int m_dialogPos[2];
};
