#pragma once

#include <QMainWindow>

class RosterTableModel;

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

    void newDataFetched(QString text);

private:
    Ui::MainWindow *ui;
    RosterTableModel *m_tableModel;
};
