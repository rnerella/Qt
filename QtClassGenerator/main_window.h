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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class CodeGenerator;
class QLabel;
class QTimer;
class QTableView;
class QComboBox;
class QLineEdit;
class QPushButton;
class QCheckBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

private:

    void setError(const QString& err);

    void namespaceNameChanged(const QString& namespaceName);

    void classNameChanged(const QString& className);

    void generateCodeButtonClicked();

    void variableNameChanged(const QString& varName);

    void addVariableButtonClicked();

    void qObjectCheckboxToggled(bool checked);

    void qPropertyCheckboxToggled(bool toggled);

    void deleteSelectedVariable();

    void moveVariableUp();

    void moveVariableDown();

    void updateVariableEditButtons();

    QString saveLocation();

    QLabel*             m_errorLabel;
    QTimer*             m_errorClearTimer;
    CodeGenerator*      m_codeGenerator;
    QLineEdit*          m_namespaceNameInputField;
    QLineEdit*          m_classNameInputField;
    QLineEdit*          m_baseClassNameInputField;
    QLineEdit*          m_fileNameInputField;
    QCheckBox*          m_qObjectChkBox;
    QPushButton*        m_generateCodeButton;
    QTableView*         m_variablesTableView;
    QComboBox*          m_typeCombo;
    QLineEdit*          m_varNameInputField;
    QCheckBox*          m_isPropertyChkBox;
    QCheckBox*          m_addSetterChkBox;
    QCheckBox*          m_addGetterChkBox;
    QPushButton*        m_addVariableButton;
    QPushButton*        m_deleteVarButton;
    QPushButton*        m_moveUpVarButton;
    QPushButton*        m_moveDownVarButton;

};

#endif // MAIN_WINDOW_H
