#pragma once

#include <QString>
#include <QDateTime>
#include <QIcon>

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

inline const char *toString(SEX sex)
{
    switch (sex)
    {
    case SEX::FEMALE      : return "Female";
    case SEX::MALE        : return "Male";
    case SEX::NOT_DEFINED : return "N/A";
    }
}

struct Roster
{
    Roster();
    ~Roster();

    QString id;
    int groupIndex;
    struct Account
    {
        QString id;
        QString userName;
        QString firstName;
        QString lastName;
        SEX sex;
        QString country;
        QString timezone;
        QString language;
        QString badge;
        QDateTime birthday;
        QDateTime createTime;
    } account;

    QIcon *icon32x32() const;
    QPixmap createPixmap(const QSize iconSize, QColor bkgndColor) const;

private:
    mutable QIcon *icon; // optimization: generate icon only when is accessed
};
