#pragma once

#include <QString>
#include <QDateTime>

struct RosterGroup
{
    int id;
    QString name;
};

enum class SEX
{
    NOT_DEFINED = 0,
    MALE = 1,
    FEMALE = 2
};

struct Roster
{
    QString id;
    int groupIndex;
    struct Account
    {
        QString userName;
        QString firstName;
        QString lastName;
        SEX sex;
        QString country;
        QString language;
        QDateTime birthday;
        //    "anniversary": 0,
        //    "badge": "USER",
        //    "contacts": 0,
        //    "createTime": 1457081728962,
        //    "id": "d8226a90-e1e6-11e5-a0ec-cf70b5e21e57",
        //    "otherSpecialEvent": 0,
        //    "parentId": "2294ebd0-e14b-11e5-b89b-413e84128188",
        //    "timezone": "GMT+3",
    } account;
};
