/*=================================================================================
// File Name: variable_list_model.h
// Author   : Ramesh Nerella
// Created  : 03.04.2019 23:48:57s
//=================================================================================*/


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
        static VariableListModel* instance = new VariableListModel();
        return instance;
    }


    ~VariableListModel() override;

    int rowCount(const QModelIndex& idx=QModelIndex()) const override;

    int columnCount(const QModelIndex& idx) const override;

    QVariant data(const QModelIndex& idx, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool nameExists(const QString& name);

    const QVector<Variable>& variables() const;

    void deleteSelected(const QModelIndex& idx);

    void moveUp(const QModelIndex& idx);

    void moveDown(const QModelIndex& idx);

protected:

    void add(const Variable& v);

private:

    explicit VariableListModel(QObject* prnt = nullptr);

    QVector<Variable>   m_variables;

};

#endif //QTCLASSGENERATOR_VARIABLE_LIST_MODEL_H
