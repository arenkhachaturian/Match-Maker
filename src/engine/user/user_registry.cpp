#include "engine/user/user_registry.h"

UserRegistry::UserRegistry(QObject* parent) : QObject(parent) {}

bool UserRegistry::addUser(const User& user) {
    QMutexLocker locker(&mutex); // Lock for thread safety
    if (m_users.indexOf(user) >= 0) {
        return false;
    }
    m_users.append(user);
    emit userChanged(user.getUsername(), m_users.size() - 1, ChangeType::Created);
    return true;
}

bool UserRegistry::removeUser(const QString& username) {
    QMutexLocker locker(&mutex); // Lock for thread safety
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it < 0) {
        return false;
    }

    m_users.removeAt(it);
    emit userChanged(username, it, ChangeType::Removed);
    return true;
}

std::shared_ptr<const User> UserRegistry::getUser(const QString& username) const {
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it >= 0) {
        return std::make_shared<const User>(m_users[it]);
    }
    return nullptr;
}

const QList<User>& UserRegistry::getAllUsers() const {
    return m_users;
}

bool UserRegistry::updateUserRating(const QString& username, const QString& game, int newRating) {
    QMutexLocker locker(&mutex); // Lock for thread safety
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it < 0) {
        return false;
    }

    m_users[it].updateRating(game, newRating);

    emit userChanged(username,  it, ChangeType::Updated);
    return true;
}
