#include "remove_user_dialog.h"
#include "models/user_table_model.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QMessageBox>

RemoveUserDialog::RemoveUserDialog(UserTableModel* userModel, QWidget* parent)
    : QDialog(parent), m_userModel(userModel) {
    setWindowTitle("Remove User");

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Search Input
    m_searchLineEdit = new QLineEdit(this);
    m_searchLineEdit->setPlaceholderText("Search username...");
    connect(m_searchLineEdit, &QLineEdit::textChanged, this, &RemoveUserDialog::filterUserList);
    layout->addWidget(m_searchLineEdit);

    // User List
    m_userListWidget = new QListWidget(this);
    filterUserList(""); // Populate the initial list
    layout->addWidget(m_userListWidget);

    // Button Box
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &RemoveUserDialog::validateSelection);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(m_buttonBox);
}

QString RemoveUserDialog::getSelectedUser() const {
    QListWidgetItem* selectedItem = m_userListWidget->currentItem();
    return selectedItem ? selectedItem->text() : QString();
}

void RemoveUserDialog::filterUserList(const QString& searchText) {
    m_userListWidget->clear();
    for (int row = 0; row < m_userModel->rowCount(); ++row) {
        QString username = m_userModel->data(m_userModel->index(row, 0)).toString();
        if (username.contains(searchText, Qt::CaseInsensitive)) {
            m_userListWidget->addItem(username);
        }
    }
}

void RemoveUserDialog::validateSelection() {
    QString selectedUser = getSelectedUser();
    if (selectedUser.isEmpty()) {
        QMessageBox::warning(this, "No User Selected", "Please select a user to remove.");
        return;
    }
    accept();
}
