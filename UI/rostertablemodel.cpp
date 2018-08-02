#include "rostertablemodel.h"
#include "downloader.h"
#include "config.h"
#include <QDebug>
#include <QPixmap>
#include <QPainter>

RosterTableModel::RosterTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_downloader(new Downloader(Config::downloadURL(), Config::rosterFileName()))
    , m_fetchedCount(0)
{
    connect(m_downloader, SIGNAL(downloadFinished(bool)),
            this        , SLOT(downloadFinished(bool)));
}

RosterTableModel::~RosterTableModel()
{
    delete m_downloader;
}

void RosterTableModel::init()
{
    if (QFile(Config::rosterFileFullName()).exists())
    {
        m_rosterParser.update();
        emit updateWindow(true);
    }
}

int RosterTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_fetchedCount;
}

int RosterTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 4;
}

QVariant RosterTableModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    {
        auto addSingleRow = [this](int row, int column)->QVariant
        {
            const auto &roster =  m_rosterParser.rosters()[row];

            switch (column)
            {
            case 1: return roster.account.firstName;
            case 2: return roster.account.lastName;
            case 3: return m_rosterParser.groups()[roster.groupIndex].name;
            }
            return QVariant();
        };

        if (m_filterText.isEmpty())
        {
            if (index.column() >= 1 && index.column() < 4 &&
                index.row() >= 0 && index.row() < m_rosterParser.rosters().count())
            {
                QVariant variant = addSingleRow(index.row(), index.column());
                if (variant.isValid())
                    return variant;
            }
        }
        else
        {
            if (index.column() >= 1 && index.column() < 4 &&
                index.row() >= 0 && index.row() < m_filteredResults.count())
            {
                QVariant variant = addSingleRow(m_filteredResults[index.row()], index.column());
                if (variant.isValid())
                    return variant;
            }
        }
        break;
    }
    case Qt::DecorationRole:
    {
        if (m_filterText.isEmpty())
        {
            if (index.column() == 0 && index.row() >= 0 && index.row() < m_rosterParser.rosters().count())
                return QVariant(*m_rosterParser.rosters()[index.row()].icon32x32());
        }
        else
        {
            if (index.column() == 0 && index.row() >= 0 && index.row() < m_filteredResults.count())
                return QVariant(*m_rosterParser.rosters()[m_filteredResults[index.row()]].icon32x32());
        }
        break;
    }
    }

    return QVariant();
}

QVariant RosterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool RosterTableModel::canFetchMore(const QModelIndex &/*parent*/) const
{
    return m_fetchedCount < m_rosterParser.rosters().size();
}

void RosterTableModel::fetchMore(const QModelIndex &parent)
{
    const int pageSize = 64;

    if (m_filterText.isEmpty())
    {
        int remaining = m_rosterParser.rosters().size() - m_fetchedCount;
        int itemsToFetch = qMin(pageSize, remaining);

        if (itemsToFetch <= 0)
            return;

        beginInsertRows(parent, m_fetchedCount, m_fetchedCount + itemsToFetch - 1);

        m_fetchedCount += itemsToFetch;

        endInsertRows();

        QString outputText;
        outputText.sprintf("Showing %d out of %d..", m_fetchedCount, m_rosterParser.rosters().size());
        emit newDataFetched(outputText);
    }
    else
    {
        QVector<int> filteredResults;
        filteredResults.reserve(pageSize);
        while (m_fetchedCount < m_rosterParser.rosters().size())
        {
            const Roster::Account &account = m_rosterParser.rosters()[m_fetchedCount].account;
            if ((account.firstName + " " + account.lastName).toLower().contains(m_filterText))
                filteredResults.push_back(m_fetchedCount);

            ++m_fetchedCount;

            if (filteredResults.size() == pageSize)
                break;
        }

        if (filteredResults.size() > 0)
        {
            beginInsertRows(parent,
                            m_filteredResults.size(),
                            m_filteredResults.size() + filteredResults.size() - 1);

            m_filteredResults.append(filteredResults);

            endInsertRows();

            QString outputText;
            outputText.sprintf("Showing filtered results %d out of total %d..",
                               m_filteredResults.size(),
                               m_rosterParser.rosters().size());
            emit newDataFetched(outputText);
        }
        else
        {
            emit updateWindow(true);
        }
    }
}

void RosterTableModel::clear()
{
    beginResetModel();
    m_fetchedCount = 0;
    m_filteredResults.clear();
    endResetModel();
}

void RosterTableModel::download()
{
    m_downloader->execute();
}

void RosterTableModel::setFilter(const QString &filterText, bool invokeFetching)
{
    m_filterText = filterText.toLower();

    if (invokeFetching)
        clear();
}

const Roster &RosterTableModel::getRoster(int index) const
{
    if (m_filterText.isEmpty())
        return m_rosterParser.rosters()[index];

    if (index >= m_filteredResults.size())
    {
        static Roster tmpRoster;
        return tmpRoster;
    }

    return m_rosterParser.rosters()[m_filteredResults[index]];
}

void RosterTableModel::downloadFinished(bool success)
{
    if (success)
    {
        m_rosterParser.update();
        setFilter("", false);
    }

    emit updateWindow(success);
}
