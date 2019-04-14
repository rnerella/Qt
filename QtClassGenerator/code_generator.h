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

public:

    explicit CodeGenerator(QObject* prnt=nullptr);

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

public slots:

    void generate();

    void setClassName(const QString& className);

    void setFileName(const QString &fileName);

    void setIsQObject(bool isQObject);

    void setBaseClassName(const QString& baseClassName);

    void setGenerateCpp(bool generateCpp);

signals:

    void classNameChanged(const QString& className);

    void fileNameChanged(const QString& fileName);

    void isQObjectChanged(bool isQObject);

    void baseClassNameChanged(const QString& baseClassName);

    void generateCppChanged(bool generateCpp);

private:

    void generateHeaderFileContent();

    void generateCppFileContent();

    void addCtrDtr(QTextStream& stream);

    void appendCtrDefinition(QTextStream& stream);

    void appendQProperties(QTextStream& stream);

    void appendSetterGetters(QTextStream& stream);

    QString     m_className;
    QString     m_fileName;
    QString     m_baseClassName;
    bool        m_isQObject;
    QString     m_tabString;
    bool        m_generateCpp;
};

#endif // CODE_GENERATOR_H
