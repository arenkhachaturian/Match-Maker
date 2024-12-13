#include "user_table_context_menu.h"
#include <QWidget>
#include <QTableView>
#include <QHeaderView>

UserTableContextMenu::UserTableContextMenu(QWidget *parent) : QObject(parent)
{
    m_contextMenu = new QMenu(parent);
    m_addUserAction = new QAction("Add User", this);
    m_removeUserAction = new QAction("Remove User", this);

    m_contextMenu->addAction(m_addUserAction);
    m_contextMenu->addAction(m_removeUserAction);

    if (auto tableView = qobject_cast<QTableView *>(parent))
    {
        tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tableView, &QTableView::customContextMenuRequested, this, &UserTableContextMenu::showContextMenu);

        // view context menu even if clicked on header
        auto header = tableView->horizontalHeader();
        header->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(header, &QHeaderView::customContextMenuRequested, this, &UserTableContextMenu::showContextMenu);
    }
    else
    {
        qDebug() << "smth wrong";
    }

    connect(m_addUserAction, &QAction::triggered, this, &UserTableContextMenu::handleAddUser);
    connect(m_removeUserAction, &QAction::triggered, this, &UserTableContextMenu::handleRemoveUser);
}

void UserTableContextMenu::showContextMenu(const QPoint &pos)
{
    m_contextMenu->exec(QCursor::pos());
}

void UserTableContextMenu::handleAddUser()
{
    emit addUserRequested();
}

void UserTableContextMenu::handleRemoveUser()
{
    emit removeUserRequested();
}
