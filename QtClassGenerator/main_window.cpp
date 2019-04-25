#include "main_window.h"
#include "variable_list_model.h"
#include "code_generator.h"
#include "settings_file.h"
#include "settings_widget.h"

#include <QCoreApplication>
#include <QTableView>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QHeaderView>
#include <QCheckBox>
#include <QFormLayout>
#include <QInputDialog>
#include <QTimer>
#include <QStatusBar>
#include <QStandardPaths>
#include <QFileDialog>
#include <QTabWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_errorLabel(nullptr),
    m_errorClearTimer(new QTimer(this)),
    m_codeGenerator(new CodeGenerator(this))
{
    VariableListModel::instance()->setParent(this);

    m_errorClearTimer->setInterval(1500);
    m_errorClearTimer->setSingleShot(true);

    auto tabbedWidget = new QTabWidget(this);
    setCentralWidget(tabbedWidget);

    auto cntrlWidget = new QWidget(this);
    tabbedWidget->addTab(cntrlWidget, tr("Class Details"));

    auto settingsWidget = new SettingsWidget(this);
    tabbedWidget->addTab(settingsWidget, tr("Settings"));

    auto cntrlLayout = new QVBoxLayout(cntrlWidget);

    auto clasDetailsPrnt = new QFrame(cntrlWidget);
    clasDetailsPrnt->setFixedHeight(120);
    clasDetailsPrnt->setMaximumWidth(400);
    auto classDetailsLayout = new QFormLayout(clasDetailsPrnt);
    classDetailsLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    m_namespaceNameInputField = new QLineEdit(clasDetailsPrnt);
    m_classNameInputField = new QLineEdit(clasDetailsPrnt);
    m_baseClassNameInputField = new QLineEdit(clasDetailsPrnt);
    m_fileNameInputField = new QLineEdit(clasDetailsPrnt);
    classDetailsLayout->addRow(tr("Namespace Name: "), m_namespaceNameInputField);
    classDetailsLayout->addRow(tr("Class Name: "), m_classNameInputField);
    classDetailsLayout->addRow(tr("Base Class Name: "), m_baseClassNameInputField);
    classDetailsLayout->addRow(tr("File Name: "), m_fileNameInputField);

    auto optionsPrnt = new QWidget(cntrlWidget);
    auto optionsLayout = new QFormLayout(optionsPrnt);
    m_qObjectChkBox = new QCheckBox(optionsPrnt);
    auto generateCppChkBox = new QCheckBox(optionsPrnt);
    auto generateFileNameChkbox = new QCheckBox(optionsPrnt);
    auto addSuperChkbox = new QCheckBox(optionsPrnt);
    optionsLayout->addRow(tr("Is QObject:"), m_qObjectChkBox);
    optionsLayout->addRow(tr("Generate CPP:"), generateCppChkBox);
    optionsLayout->addRow(tr("Generate File Name:"), generateFileNameChkbox);
    optionsLayout->addRow(tr("Add super typedef:"), addSuperChkbox);

    m_generateCodeButton = new QPushButton(tr("Generate"), clasDetailsPrnt);
    m_generateCodeButton->setFixedHeight(90);

    auto mainDetailsPrnt = new QFrame(cntrlWidget);
    mainDetailsPrnt->setFixedHeight(125);
    auto mainDetailsLayout = new QHBoxLayout(mainDetailsPrnt);
    mainDetailsLayout->addWidget(clasDetailsPrnt);
    mainDetailsLayout->addWidget(optionsPrnt);
    mainDetailsLayout->addWidget(m_generateCodeButton, 0, Qt::AlignVCenter);

    m_variablesTableView = new QTableView(cntrlWidget);
    m_variablesTableView->verticalHeader()->hide();
    m_variablesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_variablesTableView->setSelectionMode(QTableView::SingleSelection);
    m_variablesTableView->setSelectionBehavior(QTableView::SelectRows);
    m_variablesTableView->setModel(VariableListModel::instance());
    m_variablesTableView->setShowGrid(false);
    m_variablesTableView->setAlternatingRowColors(true);

    auto addNewControlParent = new QFrame(cntrlWidget);
    addNewControlParent->setFixedHeight(70);
    auto newControlLayout = new QHBoxLayout(addNewControlParent);
    auto label = new QLabel(tr("Data Type: "), addNewControlParent);
    newControlLayout->addWidget(label);
    m_typeCombo = new QComboBox(addNewControlParent);
    m_typeCombo->setMinimumWidth(200);
    newControlLayout->addWidget(m_typeCombo, 0, Qt::AlignVCenter);
    newControlLayout->addWidget(new QLabel(tr("Name: "), addNewControlParent));
    m_varNameInputField = new QLineEdit(addNewControlParent);
    //varNameInputField->setMinimumWidth(300);
    newControlLayout->addWidget(m_varNameInputField);
    m_isPropertyChkBox = new QCheckBox(tr("Is QProperty "), addNewControlParent);
    newControlLayout->addWidget(m_isPropertyChkBox);
    m_addSetterChkBox = new QCheckBox(tr("Add Setter "), addNewControlParent);
    newControlLayout->addWidget(m_addSetterChkBox);
    m_addGetterChkBox = new QCheckBox(tr("Add Getter "), addNewControlParent);
    newControlLayout->addWidget(m_addGetterChkBox);
    m_addVariableButton = new QPushButton(tr("Add Variable"), addNewControlParent);
    newControlLayout->addWidget(m_addVariableButton);

    m_typeCombo->addItem(tr("<Custom>"), static_cast<QVariant::Type>(QMetaType::User));

    for (int i = 1; i < 125; ++i) {
        auto type = static_cast<QMetaType::Type>(i);
        QString typeName(QVariant::typeToName(i));
        //qDebug() << i << typeName;

        if (!typeName.isEmpty() && type != QMetaType::Void) {
            m_typeCombo->addItem(typeName, static_cast<QVariant::Type>(i));
        }
    }

    m_typeCombo->model()->sort(0);

    auto bottomControlWidget = new QWidget(cntrlWidget);
    auto bottomLayout = new QHBoxLayout(bottomControlWidget);
    bottomLayout->addStretch(1);
    m_deleteVarButton = new QPushButton(tr("Delete Selected"), bottomControlWidget);
    bottomLayout->addWidget(m_deleteVarButton);
    m_moveUpVarButton = new QPushButton(tr("Move Up"), bottomControlWidget);
    bottomLayout->addWidget(m_moveUpVarButton);
    m_moveDownVarButton = new QPushButton(tr("Move Down"), bottomControlWidget);
    bottomLayout->addWidget(m_moveDownVarButton);

    m_errorLabel = new QLabel(cntrlWidget);
    auto labelPalette(m_errorLabel->palette());
    labelPalette.setColor(QPalette::WindowText, QColor(Qt::red));
    m_errorLabel->setPalette(labelPalette);
    m_errorLabel->setFixedHeight(50);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    auto labelFont(m_errorLabel->font());
    labelFont.setBold(true);
    labelFont.setPointSize(labelFont.pointSize() * 2);
    m_errorLabel->setFont(labelFont);

    cntrlLayout->addWidget(mainDetailsPrnt);
    cntrlLayout->addWidget(addNewControlParent);
    cntrlLayout->addWidget(m_variablesTableView);
    cntrlLayout->addWidget(bottomControlWidget);
    cntrlLayout->addStretch(1);
    cntrlLayout->addWidget(m_errorLabel);

    connect(m_namespaceNameInputField, &QLineEdit::textChanged, this, &MainWindow::namespaceNameChanged);
    connect(m_classNameInputField, &QLineEdit::textChanged, this, &MainWindow::classNameChanged);
    connect(m_generateCodeButton, &QPushButton::clicked, this, &MainWindow::generateCodeButtonClicked);
    connect(m_varNameInputField, &QLineEdit::textChanged, this, &MainWindow::variableNameChanged);
    connect(m_addVariableButton, &QPushButton::clicked, this, &MainWindow::addVariableButtonClicked);
    connect(m_qObjectChkBox, &QCheckBox::toggled, this, &MainWindow::qObjectCheckboxToggled);
    connect(generateCppChkBox, &QCheckBox::toggled, m_codeGenerator, &CodeGenerator::setGenerateCpp);
    connect(addSuperChkbox, &QCheckBox::toggled, m_codeGenerator, &CodeGenerator::setAddSuperTypedef);
    connect(generateFileNameChkbox, &QCheckBox::toggled, m_codeGenerator, &CodeGenerator::setGenerateFilename);
    connect(m_errorClearTimer, &QTimer::timeout, m_errorLabel, &QLabel::clear);
    connect(m_baseClassNameInputField, &QLineEdit::textChanged, m_codeGenerator, &CodeGenerator::setBaseClassName);
    connect(m_fileNameInputField, &QLineEdit::textChanged, m_codeGenerator, &CodeGenerator::setFileName);
    connect(m_codeGenerator, &CodeGenerator::isQObjectChanged, m_isPropertyChkBox, &QCheckBox::setEnabled);
    connect(m_codeGenerator, &CodeGenerator::fileNameGenerated, m_fileNameInputField, &QLineEdit::setText);
    connect(m_codeGenerator, &CodeGenerator::generateFilenameChanged, m_fileNameInputField, &QLineEdit::setDisabled);
    connect(m_isPropertyChkBox, &QCheckBox::toggled, this, &MainWindow::qPropertyCheckboxToggled);
    connect(m_variablesTableView, &QTableView::clicked, this, &MainWindow::updateVariableEditButtons);
    connect(m_variablesTableView, &QTableView::activated, this, &MainWindow::updateVariableEditButtons);
    connect(m_deleteVarButton, &QPushButton::clicked, this, &MainWindow::deleteSelectedVariable);
    connect(m_moveUpVarButton, &QPushButton::clicked, this, &MainWindow::moveVariableUp);
    connect(m_moveDownVarButton, &QPushButton::clicked, this, &MainWindow::moveVariableDown);

    connect(VariableListModel::instance(), &QAbstractTableModel::rowsMoved, this, [&](const QModelIndex & parent, int start, int end, const QModelIndex & destination, int row) {
        //        qDebug() << parent << start << end << destination << row;
        Q_UNUSED(parent);
        Q_UNUSED((start + end + row));
        m_variablesTableView->setCurrentIndex(destination);
    });

    m_qObjectChkBox->setChecked(true);
    generateCppChkBox->setChecked(true);
    generateFileNameChkbox->setChecked(true);
    addSuperChkbox->setChecked(true);
    m_generateCodeButton->setEnabled(false);
    m_deleteVarButton->setEnabled(false);
    m_moveUpVarButton->setEnabled(false);
    m_moveDownVarButton->setEnabled(false);
    m_addVariableButton->setEnabled(false);
    m_fileNameInputField->setEnabled(!generateFileNameChkbox->isChecked());

    statusBar()->addWidget(new QLabel("Qt Class Generator | R Nerella", statusBar()), 1);
    statusBar()->addWidget(new QLabel(qApp->applicationVersion(), statusBar()));

    setMinimumSize(1000, 450);
}

MainWindow::~MainWindow() = default;

void MainWindow::setError(const QString& err)
{
    m_errorLabel->setText(err);
    m_errorClearTimer->start();
}

void MainWindow::namespaceNameChanged(const QString& namespaceName)
{
    m_codeGenerator->setNamespaceName(namespaceName);
}

void MainWindow::classNameChanged(const QString& className)
{
    m_codeGenerator->setClassName(className);
    m_generateCodeButton->setEnabled(!className.isEmpty());

    if (className.isEmpty()) {
        m_baseClassNameInputField->clear();
        m_fileNameInputField->clear();
    }
}

void MainWindow::generateCodeButtonClicked()
{
    m_codeGenerator->setClassName(m_classNameInputField->text());

    if (m_codeGenerator->fileName().isEmpty()) {
        setError(tr("Invalid / Illegal / Empty file name"));
    } else {
        QString path(saveLocation());

        if (!path.isEmpty()) {
            m_codeGenerator->generate(path);
            m_namespaceNameInputField->clear();
            m_classNameInputField->clear();
            m_baseClassNameInputField->clear();
            m_fileNameInputField->clear();
        }
    }
}

void MainWindow::variableNameChanged(const QString& varName)
{
    bool ok(!varName.trimmed().isEmpty());

    if (ok) {
        if (VariableListModel::instance()->nameExists(varName)) {
            setError(tr("Duplicate name!"));
            ok = false;
        } else if (!varName.at(0).isLetter()) {
            setError(tr("Illegal name!"));
            ok = false;
        }
    }

    m_addVariableButton->setEnabled(ok);
}

void MainWindow::addVariableButtonClicked()
{
    Variable var{m_typeCombo->currentData().type(), m_varNameInputField->text(),
                 m_isPropertyChkBox->isChecked(),
                 m_addSetterChkBox->isChecked(), m_addGetterChkBox->isChecked()};

    if (var.isCustomType()) {
        bool ok;
        QString text = QInputDialog::getText(this, qApp->applicationName(),
                                             tr("Enter variable type:"), QLineEdit::Normal,
                                             "", &ok);

        if (!ok || text.isEmpty()) {
            return;
        }

        var.setTypeString(text);
        qDebug() << text;
    }

    VariableListModel::instance()->add(var);
    m_varNameInputField->clear();
    m_isPropertyChkBox->setChecked(false);
    m_addSetterChkBox->setChecked(false);
    m_addGetterChkBox->setChecked(false);
    m_addVariableButton->setEnabled(false);
    m_typeCombo->setCurrentIndex(0);
}

void MainWindow::qObjectCheckboxToggled(bool checked)
{
    m_codeGenerator->setIsQObject(checked);

    if (!checked) {
        m_variablesTableView->hideColumn(VariableListModel::RoleIsProperty);
    } else {
        m_variablesTableView->showColumn(VariableListModel::RoleIsProperty);
    }
}

void MainWindow::qPropertyCheckboxToggled(bool toggled)
{
    m_addGetterChkBox->setEnabled(!toggled);

    if (!m_addGetterChkBox->isChecked()) {
        m_addGetterChkBox->setChecked(toggled);
    }
}

void MainWindow::deleteSelectedVariable()
{
    VariableListModel::instance()->deleteSelected(m_variablesTableView->currentIndex());
    updateVariableEditButtons();
}

void MainWindow::moveVariableUp()
{
    auto selectedIdx(m_variablesTableView->currentIndex());

    if (selectedIdx.isValid()) {
        VariableListModel::instance()->moveUp(selectedIdx);
    }

    updateVariableEditButtons();
}

void MainWindow::moveVariableDown()
{
    auto selectedIdx(m_variablesTableView->currentIndex());

    if (selectedIdx.isValid()) {
        VariableListModel::instance()->moveDown(selectedIdx);
    }

    updateVariableEditButtons();
}

void MainWindow::updateVariableEditButtons()
{
    auto selectedIdx(m_variablesTableView->currentIndex());
    m_deleteVarButton->setEnabled(selectedIdx.isValid());
    m_moveUpVarButton->setEnabled(selectedIdx.row() >= 1);
    m_moveDownVarButton->setEnabled(selectedIdx.row() < VariableListModel::instance()->rowCount() - 1);
}

QString MainWindow::saveLocation()
{
    SettingsFile settings;
    QString dir(settings.saveLocation());

    if (dir.isEmpty()) {
        dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    }

    QString path(QFileDialog::getExistingDirectory(this, qApp->applicationName(), dir));
    return path;
}
