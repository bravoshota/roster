#include "config.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>

QString Config::m_downloadURL;
QString Config::m_rosterFileName;
bool Config::m_hasError;

void Config::initialize()
{
    m_hasError = true;

    QString fullName = QDir::currentPath() + "/" + CONFIG_FILE;

    QFile configFile(fullName);
    if (!configFile.exists())
    {
        QMessageBox(QMessageBox::Warning, "Config Error",
                    QString("Missing config file: \"%1\"").arg(fullName)).exec();
        return;
    }

    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
        QMessageBox(QMessageBox::Warning, "Config Error",
                    QString("Can't open file: \"%1\"").arg(fullName)).exec();
        return;
    }

    QString configText = configFile.readAll();
    configFile.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(configText.toUtf8());
    if (jsonDocument.isObject() == false)
    {
        QMessageBox(QMessageBox::Warning, "Config Error", "not a valid object data").exec();
        return;
    }

    QJsonObject jsonObject = jsonDocument.object();
    QVariantMap map = jsonObject.toVariantMap();

    m_downloadURL = map["URL"].toString();
    if (m_downloadURL.isEmpty())
    {
        QMessageBox(QMessageBox::Warning, "Config Error", "Missing \"URL\" configuration").exec();
        return;
    }

    m_rosterFileName = map["rosterFile"].toString();
    if (m_rosterFileName.isEmpty())
    {
        QMessageBox(QMessageBox::Warning, "Config Error", "Missing \"rosterFile\" configuration").exec();
        return;
    }

    m_hasError = false;
}

QString Config::rosterFileFullName()
{
    return QDir::currentPath() + "/" + m_rosterFileName;
}
