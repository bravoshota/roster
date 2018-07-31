#pragma once

#include "rosterparser.h"
#include <QAbstractTableModel>

class RosterTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RosterTableModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void update();
    void setFilter(const QString &filterText, bool invokeFetching = true);

signals:
    void newDataFetched(QString);

protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

private:
    RosterParser m_rosterParser;
    QVector<int> m_filteredResults;
    QString m_filterText;
    int m_fetchedCount;
};
