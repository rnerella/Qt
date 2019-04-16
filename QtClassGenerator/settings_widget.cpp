#include "settings_widget.h"
#include "settings_file.h"

#include <QFormLayout>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QDebug>

SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent)
{
    SettingsFile settings;
    auto formLayout = new QFormLayout(this);
    auto fnBlkChkBox = new QCheckBox(this);
    fnBlkChkBox->setChecked(settings.addFunctionBlockSeparator());
    formLayout->addRow(tr("Add Function Block Separator"), fnBlkChkBox);
    connect(fnBlkChkBox, &QCheckBox::toggled, this, [](bool toggled) {
        SettingsFile settings;
        settings.setAddFunctionBlockSeparator(toggled);
    });
    auto tabSizeSpin = new QSpinBox(this);
    tabSizeSpin->setMaximumWidth(70);
    tabSizeSpin->setRange(2, 10);
    formLayout->addRow(tr("Tab Size"), tabSizeSpin);
    connect(tabSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [](int sz) {
        SettingsFile settings;
        settings.setTabSize(sz);
    });
    auto authorText = new QLineEdit(settings.authorName(), this);
    authorText->setMaximumWidth(200);
    formLayout->addRow(tr("Author"), authorText);
    connect(authorText, &QLineEdit::textChanged, this, [](const QString & name) {
        SettingsFile settings;
        settings.setAuthorName(name);
    });
    auto orgNameText = new QLineEdit(settings.organisationName(), this);
    orgNameText->setMaximumWidth(200);
    formLayout->addRow(tr("Organisation"), orgNameText);
    connect(orgNameText, &QLineEdit::textChanged, this, [](const QString & name) {
        SettingsFile settings;
        settings.setOrganisationName(name);
    });
    auto copyrightText = new QPlainTextEdit(settings.copyrightContent(false), this);
    copyrightText->setMinimumSize(550, 300);
    formLayout->addRow(tr("Copyright"), copyrightText);
    connect(copyrightText, &QPlainTextEdit::textChanged, this, [copyrightText]() {
        SettingsFile settings;
        settings.setCopyrightContent(copyrightText->toPlainText());
    });

}
