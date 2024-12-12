#include "user_table_context_menu.h"
#include <QWidget>
#include <QTableView>
#include <QHeaderView>

UserTableContextMenu::UserTableContextMenu(QWidget* parent) : QObject(parent) {
    // Initialize context menu and actions
    m_contextMenu = new QMenu(parent);
    m_addUserAction = new QAction("Add User", this);
    m_removeUserAction = new QAction("Remove User", this);

    // Add actions to the menu
    m_contextMenu->addAction(m_addUserAction);
    m_contextMenu->addAction(m_removeUserAction);

    if (auto tableView = qobject_cast<QTableView*>(parent)) {
        tableView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tableView, &QTableView::customContextMenuRequested, this, &UserTableContextMenu::showContextMenu);

        // Also attach to the header
        auto header = tableView->horizontalHeader();
        header->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(header, &QHeaderView::customContextMenuRequested, this, &UserTableContextMenu::showContextMenu);
    } else {
        qDebug() << "smth wrong";
    }

    // Connect actions to respective slots
    connect(m_addUserAction, &QAction::triggered, this, &UserTableContextMenu::handleAddUser);
    connect(m_removeUserAction, &QAction::triggered, this, &UserTableContextMenu::handleRemoveUser);
}

void UserTableContextMenu::showContextMenu(const QPoint& pos) {
    // Show the context menu at the cursor position
    m_contextMenu->exec(QCursor::pos());
}

void UserTableContextMenu::handleAddUser() {
    emit addUserRequested();
}

void UserTableContextMenu::handleRemoveUser() {
    emit removeUserRequested();
}
