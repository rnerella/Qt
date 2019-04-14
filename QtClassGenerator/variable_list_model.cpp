/*=================================================================================
// File Name: variable_list_model.cpp
// Author   : Ramesh Nerella
// Created  : 03.04.2019 23:48:57s
//=================================================================================*/


#include "variable_list_model.h"

#include <QDebug>

//--------------------------------------------------------------------------------------------------
VariableListModel::VariableListModel(QObject* prnt)
    : QAbstractTableModel(prnt),
      m_variables()
{
//    qDebug() << "Model created!";
}

int VariableListModel::rowCount(const QModelIndex& idx) const
{
    Q_UNUSED(idx)
    return m_variables.size();
}

int VariableListModel::columnCount(const QModelIndex& idx) const
{
    Q_UNUSED(idx)
    return RoleMax;
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

const QVector<Variable> &VariableListModel::variables() const
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

//--------------------------------------------------------------------------------------------------
VariableListModel::~VariableListModel()
{
    qDebug() << "Model destroyed!";
}



