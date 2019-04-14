#ifndef SETTINGS_FILE_H
#define SETTINGS_FILE_H

#include <QSettings>

class SettingsFile : public QSettings
{
    Q_OBJECT

public:

    explicit SettingsFile(QObject* prnt=nullptr);

    void setCopyrightContent(const QString& content);

    QString copyrightContent(bool replacePlaceholders=true) const;

    QString formattedCopyrightContent() const;

    void setAddFunctionBlockSeparator(bool set);

    bool addFunctionBlockSeparator() const;

};

#endif // SETTINGS_FILE_H
