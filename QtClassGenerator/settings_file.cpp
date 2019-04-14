#include "settings_file.h"

#include <QStandardPaths>
#include <QTextStream>

namespace  {

const QString SETTINGS_FILE(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/.qtclasscreator.info");
const QString COPYRIGHT_SETTINGS_KEY("copyrightContent");
const QString AUTHOR_PLACEHOLDER("_AUTHOR_");
const QString COMPANY_NAME_PLACEHOLDER("_COMPANY_NAME_");
const QString DATETIME_PLACEHOLDER("_DATE_TIME_");
const QString FUNCTION_BLOCK_SEPARATOR("functionBlockSepartor");

}

SettingsFile::SettingsFile(QObject* prnt)
    : QSettings(SETTINGS_FILE, QSettings::IniFormat, prnt)
{
}

void SettingsFile::setCopyrightContent(const QString& content)
{
    setValue(COPYRIGHT_SETTINGS_KEY, content);
}

QString SettingsFile::copyrightContent(bool replacePlaceholders) const
{
    QString copyright(value(COPYRIGHT_SETTINGS_KEY).toString());
    if (!copyright.isEmpty() && replacePlaceholders) {
        copyright.replace(AUTHOR_PLACEHOLDER, "rnerella");
    }
    return copyright;
}

QString SettingsFile::formattedCopyrightContent() const
{
    QString copyright(copyrightContent());
    if (!copyright.isEmpty()) {
        QString str;
        QTextStream stream(&str);
        QString stars(99, '*');
        stream << '/' << stars << endl;
        stream << copyright << endl;
        stream << stars << '/' << endl << endl;
        return str;
    }
    return copyright;
}

void SettingsFile::setAddFunctionBlockSeparator(bool set)
{
    setValue(FUNCTION_BLOCK_SEPARATOR, set);
}

bool SettingsFile::addFunctionBlockSeparator() const
{
    return value(FUNCTION_BLOCK_SEPARATOR, false).toBool();
}
