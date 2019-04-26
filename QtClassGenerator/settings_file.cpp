/*
* Copyright (c) 2019 Ramesh Nerella
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/

#include "settings_file.h"

#include <QStandardPaths>
#include <QTextStream>
#include <QDateTime>

namespace
{

const QString SETTINGS_FILE(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/.qtclasscreator.info");
const QString COPYRIGHT_SETTINGS_KEY("copyrightContent");
const QString AUTHOR_PLACEHOLDER("_AUTHOR_");
const QString ORG_NAME_PLACEHOLDER("_ORGANISATION_NAME_");
const QString DATETIME_PLACEHOLDER("_DATE_TIME_");
const QString FILE_NAME_PLACEHOLDER("_FILE_NAME_");
const QString FUNCTION_BLOCK_SEPARATOR("functionBlockSepartor");
const QString AUTHOR_NAME("authorName");
const QString ORGANISATION_NAME("orgName");
const QString TAB_SIZE("tabSize");
const QString SAVE_LOCATION("saveLocation");

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
        copyright.replace(AUTHOR_PLACEHOLDER, authorName());
        copyright.replace(ORG_NAME_PLACEHOLDER, organisationName());
        copyright.replace(DATETIME_PLACEHOLDER, QDateTime::currentDateTime().toString());
    }

    return copyright;
}

QString SettingsFile::formattedCopyrightContent(const QString& fileName) const
{
    QString copyright(copyrightContent(true));

    if (!copyright.isEmpty()) {
        copyright.replace(FILE_NAME_PLACEHOLDER, fileName);
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

void SettingsFile::setAuthorName(const QString& name)
{
    setValue(AUTHOR_NAME, name);
}

QString SettingsFile::authorName() const
{
    return value(AUTHOR_NAME).toString();
}

void SettingsFile::setOrganisationName(const QString& orgName)
{
    setValue(ORGANISATION_NAME, orgName);
}

QString SettingsFile::organisationName() const
{
    return value(ORGANISATION_NAME).toString();
}

void SettingsFile::setTabSize(int sz)
{
    setValue(TAB_SIZE, sz);
}

int SettingsFile::tabSize() const
{
    return value(TAB_SIZE, 4).toInt();
}

void SettingsFile::setSaveLocation(const QString& dirPath)
{
    setValue(SAVE_LOCATION, dirPath);
}

QString SettingsFile::saveLocation() const
{
    return value(SAVE_LOCATION).toString();
}
