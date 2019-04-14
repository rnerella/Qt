#include "variable.h"

Variable::Variable(QVariant::Type t, const QString& nm, bool isQprop, bool setter, bool getter)
    : m_type(t),
      m_typeString(QVariant::typeToName(static_cast<int>(m_type))),
      m_name(nm),
      m_isProperty(isQprop),
      m_addSetter(setter),
      m_addGetter(getter)
{
}

QVariant::Type Variable::type() const
{
    return m_type;
}

void Variable::setType(const QVariant::Type& value)
{
    m_type = value;
}

const QString& Variable::name() const
{
    return m_name;
}

void Variable::setName(const QString& value)
{
    m_name = value;
}

bool Variable::isProperty() const
{
    return m_isProperty;
}

void Variable::setIsProperty(bool value)
{
    m_isProperty = value;
}

bool Variable::addSetter() const
{
    return m_addSetter;
}

void Variable::setAddSetter(bool value)
{
    m_addSetter = value;
}

bool Variable::addGetter() const
{
    return m_addGetter;
}

void Variable::setAddGetter(bool value)
{
    m_addGetter = value;
}

bool Variable::useConst() const
{
    return isCustomType() || typeString().startsWith('Q') || isPointerType();
}

bool Variable::useReference() const
{
    return (isCustomType() || typeString().startsWith('Q')) && !isPointerType();
}

bool Variable::isPointerType() const
{
    const QString str(typeString());
    return str.endsWith('*');
}

QString Variable::typeString() const
{
    return m_typeString;
}

void Variable::setTypeString(const QString& typeStr)
{
    m_typeString = typeStr;
}

bool Variable::isCustomType() const
{
    return m_type == static_cast<int>(QMetaType::User);
}

QString Variable::memberName() const
{
    return "m_" + m_name;
}

QString Variable::setterString() const
{
    if (m_name.isEmpty()) {
        return m_name;
    }

    QString setter(m_name);
    setter[0] = setter.at(0).toUpper();
    return "set" + setter;
}
