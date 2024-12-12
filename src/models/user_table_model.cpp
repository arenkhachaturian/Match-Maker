#include "models/user_table_model.h"
#include <QVariant>

UserTableModel::UserTableModel(UserRegistry* userRegistry, QObject* parent)
    : QAbstractTableModel(parent), m_userRegistry(userRegistry) {
    connect(m_userRegistry, &UserRegistry::userChanged, this, &UserTableModel::handleUserChanged);
}

std::shared_ptr<const User> UserTableModel::getUser(const QString& username) const {
    return m_userRegistry->getUser(username);
}

int UserTableModel::rowCount(const QModelIndex& /*parent*/) const {
    return m_userRegistry->getAllUsers().size();
}

int UserTableModel::columnCount(const QModelIndex& /*parent*/) const {
    return 4; // Username, First Name, Last Name, Preferred Games
}

QVariant UserTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    // Get user at the specified row
    const int row = index.row();
    const auto allUsers = m_userRegistry->getAllUsers();
    if (row < 0 || row >= allUsers.size()) {
        return QVariant();
    }

    const User& user = allUsers.at(row);

    switch (index.column()) {
    case 0: return user.getUsername();
    case 1: return user.getFirstName();
    case 2: return user.getLastName();
    case 3: return QList(user.getPreferredGames().begin(), user.getPreferredGames().end()).join(", ");
    }

    return QVariant();
}

QVariant UserTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Username";
        case 1: return "First Name";
        case 2: return "Last Name";
        case 3: return "Preferred Games";
        default: return QVariant();
        }
    } else if (orientation == Qt::Vertical) {
        return section + 1; // Row numbers
    }

    return QVariant();
}


bool UserTableModel::addUser(const User& user) {
    return m_userRegistry->addUser(user); // Signal will trigger update
}

bool UserTableModel::removeUser(const QString& username) {
    return m_userRegistry->removeUser(username); // Signal will trigger update
}

bool UserTableModel::updateUserRating(const QString& username, const QString& game, int rating) {
    return m_userRegistry->updateUserRating(username, game, rating); // Signal will trigger update
}

void UserTableModel::handleUserChanged(const QString& username, int row, UserRegistry::ChangeType changeType) {
    auto allUsers = m_userRegistry->getAllUsers();

    switch (changeType) {
    case UserRegistry::ChangeType::Created: {
        // Insert a new row for the new user
        beginInsertRows(QModelIndex(), row, row);
        endInsertRows();
        break;
    }
    case UserRegistry::ChangeType::Removed: {
        // Remove the row for the deleted user
        if (row >= 0) {
            beginRemoveRows(QModelIndex(), row, row);
            endRemoveRows();
        }
        break;
    }
    case UserRegistry::ChangeType::Updated: {
        // Update data for the specific user
        if (row >= 0) {
            emit dataChanged(index(row, 0), index(row, columnCount() - 1));
        }
        break;
    }
    }
}
