#include "rosterloader.h"
#include "config.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <algorithm>

rosterLoader::rosterLoader()
    : m_invalidRostersCount(0)
    , m_hasError(false)
{
}

void rosterLoader::load()
{
    m_invalidRostersCount = 0;
    m_rosterArr.clear();
    m_groupArr.clear();
    m_hasError = true;

    QString fullName = QDir::currentPath() + "/" + Config::rosterFileName();

    QFile rosterFile(fullName);
    if (!rosterFile.exists())
    {
        QMessageBox(QMessageBox::Warning, "Roster loader Error",
                    QString("Missing file: \"%1\"").arg(Config::rosterFileName()));
        return;
    }

    if (rosterFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox(QMessageBox::Warning, "Roster loader Error",
                    QString("Can't open file: \"%1\"").arg(Config::rosterFileName()));
        return;
    }

    QString rosterTextData = rosterFile.readAll();
    rosterFile.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(rosterTextData.toUtf8());
    if (jsonDocument.isObject() == false)
    {
        QMessageBox(QMessageBox::Warning, "Roster loader Error", "not a valid object data");
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    QVariantMap rostersMap = jsonObject.toVariantMap();

    int rostersCount = rostersMap["rosterSize"].toInt();
    QVariantList rostersList = rostersMap["roster"].toList();

    if (rostersCount != rostersList.count())
    {
        QMessageBox(QMessageBox::Warning,
                    "Roster data error",
                    "roster array count is not equal to rosterSize parameter");
        return;
    }

    QMap<int, QString> groupsMap;
    int currentRoster = 0;
    m_rosterArr.resize(rostersCount);

    // parsing and validation
    for (QVariant &rosterVariant : rostersList)
    {
        if (rosterVariant.isValid() == false)
            continue;

        if (loadSingleRoster(rosterVariant.toMap(), m_rosterArr[currentRoster], groupsMap))
            ++currentRoster; // if single roster is successfully converted then do increment
    }

    // calculate invalid rosters count
    m_invalidRostersCount = rostersCount - currentRoster;

    // if at least one roster is invalid then consider error has occured
    m_hasError = (m_invalidRostersCount > 0);

    // it won't affect if everithing passed validation process
    m_rosterArr.resize(rostersCount);

    // sort by group id and by name inside it
    // using STL for sorting because passing lambda hits warning for qSort (QtAlgorithms):
    //'qSort<Roster *, (lambda at ..)>' is deprecated: Use std::sort [-Wdeprecated-declarations]
    std::sort(m_rosterArr.begin(), m_rosterArr.end(), [&](const Roster &r1, const Roster &r2)
    {
        // either id is less or sorted alphabetically if ids are equal
        return  (r1.id < r2.id) ||
                (r1.id ==r2.id && (r1.account.firstName + r1.account.lastName) < (r2.account.firstName + r2.account.lastName));
    });

    // generate groups array from map
    m_groupArr.resize(groupsMap.size());
    int groupIndex = 0;
    for (auto it = groupsMap.begin(); it != groupsMap.end(); ++it, ++groupIndex)
    {
        m_groupArr[groupIndex].id = it.key();
        m_groupArr[groupIndex].name = it.value();
    }

    // reorder groups to 0-based indices
    // it will help us to quick access to related group
    groupIndex = -1;
    int currentGroupId = -1;
    for (Roster &roster : m_rosterArr)
    {
        if (currentGroupId!= roster.groupIndex)
        {
            currentGroupId = roster.groupIndex;
            ++groupIndex;
        }
        roster.groupIndex = groupIndex;
    }
}

bool rosterLoader::loadSingleRoster(const QVariantMap &from, Roster &to, QMap<int, QString> &groupsMap)
{
    // id
    {
        to.id = from["id"].toString();
        if (to.id.isEmpty())
            return false;
    }

    // group
    {
        bool ok;
        to.groupIndex = from["groupOrder"].toInt(&ok);
        if (!ok || to.groupIndex < 0)
            return false;

        QString groupName = from["group"].toString();
        if (groupName.isEmpty())
            return false;

        auto groupIt = groupsMap.find(to.groupIndex);
        if (groupIt == groupsMap.end())
        {
            // if there is no such group, let's add it
            groupsMap[to.groupIndex] = groupName;
        }
        else
        {
            // if there already exists such group then let's compare
            // that group names are identical
            if (groupsMap[to.groupIndex] != groupName)
                return false;
        }
    }

    // account
    {
        QVariant accountVariant = from["account"];
        if (accountVariant.isValid() == false)
            return false;

        if (loadSingleAccount(accountVariant.toMap(), to.account) == false)
            return false;
    }

    return true;
}

bool rosterLoader::loadSingleAccount(const QVariantMap &from, Roster::Account &to)
{
    // userName
    {
        to.userName = from["username"].toString();
        if (to.userName.isEmpty())
            return false;
    }

    // firstName
    {
        to.firstName = from["firstName"].toString();
        if (to.firstName.isEmpty())
            return false;
    }

    // lastName
    {
        to.lastName = from["lastName"].toString();
        if (to.lastName.isEmpty())
            return false;
    }

    // sex
    {
        QString sex = from["sex"].toString();
        if (sex == "MALE")
        {
            to.sex = SEX::MALE;
        }
        else if (sex == "FEMALE")
        {
            to.sex = SEX::FEMALE;
        }
        else if (sex == "NOT_DEFINED")
        {
            to.sex = SEX::NOT_DEFINED;
        }
        else
        {
            return false;
        }
    }

    // country
    {
        to.country = from["country"].toString();
        if (to.country.isEmpty())
            return false;
    }

    // language
    {
        to.language = from["language"].toString();
        if (to.language.isEmpty())
            return false;
    }

    // birthday
    {
        to.birthday = from["birthday"].toDateTime();
        bool ok;
        int timeValue = from["birthday"].toUInt(&ok);
        if (!ok)
            return false;

        to.birthday = QDateTime::fromTime_t(timeValue);
        if (!to.birthday.isValid())
            return false;
    }

    return true;
}
