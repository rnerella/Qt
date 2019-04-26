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

#include "settings_widget.h"
#include "settings_file.h"

#include <QFormLayout>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QDebug>

SettingsWidget::SettingsWidget(QWidget* parent) : QWidget(parent)
{
    SettingsFile settings;
    auto formLayout = new QFormLayout(this);
    auto fnBlkChkBox = new QCheckBox(this);
    fnBlkChkBox->setChecked(settings.addFunctionBlockSeparator());
    fnBlkChkBox->setToolTip(tr("Adds //-------- before every function definition"));
    formLayout->addRow(tr("Add Function Block Separator"), fnBlkChkBox);
    connect(fnBlkChkBox, &QCheckBox::toggled, this, [](bool toggled) {
        SettingsFile settings;
        settings.setAddFunctionBlockSeparator(toggled);
    });
    auto tabSizeSpin = new QSpinBox(this);
    tabSizeSpin->setMaximumWidth(70);
    tabSizeSpin->setRange(2, 10);
    tabSizeSpin->setValue(settings.tabSize());
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
    QString info(tr("Placeholders") + QStringLiteral(": _AUTHOR_, _ORGANISATION_NAME_, _DATE_TIME_, _FILE_NAME_"));
    auto infoLabel = new QLabel(info, this);
    infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    auto copyLayout = new QVBoxLayout();
    copyLayout->addWidget(infoLabel);
    copyLayout->addWidget(copyrightText);
    formLayout->addRow(tr("Copyright"), copyLayout);
    connect(copyrightText, &QPlainTextEdit::textChanged, this, [copyrightText]() {
        SettingsFile settings;
        settings.setCopyrightContent(copyrightText->toPlainText());
    });

}
