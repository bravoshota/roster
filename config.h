#pragma once

#include <QString>

class Config
{
public:
    static void initialize();

    static const QString &downloadURL() { return m_downloadURL; }
    static const QString &rosterFileName() { return m_rosterFileName; }
    static bool hasError() { return m_hasError; }

private:
    static QString m_downloadURL;
    static QString m_rosterFileName;
    static bool m_hasError;
};

