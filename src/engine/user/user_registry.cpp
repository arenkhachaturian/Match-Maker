#include "engine/user/user_registry.h"
#include <stdexcept>

UserRegistry::UserRegistry(QObject* parent) : QObject(parent) {}

bool UserRegistry::addUser(const User& user) {
    QMutexLocker locker(&mutex);  
    if (m_users.indexOf(user) >= 0) {
        return false;
    }
    m_users.append(user);
    emit userChanged(user.getUsername(), m_users.size() - 1, ChangeType::Created);
    return true;
}

bool UserRegistry::removeUser(const QString& username) {
    QMutexLocker locker(&mutex);  
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it < 0) {
        return false;
    }

    m_users.removeAt(it);
    emit userChanged(username, it, ChangeType::Removed);
    return true;
}

const User& UserRegistry::getUser(const QString& username) const {
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it >= 0) {
        return m_users[it];
    }
    throw std::runtime_error(std::string("User ") + username.toStdString() + "does not exists");
}

bool UserRegistry::isExistingUser(const QString& username) const{
    QMutexLocker locker(&mutex);
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it < 0) {
        return false;
    }
    return true;
}

QSet<QString> UserRegistry::getUserPreferredGames(const QString& username) const {
    QMutexLocker locker(&mutex); 
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it < 0) {
        return QSet<QString>{};
    }
    return m_users[it].getPreferredGames();
}

const QList<User>& UserRegistry::getAllUsers() const {
    return m_users;
}

bool UserRegistry::updateUserRating(const QString& username, const QString& game, int newRating) {
    QMutexLocker locker(&mutex);  
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it < 0) {
        return false;
    }

    m_users[it].updateRating(game, newRating);

    emit userChanged(username, it, ChangeType::Updated);
    return true;
}

int UserRegistry::getUserRating(const QString &username, const QString &gameName) {
    User temp(username);
    auto it = m_users.indexOf(temp);

    if (it < 0) {
        return 0;
    }

    return m_users[it].getRating(gameName);
}

void UserRegistry::incrementUserRating(const QString &username, const QString &gameName, int incr) {
    int current = getUserRating(username, gameName);
    updateUserRating(username, gameName, current + incr);
}
