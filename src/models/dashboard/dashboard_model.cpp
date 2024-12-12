#include "dashboard_model.h"

DashboardModel::DashboardModel(GameRegistry* gameRegistry, UserRegistry* userRegistry, QObject* parent)
    : QAbstractItemModel(parent), m_gameRegistry(gameRegistry), m_userRegistry(userRegistry) {
    buildTree(); // Build the tree structure
    connect(m_gameRegistry, &GameRegistry::gameAdded, this, &DashboardModel::onGameAdded);
    connect(m_gameRegistry, &GameRegistry::gameRemoved, this, &DashboardModel::onGameRemoved);

    // Connect UserRegistry signals
    connect(m_userRegistry, &UserRegistry::userChanged, this, &DashboardModel::onUserChanged);
}

std::shared_ptr<const Game> DashboardModel::getGame(const QString& gameName) const {
    return m_gameRegistry->getGame(gameName);
}

QStringList DashboardModel::getAllGameNames() const {
    QStringList gameNames;
    for (const TreeNode* root : m_rootNodes) {
        if (!root->gameName.isEmpty()) { // Ensure it's a valid game node
            gameNames.append(root->gameName);
        }
    }
    return gameNames;
}

void DashboardModel::buildTree() {
    m_rootNodes.clear();

    // Iterate over all games
    for (const auto& game : m_gameRegistry->getAllGames()) {
        TreeNode* gameNode = new TreeNode{game.getName(), "", ""};

        // Find all users who play this game
        for (const auto& user : m_userRegistry->getAllUsers()) {
            if (user.getPreferredGames().contains(game.getName())) {
                QString rating = QString::number(user.getRating(game.getName()));
                gameNode->children.append(new TreeNode{"", user.getUsername(), rating});
            }
        }

        m_rootNodes.append(gameNode);
    }
}

QModelIndex DashboardModel::index(int row, int column, const QModelIndex& parent) const {
    if (!parent.isValid()) {
        return createIndex(row, column, m_rootNodes[row]);
    }

    TreeNode* parentNode = static_cast<TreeNode*>(parent.internalPointer());
    return createIndex(row, column, parentNode->children[row]);
}

QModelIndex DashboardModel::parent(const QModelIndex& child) const {
    TreeNode* childNode = static_cast<TreeNode*>(child.internalPointer());
    for (TreeNode* root : m_rootNodes) {
        if (root->children.contains(childNode)) {
            return createIndex(m_rootNodes.indexOf(root), 0, root);
        }
    }
    return QModelIndex();
}

int DashboardModel::rowCount(const QModelIndex& parent) const {
    if (!parent.isValid()) {
        return m_rootNodes.size(); // Top-level (games)
    }

    TreeNode* parentNode = static_cast<TreeNode*>(parent.internalPointer());
    return parentNode->children.size(); // Children (users)
}

int DashboardModel::columnCount(const QModelIndex&) const {
    return 3; // Game, Username, Rating
}

QVariant DashboardModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    switch (index.column()) {
    case 0: // Game name
        return node->gameName;
    case 1: // Username
        return node->username;
    case 2: // Rating
        return node->rating;
    default:
        return QVariant();
    }
}

QVariant DashboardModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Game";
        case 1: return "User";
        case 2: return "Rating";
        default: return QVariant();
        }
    }
    return QVariant();
}

void DashboardModel::onGameAdded(const QString& gameName) {
    // Insert a new game node at the appropriate position
    beginInsertRows(QModelIndex(), m_rootNodes.size(), m_rootNodes.size());

    TreeNode* newGameNode = new TreeNode{gameName, "", ""};
    m_rootNodes.append(newGameNode);

    endInsertRows();
}

void DashboardModel::onGameRemoved(const QString& gameName) {
    // Find the game node and remove it
    for (int i = 0; i < m_rootNodes.size(); ++i) {
        if (m_rootNodes[i]->gameName == gameName) {
            beginRemoveRows(QModelIndex(), i, i);
            delete m_rootNodes.takeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void DashboardModel::onUserChanged(const QString& username, int row,  UserRegistry::ChangeType changeType) {
    switch (changeType) {
    case UserRegistry::ChangeType::Created: {
        // Add the user to all their preferred games
        auto user = m_userRegistry->getUser(username);
        if (!user) return;

        for (int i = 0; i < m_rootNodes.size(); ++i) {
            if (user->getPreferredGames().contains(m_rootNodes[i]->gameName)) {
                TreeNode* newUserNode = new TreeNode{"", username, QString::number(user->getRating(m_rootNodes[i]->gameName))};
                beginInsertRows(index(i, 0, QModelIndex()), m_rootNodes[i]->children.size(), m_rootNodes[i]->children.size());
                m_rootNodes[i]->children.append(newUserNode);
                endInsertRows();
            }
        }
        break;
    }

    case UserRegistry::ChangeType::Updated: {
        // Update the user's ratings
        auto user = m_userRegistry->getUser(username);
        if (!user) return;

        for (int i = 0; i < m_rootNodes.size(); ++i) {
            for (int j = 0; j < m_rootNodes[i]->children.size(); ++j) {
                TreeNode* userNode = m_rootNodes[i]->children[j];
                if (userNode->username == username) {
                    userNode->rating = QString::number(user->getRating(m_rootNodes[i]->gameName));
                    qDebug() << "username: " << username << " rating changed " << userNode->rating;
                    QModelIndex parentIndex = index(i, 0, QModelIndex()); // Top-level index
                    QModelIndex changedIndex = index(j, 2, parentIndex);  // Child index for rating column

                    if (changedIndex.isValid()) {
                        emit dataChanged(changedIndex, changedIndex); // Notify the view
                    } else {
                        qDebug() << "Invalid index for user:" << username
                                 << "at game:" << m_rootNodes[i]->gameName;
                    }
                }
            }
        }
        break;
    }

    case UserRegistry::ChangeType::Removed: {
        // Remove the user from all games
        for (int i = 0; i < m_rootNodes.size(); ++i) {
            for (int j = 0; j < m_rootNodes[i]->children.size(); ++j) {
                if (m_rootNodes[i]->children[j]->username == username) {
                    beginRemoveRows(index(i, 0, QModelIndex()), j, j);
                    delete m_rootNodes[i]->children.takeAt(j);
                    endRemoveRows();
                }
            }
        }
        break;
    }
    }
}

