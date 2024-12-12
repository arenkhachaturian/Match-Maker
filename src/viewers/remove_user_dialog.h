#ifndef REMOVE_USER_DIALOG_H
#define REMOVE_USER_DIALOG_H

#include <QDialog>

class QLineEdit;
class QListWidget;
class QDialogButtonBox;
class UserTableModel;

class RemoveUserDialog : public QDialog {
    Q_OBJECT

public:
    explicit RemoveUserDialog(UserTableModel* userModel, QWidget* parent = nullptr);

    QString getSelectedUser() const;

private slots:
    void filterUserList(const QString& searchText);
    void validateSelection();

private:
    QLineEdit* m_searchLineEdit;
    QListWidget* m_userListWidget;
    QDialogButtonBox* m_buttonBox;
    UserTableModel* m_userModel;
};

#endif // REMOVE_USER_DIALOG_H
