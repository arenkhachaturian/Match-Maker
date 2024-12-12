#ifndef USER_TABLE_MODEL_H
#define USER_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include "engine/user/user_registry.h"

class UserTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit UserTableModel(UserRegistry* userRegistry, QObject* parent = nullptr);

    std::shared_ptr<const User> getUser(const QString& username) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool addUser(const User& user);
    bool removeUser(const QString& username);
    bool updateUserRating(const QString& username, const QString& game, int rating);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private slots:
    void handleUserChanged(const QString& username, int row, UserRegistry::ChangeType changeType);

private:
    UserRegistry* m_userRegistry; // Source of truth for user data
};

#endif // USER_TABLE_MODEL_H
