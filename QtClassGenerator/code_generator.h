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

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <QObject>
#include <QTextStream>


class CodeGenerator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString className READ className WRITE setClassName NOTIFY classNameChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString baseClassName READ baseClassName WRITE setBaseClassName NOTIFY baseClassNameChanged)
    Q_PROPERTY(bool isQObject READ isQObject WRITE setIsQObject NOTIFY isQObjectChanged)
    Q_PROPERTY(bool generateCpp READ generateCpp WRITE setGenerateCpp NOTIFY generateCppChanged)
    Q_PROPERTY(QString namespaceName READ namespaceName WRITE setNamespaceName NOTIFY namespaceNameChanged)

public:

    explicit CodeGenerator(QObject* prnt = nullptr);

    const QString& className() const;

    const QString& fileName() const;

    bool isQObject() const
    {
        return m_isQObject;
    }

    QString baseClassName() const
    {
        return m_baseClassName;
    }

    bool generateCpp() const
    {
        return m_generateCpp;
    }

    bool generateFilename() const;

    void setGenerateFilename(bool generateFilename);

    bool addSuperTypedef() const;

    void setAddSuperTypedef(bool addSuperTypedef);

    const QString& namespaceName() const
    {
        return m_namespaceName;
    }

public slots:

    void generate(const QString& dir);

    void setClassName(const QString& className);

    void setFileName(const QString& fileName);

    void setIsQObject(bool isQObject);

    void setBaseClassName(const QString& baseClassName);

    void setGenerateCpp(bool generateCpp);

    void setNamespaceName(QString namespaceName);

signals:

    void classNameChanged(const QString& className);

    void fileNameGenerated(const QString& fileName);

    void fileNameChanged(const QString& fileName);

    void isQObjectChanged(bool isQObject);

    void baseClassNameChanged(const QString& baseClassName);

    void generateCppChanged(bool generateCpp);

    void generateFilenameChanged(bool generate);

    void namespaceNameChanged(QString namespaceName);

private:

    void generateHeaderFileContent(QTextStream& stream);

    void generateCppFileContent(QTextStream& stream);

    void appendIncludesAndForwardDeclares(QTextStream& stream);

    void addCtrDtr(QTextStream& stream);

    void appendCtrDefinition(QTextStream& stream);

    void appendQProperties(QTextStream& stream);

    void appendSetterGetters(QTextStream& stream);

    void appendSignals(QTextStream& stream);

    void appendMemberVariables(QTextStream& stream);

    void createFileName();

    QString headerFileName() const;

    QString cppFileName() const;

    QString     m_className;
    QString     m_fileName;
    QString     m_baseClassName;
    bool        m_isQObject;
    QString     m_tabString;
    bool        m_generateCpp;
    bool        m_generateFilename;
    bool        m_addSuperTypedef;
    QString     m_namespaceName;
};

#endif // CODE_GENERATOR_H
