#ifndef USER_TABLE_WIDGET_H
#define USER_TABLE_WIDGET_H

#include "models/user_table/user_table_model.h"
#include "models/dashboard/dashboard_model.h"
#include "viewers/user_table/user_table_context_menu.h"
#include <QWidget>
#include <QTableView>
#include <QLineEdit>
#include <QSortFilterProxyModel>

class UserTableWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit UserTableWidget(UserTableModel *model, DashboardModel *dashboardModel, QWidget *parent = nullptr);

private slots:
    void handleFilterTextChanged(const QString &text);

public slots:
    void onAddUser();
    void onRemoveUser();

private:
    QTableView *m_tableView;
    QLineEdit *m_filterEdit;
    UserTableContextMenu *m_contextMenu;
    DashboardModel *m_dashboardModel;
    UserTableModel *m_userTableModel;
    QSortFilterProxyModel *m_proxyModel;
};

#endif // USER_TABLE_WIDGET_H
