#include "viewers/user_table/user_table_widget.h"
#include "models/dashboard/dashboard_model.h"
#include "user_table_context_menu.h"
#include "viewers/dialogs/add_user_dialog.h"
#include "viewers/dialogs/remove_user_dialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

UserTableWidget::UserTableWidget(UserTableModel *model, DashboardModel *dashboardModel, QWidget *parent)
    : m_userTableModel(model), m_dashboardModel(dashboardModel), QWidget(parent)
{

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(model);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setFilterKeyColumn(0);

    m_tableView = new QTableView(this);
    m_tableView->setModel(m_proxyModel);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSortingEnabled(true);
    m_tableView->horizontalHeader()->setSectionsClickable(true);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_contextMenu = new UserTableContextMenu(m_tableView);
    connect(m_contextMenu, &UserTableContextMenu::addUserRequested, this, &UserTableWidget::onAddUser);
    connect(m_contextMenu, &UserTableContextMenu::removeUserRequested, this, &UserTableWidget::onRemoveUser);

    m_filterEdit = new QLineEdit(this);
    m_filterEdit->setPlaceholderText("Filter by username...");
    connect(m_filterEdit, &QLineEdit::textChanged, this, &UserTableWidget::handleFilterTextChanged);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_tableView);
    layout->addWidget(m_filterEdit);
    setLayout(layout);
}

void UserTableWidget::handleFilterTextChanged(const QString &text)
{
    m_proxyModel->setFilterRegularExpression(text);
}

void UserTableWidget::onAddUser()
{
    AddUserDialog dialog(m_userTableModel, m_dashboardModel, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        // Create a new user, need to refactor
        User newUser(dialog.getUsername(), dialog.getFirstName(), dialog.getLastName(), dialog.getPreferredGames(), {});
        if (!m_userTableModel->addUser(newUser))
        {
            QMessageBox::warning(this, "Add User Failed", "Failed to add user with specified parameters.");
        }
    }
}

void UserTableWidget::onRemoveUser()
{
    RemoveUserDialog dialog(m_userTableModel, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString username = dialog.getSelectedUser();
        if (!m_userTableModel->removeUser(username))
        {
            QMessageBox::warning(this, "Remove User Failed", "Failed to remove the selected user.");
        }
    }
}
