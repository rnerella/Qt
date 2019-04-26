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

#ifndef QTCLASSGENERATOR_VARIABLE_LIST_MODEL_H
#define QTCLASSGENERATOR_VARIABLE_LIST_MODEL_H

#include "variable.h"

#include <QAbstractTableModel>

class VariableListModel  : public QAbstractTableModel
{
    Q_OBJECT

    friend class MainWindow;

public:

    enum Role {
        RoleType,
        RoleName,
        RoleIsProperty,
        RoleAddSetter,
        RoleAddGetter,
        RoleMax
    };

    static VariableListModel* instance()
    {
        static auto* instance = new VariableListModel();
        return instance;
    }


    ~VariableListModel() override;

    int rowCount(const QModelIndex& idx = QModelIndex()) const override;

    int columnCount(const QModelIndex& idx) const override;

    QVariant data(const QModelIndex& idx, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool nameExists(const QString& name);

    const QVector<Variable>& variables() const;

    void deleteSelected(const QModelIndex& idx);

    void moveUp(const QModelIndex& idx);

    void moveDown(const QModelIndex& idx);

    void clear();

protected:

    void add(const Variable& v);

private:

    explicit VariableListModel(QObject* prnt = nullptr);

    QVector<Variable>   m_variables;

};

#endif //QTCLASSGENERATOR_VARIABLE_LIST_MODEL_H
