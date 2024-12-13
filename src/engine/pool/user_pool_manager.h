#ifndef USER_POOL_MANAGER_H
#define USER_POOL_MANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QSet>
#include <QMutex>

enum class UserState
{
    Free,
    Waiting,
    Busy,
    Disconnected,
    Suspended
};

class UserPoolManager final : public QObject
{
    Q_OBJECT

public:
    explicit UserPoolManager(QObject *parent = nullptr);

    void addUser(const QString &username, UserState state = UserState::Free);
    void removeUser(const QString &username);
    void changeUserState(const QString &username, UserState newState);
    QSet<QString> getUsersByState(UserState state) const;
    UserState getUserState(const QString &username) const;

signals:
    void userStateChanged(const QString &username, UserState oldState, UserState newState);

private:
    QMap<UserState, QSet<QString>> statePools;
    mutable QMutex mutex;
    void removeUserFromState(const QString &username, UserState state);
};

#endif // USER_POOL_MANAGER_H
