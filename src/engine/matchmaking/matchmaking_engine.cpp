#include "engine/matchmaking/matchmaking_engine.h"
#include <QDebug>

MatchmakingEngine::MatchmakingEngine(UserPoolManager *userPool, UserRegistry *userRegistry, GameRegistry *gameRegistry, GameExecutor *gameExecutor, QObject *parent)
    : QObject(parent), m_userPoolManager(userPool), m_userRegistry(userRegistry), m_gameRegistry(gameRegistry), m_gameExecutor(gameExecutor)
{
    connect(&m_timer, &QTimer::timeout, this, &MatchmakingEngine::processMatchmaking);
    connect(m_userRegistry, UserRegistry::userChanged, this, &MatchmakingEngine::handleRegistryUpdate);

    // Handle game results
    connect(m_gameExecutor, &GameExecutor::gameFinished, this, [&](const QString &gameName, const GameResult &result)
            {
        if (!result.success) {
            qWarning() << "Game execution failed for" << gameName;
            return;
        }

        const QStringList& players = result.players;
        const QString& winner = result.winner;

        qDebug() << "match ended: " << players[0] << " vs " << players[1] << " WINNER: " << winner;
        m_userPoolManager->changeUserState(players[0], UserState::Free);
        m_userPoolManager->changeUserState(players[1], UserState::Free);

        if (!winner.isEmpty()) {
            m_userRegistry->incrementUserRating(winner, gameName, 1/*increment*/);
        }

        emit matchFinished(players[0], players[1], winner); });
}

void MatchmakingEngine::start(int intervalMs)
{
    m_timer.start(intervalMs);
}

void MatchmakingEngine::stop()
{
    m_timer.stop();
}

void MatchmakingEngine::handleRegistryUpdate(const QString &username, int row, UserRegistry::ChangeType changeType)
{
    switch (changeType)
    {
    case UserRegistry::ChangeType::Created:
    {
        m_userPoolManager->addUser(username, UserState::Free);
        break;
    }
    case UserRegistry::ChangeType::Removed:
    {
        m_userPoolManager->removeUser(username);
        break;
    }
        return;
    }
}

void MatchmakingEngine::processMatchmaking()
{
    QMutexLocker locker(&m_mutex);
    const QSet<QString> &waitingUsers = m_userPoolManager->getUsersByState(UserState::Waiting);

    for (const QString &username : waitingUsers)
    {
        auto preferredGames = m_userRegistry->getUserPreferredGames(username);
        if (preferredGames.isEmpty())
            continue;

        auto [opponentName, gameName] = findOpponent(username, preferredGames, m_ratingRange);
        qDebug() << "opponent for" << username << " is " << opponentName << "in gmae " << gameName;
        if (opponentName.isEmpty() || gameName.isEmpty())
            continue;

        m_userPoolManager->changeUserState(username, UserState::Busy);
        m_userPoolManager->changeUserState(opponentName, UserState::Busy);

        qDebug() << "Match started: " << username << " vs " << opponentName;
        emit matchStarted(username, opponentName, gameName);
        m_gameExecutor->runGame(m_gameRegistry->getGame(gameName), {{"player1", username}, {"player2", opponentName}});
    }
}

// first check opponent emptiness
std::pair<QString, QString> MatchmakingEngine::findOpponent(const QString &username, const QSet<QString> &preferredGames, int ratingRange)
{
    const QSet<QString> &waitingUsers = m_userPoolManager->getUsersByState(UserState::Waiting);
    qDebug() << "find oponent for " << username << " in " <<  m_userPoolManager->getUsersByState(UserState::Waiting);
    for (const QString &opponent : waitingUsers)
    {
        if (opponent == username)
            continue;

        QSet<QString> opponentPreferredGames = m_userRegistry->getUserPreferredGames(opponent);
        QSet<QString> commonGames = opponentPreferredGames & preferredGames;

        if (commonGames.isEmpty())
            continue;

        for (const QString &gameName : commonGames)
        {
            int userRating = m_userRegistry->getUserRating(username, gameName);
            int opponentRating = m_userRegistry->getUserRating(opponent, gameName);

            if (qAbs(userRating - opponentRating) <= ratingRange)
            {
                return {opponent, gameName};
            }
        }
    }
    return {QString{}, QString{}};
}

bool MatchmakingEngine::requestMatch(const QString &username)
{
    QMutexLocker locker(&m_mutex);
    try
    {
        if (m_userPoolManager->getUserState(username) != UserState::Free)
        {
            qWarning() << "User" << username << "is not free.";
            return false;
        }

        m_userPoolManager->changeUserState(username, UserState::Waiting);

        return true;
    }
    catch (const std::exception &e)
    {
        qWarning() << "Error requesting match for user" << username << ":" << e.what();
        return false;
    }
}
