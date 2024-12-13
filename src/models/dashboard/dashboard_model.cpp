#include "dashboard_model.h"
#include <cstddef>

DashboardModel::DashboardModel(GameRegistry *gameRegistry, UserRegistry *userRegistry, QObject *parent)
    : QAbstractItemModel(parent), m_gameRegistry(gameRegistry), m_userRegistry(userRegistry)
{
    buildTree();
    connect(m_gameRegistry, &GameRegistry::gameAdded, this, &DashboardModel::onGameAdded);
    connect(m_gameRegistry, &GameRegistry::gameRemoved, this, &DashboardModel::onGameRemoved);

    connect(m_userRegistry, &UserRegistry::userChanged, this, &DashboardModel::onUserChanged);
}

const Game& DashboardModel::getGame(const QString &gameName) const
{
    return m_gameRegistry->getGame(gameName);
}

QStringList DashboardModel::getAllGameNames() const
{
    QStringList gameNames;
    for (const TreeNode *root : m_rootNodes)
    {
        if (!root->gameName.isEmpty())
        {
            gameNames.append(root->gameName);
        }
    }
    return gameNames;
}

void DashboardModel::buildTree()
{
    m_rootNodes.clear();

    for (const auto &game : m_gameRegistry->getAllGames())
    {
        TreeNode *gameNode = new TreeNode{game.getName(), "", ""};

        for (const auto &user : m_userRegistry->getAllUsers())
        {
            if (user.getPreferredGames().contains(game.getName()))
            {
                QString rating = QString::number(user.getRating(game.getName()));
                gameNode->children.append(new TreeNode{"", user.getUsername(), rating});
            }
        }

        m_rootNodes.append(gameNode);
    }
}

QModelIndex DashboardModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return createIndex(row, column, m_rootNodes[row]);
    }

    TreeNode *parentNode = static_cast<TreeNode *>(parent.internalPointer());
    return createIndex(row, column, parentNode->children[row]);
}

QModelIndex DashboardModel::parent(const QModelIndex &child) const
{
    TreeNode *childNode = static_cast<TreeNode *>(child.internalPointer());
    for (TreeNode *root : m_rootNodes)
    {
        if (root->children.contains(childNode))
        {
            return createIndex(m_rootNodes.indexOf(root), 0, root);
        }
    }
    return QModelIndex();
}

int DashboardModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return m_rootNodes.size();
    }

    TreeNode *parentNode = static_cast<TreeNode *>(parent.internalPointer());
    return parentNode->children.size();
}

int DashboardModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant DashboardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    TreeNode *node = static_cast<TreeNode *>(index.internalPointer());
    switch (index.column())
    {
    case 0:
        return node->gameName;
    case 1:
        return node->username;
    case 2:
        return node->rating;
    default:
        return QVariant();
    }
}

QVariant DashboardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return "Game";
        case 1:
            return "User";
        case 2:
            return "Rating";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void DashboardModel::onGameAdded(const QString &gameName)
{
    beginInsertRows(QModelIndex(), m_rootNodes.size(), m_rootNodes.size());

    TreeNode *newGameNode = new TreeNode{gameName, "", ""};
    m_rootNodes.append(newGameNode);

    endInsertRows();
}

void DashboardModel::onGameRemoved(const QString &gameName)
{
    for (std::size_t i = 0; i < m_rootNodes.size(); ++i)
    {
        if (m_rootNodes[i]->gameName == gameName)
        {
            beginRemoveRows(QModelIndex(), i, i);
            delete m_rootNodes.takeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void DashboardModel::onUserChanged(const QString &username, int row, UserRegistry::ChangeType changeType)
{
    switch (changeType)
    {
    case UserRegistry::ChangeType::Created:
    {
        for (std::size_t i = 0; i < m_rootNodes.size(); ++i)
        {
            if (m_userRegistry->getUserPreferredGames(username).contains(m_rootNodes[i]->gameName))
            {
                TreeNode *newUserNode = new TreeNode{"", username, QString::number(m_userRegistry->getUserRating(username, m_rootNodes[i]->gameName))};
                beginInsertRows(index(i, 0, QModelIndex()), m_rootNodes[i]->children.size(), m_rootNodes[i]->children.size());
                m_rootNodes[i]->children.append(newUserNode);
                endInsertRows();
            }
        }
        break;
    }

    case UserRegistry::ChangeType::Updated:
    {
        for (std::size_t i = 0; i < m_rootNodes.size(); ++i)
        {
            for (std::size_t j = 0; j < m_rootNodes[i]->children.size(); ++j)
            {
                TreeNode *userNode = m_rootNodes[i]->children[j];
                if (userNode->username == username)
                {
                    userNode->rating = QString::number(m_userRegistry->getUserRating(username, m_rootNodes[i]->gameName));
                    qDebug() << "username: " << username << " rating changed " << userNode->rating;
                    QModelIndex parentIndex = index(i, 0, QModelIndex()); // Top-level index
                    QModelIndex changedIndex = index(j, 2, parentIndex);  // Child index for rating column

                    if (changedIndex.isValid())
                    {
                        emit dataChanged(changedIndex, changedIndex);
                    }
                    else
                    {
                        qDebug() << "Invalid index for user:" << username
                                 << "at game:" << m_rootNodes[i]->gameName;
                    }
                }
            }
        }
        break;
    }

    case UserRegistry::ChangeType::Removed:
    {
        // removing user in all games
        for (std::size_t i = 0; i < m_rootNodes.size(); ++i)
        {
            for (std::size_t j = 0; j < m_rootNodes[i]->children.size(); ++j)
            {
                if (m_rootNodes[i]->children[j]->username == username)
                {
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
