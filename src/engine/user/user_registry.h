#ifndef USER_REGISTRY_H
#define USER_REGISTRY_H

#include "engine/user/user.h"
#include <QObject>
#include <QString>
#include <QSet>
#include <memory>
#include <QMutex>
#include <QVariant>

class UserRegistry final : public QObject
{
    Q_OBJECT

public:
    explicit UserRegistry(QObject *parent = nullptr);

    bool addUser(const User &user);
    bool removeUser(const QString &username);
    const User &getUser(const QString &username) const;
    bool isExistingUser(const QString &username) const;
    QSet<QString> getUserPreferredGames(const QString &username) const;
    const QList<User> &getAllUsers() const;
    bool updateUserRating(const QString &username, const QString &game, int newRating);
    int getUserRating(const QString &username, const QString &gameName);
    void incrementUserRating(const QString &username, const QString &gameName, int incr);
    enum class ChangeType
    {
        Created,
        Updated,
        Removed
    };
    Q_ENUM(ChangeType)

signals:
    void userChanged(const QString &username, int row, ChangeType type);

private:
    mutable QMutex mutex;
    QList<User> m_users;
};

#endif // USER_REGISTRY_H
