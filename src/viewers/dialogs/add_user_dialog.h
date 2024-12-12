#ifndef ADD_USER_DIALOG_H
#define ADD_USER_DIALOG_H

#include <QDialog>
#include <QString>
#include <QSet>
#include "models/user_table/user_table_model.h"
#include "models/dashboard/dashboard_model.h"

class QLineEdit;
class QListWidget;
class QDialogButtonBox;

class AddUserDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddUserDialog(const UserTableModel* userModel, const DashboardModel* dashboardModel, QWidget* parent = nullptr);

    QString getUsername() const;
    QString getFirstName() const;
    QString getLastName() const;
    QSet<QString> getPreferredGames() const;

private slots:
    void validateInputs();

private:
    QLineEdit* m_usernameEdit;
    QLineEdit* m_firstNameEdit;
    QLineEdit* m_lastNameEdit;
    QListWidget* m_gameListWidget;
    QDialogButtonBox* m_buttonBox;


    const UserTableModel* m_userModel; // Reference to the UserTableModel for validation
    const DashboardModel* m_dashboardModel;

    // thes can be free util functions
    bool validateUsername(const QString& username) const;
    bool validateFirstName(const QString& firstName) const;
    bool validateLastName(const QString& lastName) const;
};

#endif // ADD_USER_DIALOG_H
