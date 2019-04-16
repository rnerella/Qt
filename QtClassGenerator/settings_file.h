#ifndef SETTINGS_FILE_H
#define SETTINGS_FILE_H

#include <QSettings>

class SettingsFile : public QSettings
{
    Q_OBJECT

public:

    explicit SettingsFile(QObject* prnt = nullptr);

    void setCopyrightContent(const QString& content);

    QString copyrightContent(bool replacePlaceholders = true) const;

    QString formattedCopyrightContent(const QString& fileName) const;

    void setAddFunctionBlockSeparator(bool set);

    bool addFunctionBlockSeparator() const;

    void setAuthorName(const QString& name);

    QString authorName() const;

    void setOrganisationName(const QString& orgName);

    QString organisationName() const;

    void setTabSize(int sz);

    int tabSize() const;

};

#endif // SETTINGS_FILE_H
