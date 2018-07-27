#pragma once

#include <QVariantMap>
#include <QVector>
#include "globals.h"

class rosterLoader
{
public:
    rosterLoader();

    void load();

protected:
    bool loadSingleRoster(const QVariantMap &from, Roster &to, QMap<int, QString> &groupsMap);
    bool loadSingleAccount(const QVariantMap &from, Roster::Account &to);

private:
    QVector<RosterGroup> m_groupArr;
    QVector<Roster> m_rosterArr;
    int m_invalidRostersCount;
    bool m_hasError;
};

