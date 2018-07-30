#include "rostermodel.h"
#include "downloader.h"
#include "config.h"
#include <QDebug>

RosterModel::RosterModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_fetchedCount(0)
{
}

int RosterModel::rowCount(const QModelIndex &/*parent*/) const
{
    qDebug()<<"row count = "<<m_rosterLoader.rosters().size();
    return m_fetchedCount;
}

int RosterModel::columnCount(const QModelIndex &/*parent*/) const
{
    qDebug()<<"column count";
    return 4;
}

QVariant RosterModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<"role = "<<role;

    if (role == Qt::DisplayRole)
    {
        if (index.column() >= 0 && index.column() < 4 &&
            index.row() >= 0 && index.row() < m_rosterLoader.rosters().count())
        {
            const auto &roster =  m_rosterLoader.rosters()[index.row()];
            const auto &groups = m_rosterLoader.groups();

            switch (index.column())
            {
            case 0:
            {
                QString avatar;
                if (!roster.account.firstName.isEmpty() &&
                        !roster.account.lastName.isEmpty())
                {
                    avatar.push_back(roster.account.firstName[0].toUpper());
                    avatar.push_back(roster.account.lastName[0].toUpper());
                }
                return avatar;
            }
            case 1: return roster.account.firstName;
            case 2: return roster.account.lastName;
            case 3: return groups[roster.groupIndex].name;
            default: break;
            }
        }

        Q_ASSERT(false);
    }

    return QVariant();
}

QVariant RosterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0: return QString("");
            case 1: return QString("First Name");
            case 2: return QString("Last Name");
            case 3: return QString("Group");
            }
        }
    }

    return QVariant();
}

bool RosterModel::canFetchMore(const QModelIndex &/*parent*/) const
{
    return m_fetchedCount < m_rosterLoader.rosters().size();
}

void RosterModel::fetchMore(const QModelIndex &/*parent*/)
{
    const int pageSize = 64;
    int remaining = m_rosterLoader.rosters().size() - m_fetchedCount;
    int itemsToFetch = qMin(pageSize, remaining);

    if (itemsToFetch <= 0)
        return;

    beginInsertRows(QModelIndex(), m_fetchedCount, m_fetchedCount + itemsToFetch - 1);

    m_fetchedCount += itemsToFetch;

    endInsertRows();

    QString outputText;
    outputText.sprintf("Showing %d out of %d..", m_fetchedCount, m_rosterLoader.rosters().size());
    emit newDataFetched(outputText);
}

void RosterModel::reload()
{
    Downloader downloader(Config::downloadURL(), Config::rosterFileName());
    downloader.execute();

    // slot here to be connected on downloadFinished signal

    m_rosterLoader.load();

    m_fetchedCount = 0;
}
