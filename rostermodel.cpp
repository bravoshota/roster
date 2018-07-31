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
    return m_fetchedCount;
}

int RosterModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 4;
}

QVariant RosterModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto addSingleRow = [this](int row, int column)->QVariant
        {
            const auto &roster =  m_rosterLoader.rosters()[row];

            switch (column)
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
            case 3: return m_rosterLoader.groups()[roster.groupIndex].name;
            }
            return QVariant();
        };

        if (m_filterText.isEmpty())
        {
            if (index.column() >= 0 && index.column() < 4 &&
                index.row() >= 0 && index.row() < m_rosterLoader.rosters().count())
            {
                QVariant variant = addSingleRow(index.row(), index.column());
                if (variant.isValid())
                    return variant;
            }

            Q_ASSERT(false);
        }
        else
        {
            if (index.column() >= 0 && index.column() < 4 &&
                index.row() >= 0 && index.row() < m_filteredResults.count())
            {
                QVariant variant = addSingleRow(m_filteredResults[index.row()], index.column());
                if (variant.isValid())
                    return variant;
            }
        }
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

void RosterModel::fetchMore(const QModelIndex &parent)
{
    const int pageSize = 64;

    if (m_filterText.isEmpty())
    {
        int remaining = m_rosterLoader.rosters().size() - m_fetchedCount;
        int itemsToFetch = qMin(pageSize, remaining);

        if (itemsToFetch <= 0)
            return;

        beginInsertRows(parent, m_fetchedCount, m_fetchedCount + itemsToFetch - 1);

        m_fetchedCount += itemsToFetch;

        endInsertRows();

        QString outputText;
        outputText.sprintf("Showing %d out of %d..", m_fetchedCount, m_rosterLoader.rosters().size());
        emit newDataFetched(outputText);
    }
    else
    {
        int fetchedCount = 0;
        while (m_fetchedCount < m_rosterLoader.rosters().size())
        {
            const Roster::Account &account = m_rosterLoader.rosters()[m_fetchedCount].account;
            if ((account.firstName + " " + account.lastName).toLower().contains(m_filterText))
            {
                m_filteredResults.push_back(m_fetchedCount);
                ++fetchedCount;
            }

            ++m_fetchedCount;

            if (fetchedCount == pageSize)
                break;
        }

        if (fetchedCount > 0)
        {
            beginInsertRows(parent, m_filteredResults.size() - fetchedCount, m_filteredResults.size() - 1);
            endInsertRows();

            QString outputText;
            outputText.sprintf("Showing filtered results %d out of total %d..",
                               m_filteredResults.size(),
                               m_rosterLoader.rosters().size());
            emit newDataFetched(outputText);
        }
    }
}

void RosterModel::update()
{
    Downloader downloader(Config::downloadURL(), Config::rosterFileName());
    downloader.execute();

    // slot here to be connected on downloadFinished signal

    m_rosterLoader.update();

    setFilter("");
}

void RosterModel::setFilter(const QString &filterText, bool invokeFetching)
{
    m_filterText = filterText.toLower();

    if (invokeFetching)
    {
        m_fetchedCount = 0;
        m_filteredResults.clear();
    }
}
