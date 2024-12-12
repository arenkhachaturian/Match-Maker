#include "add_user_dialog.h"
#include "models/user_table/user_table_model.h"
#include "models/dashboard/dashboard_model.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>

AddUserDialog::AddUserDialog(const UserTableModel* userModel, const DashboardModel* dashboardModel, QWidget* parent)
    : QDialog(parent), m_userModel(userModel), m_dashboardModel(dashboardModel) {
    setWindowTitle("Add User");

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Username:"));
    m_usernameEdit = new QLineEdit(this);
    layout->addWidget(m_usernameEdit);

    layout->addWidget(new QLabel("First Name:"));
    m_firstNameEdit = new QLineEdit(this);
    layout->addWidget(m_firstNameEdit);

    layout->addWidget(new QLabel("Last Name:"));
    m_lastNameEdit = new QLineEdit(this);
    layout->addWidget(m_lastNameEdit);

    layout->addWidget(new QLabel("Preferred Games:"));
    m_gameListWidget = new QListWidget(this);
    for (const QString& game : dashboardModel->getAllGameNames()) {
        QListWidgetItem* item = new QListWidgetItem(game, m_gameListWidget);
        item->setCheckState(Qt::Unchecked);
    }
    layout->addWidget(m_gameListWidget);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AddUserDialog::validateInputs);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(m_buttonBox);
}


QString AddUserDialog::getUsername() const {
    return m_usernameEdit->text();
}

QString AddUserDialog::getFirstName() const {
    return m_firstNameEdit->text();
}

QString AddUserDialog::getLastName() const {
    return m_lastNameEdit->text();
}

QSet<QString> AddUserDialog::getPreferredGames() const {
    QSet<QString> preferredGames;
    for (int i = 0; i < m_gameListWidget->count(); ++i) {
        QListWidgetItem* item = m_gameListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            preferredGames.insert(item->text());
        }
    }
    return preferredGames;
}

void AddUserDialog::validateInputs() {
    QString username = m_usernameEdit->text();
    QString firstName = m_firstNameEdit->text();
    QString lastName = m_lastNameEdit->text();

    // Check if username already exists
    if (m_userModel->getUser(username) != nullptr) { // User already exists
        QMessageBox::warning(this, "Invalid Username", "Username already exists.");
        return;
    }

    if (!validateUsername(username)) {
        QMessageBox::warning(this, "Invalid Username", "Username must be ASCII, non-empty, and at most 16 characters long.");
        return;
    }

    if (!validateFirstName(firstName)) {
        QMessageBox::warning(this, "Invalid First Name", "First Name must start with a capital letter and contain only Latin characters.");
        return;
    }

    if (!validateLastName(lastName)) {
        QMessageBox::warning(this, "Invalid Last Name", "Last Name must contain only Latin characters or a single quote.");
        return;
    }

    accept(); // All inputs are valid
}


bool AddUserDialog::validateUsername(const QString& username) const {
    if (username.isEmpty() || username.length() > 16) {
        return false; // Invalid due to empty or length constraint
    }

    // Check if username already exists
    if (m_userModel->getUser(username) != nullptr) {
        return false;
    }

    // Ensure all characters are ASCII printable
    for (QChar c : username) {
        if (!c.isPrint()) {
            return false;
        }
    }

    return true;
}

// Validate First Name
bool AddUserDialog::validateFirstName(const QString& firstName) const {
    QRegularExpression regex("^[A-Z][a-z]*$"); // only latin chars
    return regex.match(firstName).hasMatch();
}

// Validate Last Name
bool AddUserDialog::validateLastName(const QString& lastName) const {
    QRegularExpression regex("^[A-Za-z]+(?:'[A-Za-z]+)?$"); // starts with latin, can contain '
    return regex.match(lastName).hasMatch();
}
