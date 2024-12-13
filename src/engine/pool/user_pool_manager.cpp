#include "user_pool_manager.h"
#include <QDebug>

UserPoolManager::UserPoolManager(QObject *parent)
    : QObject(parent) {}

void UserPoolManager::addUser(const QString &username, UserState state)
{
    QMutexLocker locker(&mutex);

    if (statePools[state].contains(username))
    {
        qWarning() << "User" << username << "already exists in state" << static_cast<int>(state);
        return;
    }

    statePools[state].insert(username);
    qDebug() << "User" << username << "added to pool " << static_cast<int>(state);
    emit userStateChanged(username, UserState::Free, state);
}

void UserPoolManager::removeUser(const QString &username)
{
    QMutexLocker locker(&mutex);

    for (auto state = statePools.begin(); state != statePools.end(); ++state)
    {
        if (state.value().remove(username))
        {
            emit userStateChanged(username, state.key(), UserState::Disconnected);
            return;
        }
    }

    qWarning() << "User" << username << "does not exist in any state. Cannot remove.";
}

void UserPoolManager::changeUserState(const QString &username, UserState newState)
{
    QMutexLocker locker(&mutex);

    for (auto state = statePools.begin(); state != statePools.end(); ++state)
    {
        if (state.value().contains(username)) {
            state.value().remove(username);
            statePools[newState].insert(username);
            emit userStateChanged(username, state.key(), newState);
            return;
        }
    }

    qWarning() << "User" << username << "does not exist in any state. Cannot change state.";
}

void UserPoolManager::removeUserFromState(const QString &username, UserState state)
{
    if (!statePools[state].remove(username))
    {
        qWarning() << "User" << username << "not found in state" << static_cast<int>(state);
    }
}

QSet<QString> UserPoolManager::getUsersByState(UserState state) const
{
    QMutexLocker locker(&mutex);
    return statePools.value(state, QSet<QString>());
}

UserState UserPoolManager::getUserState(const QString &username) const
{
    QMutexLocker locker(&mutex);

    for (auto state = statePools.begin(); state != statePools.end(); ++state)
    {
        if (state.value().contains(username))
        {
            return state.key();
        }
    }

    qWarning() << "User" << username << "does not exist in any state.";
    throw std::runtime_error("User does not exist.");
}
