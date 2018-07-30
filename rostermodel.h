#pragma once

#include "rosterloader.h"
#include <QAbstractTableModel>

class RosterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RosterModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void reload();

signals:
    void newDataFetched(QString);

protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

private:
    RosterLoader m_rosterLoader;
    int m_fetchedCount;
};
