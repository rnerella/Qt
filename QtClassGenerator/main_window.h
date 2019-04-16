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
