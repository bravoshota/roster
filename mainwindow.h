#pragma once

#include <QMainWindow>

class RosterTableModel;
class Dialog;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resetTableView();

private slots:
    void on_updateButton_clicked();
    void on_filterEdit_textEdited(const QString &arg1);
    void on_tableView_doubleClicked(const QModelIndex &index);

    void newDataFetched(QString text);
    void finished(int result);

private:
    Ui::MainWindow *ui;
    Dialog *m_rosterDialog;
    RosterTableModel *m_tableModel;
};
