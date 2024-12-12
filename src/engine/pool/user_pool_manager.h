#ifndef USER_POOL_MANAGER_H
#define USER_POOL_MANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QSet>
#include <QMutex>

// Enum for user states
enum class UserState {
    Free,
    Waiting,
    Busy,
    Disconnected,
    Suspended
};

class UserPoolManager : public QObject {
    Q_OBJECT

public:
    explicit UserPoolManager(QObject* parent = nullptr);

    // Add a user with a specific state (default is Free)
    void addUser(const QString& username, UserState state = UserState::Free);

    // Remove a user entirely
    void removeUser(const QString& username);

    // Change a user's state
    void changeUserState(const QString& username, UserState newState);

    // Get all users in a specific state
    QSet<QString> getUsersByState(UserState state) const;

    // Get the state of a specific user
    UserState getUserState(const QString& username) const;

signals:
    void userStateChanged(const QString& username, UserState oldState, UserState newState);

private:
    // Map of user states to sets of usernames
    QMap<UserState, QSet<QString>> statePools;

    // Mutex for thread safety
    mutable QMutex mutex;

    // Helper to find and remove a user from a specific state
    void removeUserFromState(const QString& username, UserState state);
};

#endif // USER_POOL_MANAGER_H
