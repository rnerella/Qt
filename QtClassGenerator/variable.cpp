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
