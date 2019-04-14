#ifndef VARIABLE_H
#define VARIABLE_H

#include <QVariant>

class Variable
{

public:

    Variable()
        : Variable(QVariant::Type::Invalid, QString(), false, false, false)
    {}

    Variable(QVariant::Type t, const QString& nm, bool isQprop, bool setter, bool getter);

    QString memberName() const;

    QString setterString() const;

    QVariant::Type type() const;

    void setType(const QVariant::Type& value);

    const QString& name() const;

    void setName(const QString& value);

    bool isProperty() const;

    void setIsProperty(bool value);

    bool addSetter() const;

    void setAddSetter(bool value);

    bool addGetter() const;

    void setAddGetter(bool value);

    bool useConst() const;

    bool useReference() const;

    bool isPointerType() const;

    QString typeString() const;

    void setTypeString(const QString& typeStr);

    bool isCustomType() const;

private:

    QVariant::Type      m_type;
    QString             m_typeString;
    QString             m_name;
    bool                m_isProperty;
    bool                m_addSetter;
    bool                m_addGetter;
};

#endif // VARIABLE_H
