#include "code_generator.h"
#include "variable_list_model.h"
#include "settings_file.h"

#include <QRegularExpression>
#include <QFile>
#include <QDebug>

#include <iostream>

namespace
{

void addFunctionBlockSeparator(QTextStream& stream)
{
    SettingsFile settings;

    if (settings.addFunctionBlockSeparator()) {
        QString str(98, '-');
        stream << "//" << str << endl;
    }
}

void appendGetterFunction(const Variable& variable, QTextStream& stream, const QString& tabStr)
{
    stream << endl << tabStr << "inline";
    stream << (variable.useConst() ? " const " : " ");
    stream << variable.typeString();
    stream << ((variable.useConst() && variable.useReference()) ? "& " : " ");

    stream << variable.name() << "() const" << endl
           << tabStr << "{" << endl
           << tabStr << tabStr << "return m_" << variable.name() << ";" << endl
           << tabStr << "}" << endl << endl;
}

void appendSetterDeclaration(const Variable& variable, QTextStream& stream, const QString& className = QString())
{
    stream << "void ";

    if (!className.isEmpty()) {
        stream << className << "::";
    }

    stream  << variable.setterString() << "(";

    if (variable.useConst()) {
        stream << "const ";
    }

    stream << variable.typeString();

    if (variable.useReference()) {
        stream << "&";
    }

    stream << " p_" << variable.name() << ")";
}

void appendSetterDefinition(const Variable& variable, QTextStream& stream, const QString& tabStr, bool isQObject, const QString& className)
{
    stream << endl;
    addFunctionBlockSeparator(stream);

    appendSetterDeclaration(variable, stream, className);
    stream << endl << "{" << endl
           << tabStr << variable.memberName() << " = p_" << variable.name() << ";" << endl;

    if (isQObject && variable.isProperty()) {
        stream << tabStr << "emit " << variable.name() << "Changed(" << variable.memberName() << ");" << endl;
    }

    stream << "}" << endl << endl;
}

}

CodeGenerator::CodeGenerator(QObject* prnt)
    : QObject(prnt),
      m_className(),
      m_fileName(),
      m_baseClassName(),
      m_isQObject(true),
      m_tabString(),
      m_generateCpp(true),
      m_generateFilename(true),
      m_addSuperTypedef(true),
      m_namespaceName()
{
}

const QString& CodeGenerator::className() const
{
    return m_className;
}

void CodeGenerator::setClassName(const QString& className)
{
    m_className = className;

    if (m_generateFilename) {
        createFileName();
    }
}

void CodeGenerator::generate(const QString& dir)
{
    SettingsFile settings;
    m_tabString = QString(settings.tabSize(), ' ');

    if (m_baseClassName.isEmpty() && m_isQObject) {
        m_baseClassName = "QObject";
    }

    QFile headerFile(dir + "/" + headerFileName());

    if (headerFile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream stream(&headerFile);
        stream << settings.formattedCopyrightContent(headerFileName());
        generateHeaderFileContent(stream);
        headerFile.close();
    }

    if (m_generateCpp) {
        QFile cppFile(dir + "/" + cppFileName());

        if (cppFile.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream stream(&cppFile);
            stream << settings.formattedCopyrightContent(cppFileName()) << endl;
            stream << "#include \"" << headerFileName() << "\"" << endl << endl;
            generateCppFileContent(stream);
            cppFile.close();
        }
    }

    VariableListModel::instance()->clear();
}

void CodeGenerator::setFileName(const QString& fileName)
{
    if (m_fileName == fileName) {
        return;
    }

    m_fileName = fileName;
    emit fileNameChanged(m_fileName);
}

void CodeGenerator::setIsQObject(bool isQObject)
{
    if (m_isQObject == isQObject) {
        return;
    }

    m_isQObject = isQObject;
    emit isQObjectChanged(m_isQObject);
}

void CodeGenerator::setBaseClassName(const QString& baseClassName)
{
    //qDebug() << baseClassName;
    if (m_baseClassName == baseClassName) {
        return;
    }

    m_baseClassName = baseClassName;
    emit baseClassNameChanged(m_baseClassName);
}

void CodeGenerator::setGenerateCpp(bool generateCpp)
{
    if (m_generateCpp == generateCpp) {
        return;
    }

    m_generateCpp = generateCpp;
    emit generateCppChanged(m_generateCpp);
}

void CodeGenerator::setNamespaceName(QString namespaceName)
{
    m_namespaceName = namespaceName;

    if (m_generateFilename) {
        createFileName();
    }
}

void CodeGenerator::generateHeaderFileContent(QTextStream& stream)
{
    const QString guardMacro(m_fileName.toUpper() + "_H");
    stream << QStringLiteral("#ifndef ") << guardMacro << endl;
    stream << QStringLiteral("#define ") << guardMacro << endl;
    stream << endl << endl;

    if (m_isQObject && m_baseClassName == QStringLiteral("QObject")) {
        stream << "#include <QObject>" << endl << endl;
    }

    if (!m_namespaceName.isEmpty()) {
        stream << "namespace " << m_namespaceName << endl << "{" << endl << endl;
    }

    stream << "class " << m_className;

    if (!m_baseClassName.isEmpty()) {
        stream << " : public " << m_baseClassName;
    }

    stream << endl << "{" << endl;
    appendQProperties(stream);
    stream << endl << "public:" << endl << endl;
    addCtrDtr(stream);
    appendSetterGetters(stream);
    appendSignals(stream);

    stream << "private:" << endl << endl;

    if (m_addSuperTypedef && !m_baseClassName.isEmpty()) {
        stream << m_tabString << "using Super = " << m_baseClassName << ";" << endl << endl;
    }

    appendMemberVariables(stream);
    stream << endl << "};" << endl << endl;

    if (!m_namespaceName.isEmpty()) {
        stream << "} // namespace " << m_namespaceName << endl << endl;
    }

    stream << QStringLiteral("#endif //") << guardMacro << endl;
}

void CodeGenerator::generateCppFileContent(QTextStream& stream)
{
    if (!m_namespaceName.isEmpty()) {
        stream << "namespace " << m_namespaceName << endl << "{" << endl << endl;
    }

    ::addFunctionBlockSeparator(stream);
    stream << m_className << "::" << m_className << "(";

    if (m_isQObject) {
        stream << "QObject* prnt";
    }

    stream << ")";
    appendCtrDefinition(stream);
    const auto& variables = VariableListModel::instance()->variables();

    for (const auto& variable : variables) {
        if (variable.addSetter()) {
            ::appendSetterDefinition(variable, stream, m_tabString, m_isQObject, m_className);
        }
    }

    if (!m_namespaceName.isEmpty()) {
        stream << endl << "} // namespace " << m_namespaceName << endl << endl;
    }
}

void CodeGenerator::addCtrDtr(QTextStream& stream)
{
    stream << m_tabString << "explicit " << m_className << "(";

    if (m_isQObject) {
        stream << "QObject* prnt=nullptr";
    }

    stream << ")";

    if (!m_generateCpp) {
        appendCtrDefinition(stream);
    } else {
        stream << ";" << endl;
    }

    stream << endl << m_tabString << "~" << m_className << "()";

    if (m_isQObject) {
        stream << " override = default";
    }

    stream << ";" << endl;
}

void CodeGenerator::appendCtrDefinition(QTextStream& stream)
{
    const auto& variables = VariableListModel::instance()->variables();

    if (!variables.isEmpty() || !m_baseClassName.isEmpty()) {
        stream << endl << m_tabString << " : ";
    }

    if (!m_baseClassName.isEmpty()) {
        stream << (m_addSuperTypedef ? "Super" : m_baseClassName) << "(";

        if (m_isQObject) {
            stream << "prnt";
        }

        stream << ")";
    }

    if (!variables.isEmpty()) {
        for (int i = 0; i < variables.size(); i++) {
            const auto variable = variables.at(i);

            if (i == 0 && m_baseClassName.isEmpty()) {
                stream << "m_" << variable.name() << "(";
            } else {
                stream << "," << endl << m_tabString << "   " << "m_" << variable.name() << "(";
            }

            if (variable.isPointerType()) {
                stream << "nullptr";
            }

            stream << ")";
        }
    }

    if (!m_generateCpp) {
        stream << endl << m_tabString << "{}" << endl;
    } else {
        stream << endl << "{" << endl << "}" << endl;
    }
}

void CodeGenerator::appendQProperties(QTextStream& stream)
{
    if (!m_isQObject) {
        return;
    }

    stream << m_tabString << "Q_OBJECT" << endl << endl;
    const auto& variables = VariableListModel::instance()->variables();

    for (const auto& variable : variables) {
        if (variable.isProperty()) {
            QString setter(variable.name());
            setter[0] = setter.at(0).toUpper();
            stream << m_tabString << "Q_PROPERTY(" << variable.typeString() << " " << variable.name()
                   << " READ " << variable.name();

            if (variable.addSetter()) {
                stream << " WRITE " << variable.setterString();
            }

            stream << " NOTIFY " << variable.name() << "Changed)" << endl;
        }
    }
}

void CodeGenerator::appendSetterGetters(QTextStream& stream)
{
    const auto& variables = VariableListModel::instance()->variables();

    if (!variables.isEmpty()) {
        for (const auto& variable : variables) {
            if (variable.addGetter()) {
                ::appendGetterFunction(variable, stream, m_tabString);
            }

            if (variable.addSetter()) {
                if (m_generateCpp) {
                    stream << endl << m_tabString;
                    ::appendSetterDeclaration(variable, stream);
                    stream << ";" << endl << endl;
                } else {
                    ::appendSetterDefinition(variable, stream, m_tabString, m_isQObject, QString());
                }
            }
        }

        stream << endl;
    }

}

void CodeGenerator::appendSignals(QTextStream& stream)
{
    if (!m_isQObject) {
        return;
    }

    bool add(false);
    const auto& variables = VariableListModel::instance()->variables();

    for (const auto& variable : variables) {
        if (variable.isProperty()) {
            add = true;
            break;
        }
    }

    if (!add) {
        return;
    }

    stream << "signals:" << endl << endl;

    for (const auto& variable : variables) {
        if (variable.isProperty()) {
            stream << m_tabString << "void " << variable.name() << "Changed(";

            if (variable.useConst()) {
                stream << "const " << variable.typeString();

                if (variable.useReference()) {
                    stream << "&";
                }
            } else {
                stream << variable.typeString();
            }

            stream << " p_" << variable.name() << ");" << endl << endl;
        }
    }

    stream << endl;
}

void CodeGenerator::appendMemberVariables(QTextStream& stream)
{
    const auto& variables = VariableListModel::instance()->variables();

    if (!variables.isEmpty()) {
        int maxSize(0);

        for (const auto& variable : variables) {
            maxSize = std::max(maxSize, variable.typeString().size());
        }

        SettingsFile settings;
        maxSize += settings.tabSize();

        for (const auto& variable : variables) {
            stream << m_tabString << variable.typeString().leftJustified(maxSize, ' ')
                   << "m_" << variable.name() << ";" << endl;
        }

        stream << endl;
    }
}

void CodeGenerator::createFileName()
{
    if (!m_className.isEmpty() && m_className.at(0).isLetter()) {
        static QRegularExpression regExp1 {"(.)([A-Z][a-z]+)"};
        static QRegularExpression regExp2 {"([a-z0-9])([A-Z])"};

        QString result;

        if (!m_namespaceName.isEmpty() && m_namespaceName.at(0).isLetter()) {
            result.append(m_namespaceName);
        }

        result.append(m_className);
        result.replace(regExp1, "\\1_\\2");
        result.replace(regExp2, "\\1_\\2");
        m_fileName = result.toLower();
        emit fileNameGenerated(m_fileName);
    }
}

QString CodeGenerator::headerFileName() const
{
    if (!m_fileName.isEmpty()) {
        return m_fileName + ".h";
    }

    return QString();
}

QString CodeGenerator::cppFileName() const
{
    if (!m_fileName.isEmpty()) {
        return m_fileName + ".cpp";
    }

    return QString();
}

bool CodeGenerator::addSuperTypedef() const
{
    return m_addSuperTypedef;
}

void CodeGenerator::setAddSuperTypedef(bool addSuperTypedef)
{
    m_addSuperTypedef = addSuperTypedef;
}

bool CodeGenerator::generateFilename() const
{
    return m_generateFilename;
}

void CodeGenerator::setGenerateFilename(bool generateFilename)
{
    m_generateFilename = generateFilename;

    if (m_generateFilename) {
        setClassName(m_className);
    }

    emit generateFilenameChanged(m_generateFilename);
}

const QString& CodeGenerator::fileName() const
{
    return m_fileName;
}
