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

#include "variable_list_model.h"

#include <QDebug>

VariableListModel::VariableListModel(QObject* prnt)
    : QAbstractTableModel(prnt),
      m_variables()
{
}

int VariableListModel::rowCount(const QModelIndex& idx) const
{
    return idx.isValid() ? 0 : m_variables.size();
}

int VariableListModel::columnCount(const QModelIndex& idx) const
{
    return idx.isValid() ? 0 : RoleMax;
}

QVariant VariableListModel::data(const QModelIndex& idx, int role) const
{
    int row(idx.row()), column(idx.column());

    if (!idx.isValid() || row >= m_variables.size() || (role != Qt::DisplayRole && role != Qt::TextAlignmentRole)) {
        return QVariant();
    }

    const Variable& var = m_variables.at(row);

    if (role == Qt::TextAlignmentRole) {
        int a = Qt::AlignVCenter;

        if (column != RoleType && column != RoleName) {
            a |= Qt::AlignHCenter;
        }

        return a;
    }

    switch (column) {
        case RoleType:
            return var.typeString();

        case RoleName:
            return var.name();

        case RoleIsProperty:
            return var.isProperty() ? QStringLiteral("Yes") : QStringLiteral("No");

        case RoleAddSetter:
            return var.addSetter() ? QStringLiteral("Yes") : QStringLiteral("No");

        case RoleAddGetter:
            return var.addGetter() ? QStringLiteral("Yes") : QStringLiteral("No");

        default:
            break;
    }

    return QVariant();
}

QVariant VariableListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < RoleMax && orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case RoleType:
                return QStringLiteral("Type");

            case RoleName:
                return QStringLiteral("Name");

            case RoleIsProperty:
                return QStringLiteral("Is QProperty");

            case RoleAddSetter:
                return QStringLiteral("Add Setter");

            case RoleAddGetter:
                return QStringLiteral("Add Getter");
        }
    }

    return QVariant();
}

bool VariableListModel::nameExists(const QString& name)
{
    if (!name.isEmpty() && !m_variables.isEmpty()) {
        for (const Variable& v : m_variables) {
            if (v.name().compare(name, Qt::CaseInsensitive) == 0) {
                return true;
            }
        }
    }

    return false;
}

void VariableListModel::add(const Variable& v)
{
    beginInsertRows(QModelIndex(), m_variables.size(), m_variables.size());
    m_variables << v;
    endInsertRows();
}

const QVector<Variable>& VariableListModel::variables() const
{
    return m_variables;
}

void VariableListModel::deleteSelected(const QModelIndex& idx)
{
    if (!idx.isValid()) {
        return;
    }

    int row(idx.row());
    beginRemoveRows(QModelIndex(), row, row);
    m_variables.removeAt(row);
    endRemoveRows();
}

void VariableListModel::moveUp(const QModelIndex& idx)
{
    if (idx.isValid() && idx.row() >= 1) {
        int row(idx.row());
        QModelIndex destination(createIndex(row - 1, 0, idx.internalPointer()));
        beginMoveRows(idx, row, row, destination, row - 1);
        m_variables.move(row, row - 1);
        endMoveRows();
    }
}

void VariableListModel::moveDown(const QModelIndex& idx)
{
    if (idx.isValid()) {
        int row(idx.row());

        if (row < rowCount() - 1) {
            QModelIndex destination(createIndex(row + 1, 0, idx.internalPointer()));
            beginMoveRows(idx, row, row, destination, rowCount());
            m_variables.move(row, row + 1);
            endMoveRows();
        }
    }

}

void VariableListModel::clear()
{
    beginResetModel();
    m_variables.clear();
    endResetModel();
}

VariableListModel::~VariableListModel()
{
    qDebug() << "Model destroyed!";
}



