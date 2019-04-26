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
