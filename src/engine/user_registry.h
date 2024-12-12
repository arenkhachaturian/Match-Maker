#ifndef USER_REGISTRY_H
#define USER_REGISTRY_H

#include <QObject>
#include <QString>
#include <QSet>
#include <memory>
#include <QMutex>
#include <QVariant>
#include "engine/user.h"

class UserRegistry : public QObject {
    Q_OBJECT

public:
    explicit UserRegistry(QObject* parent = nullptr);

    bool addUser(const User& user);
    bool removeUser(const QString& username);
    std::shared_ptr<const User> getUser(const QString& username) const;
    const QList<User>& getAllUsers() const;
    bool updateUserRating(const QString& username, const QString& game, int newRating);

    enum class ChangeType { Created, Updated, Removed };
    Q_ENUM(ChangeType)

signals:
    void userChanged(const QString& username, int row, ChangeType type);

private:
    mutable QMutex mutex; // Protect access to m_users
    QList<User> m_users;
};

#endif // USER_REGISTRY_H
