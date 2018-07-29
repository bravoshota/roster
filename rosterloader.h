#pragma once

#include <QVariantMap>
#include <QVector>
#include "globals.h"

class RosterLoader
{
public:
    RosterLoader();

    void load();

    const QVector<RosterGroup> &groups() const { return m_groupArr; }
    const QVector<Roster> &rosters() const { return m_rosterArr; }

protected:
    bool loadSingleRoster(const QVariantMap &from, Roster &to, QMap<int, QString> &groupsMap) const;
    bool loadSingleAccount(const QVariantMap &from, Roster::Account &to) const;

private:
    QVector<RosterGroup> m_groupArr;
    QVector<Roster> m_rosterArr;
    int m_invalidRostersCount;
    bool m_hasError;
};

