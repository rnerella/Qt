#include "settings_widget.h"
#include "settings_file.h"

#include <QFormLayout>
#include <QCheckBox>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent)
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
}
