#include "engine/matchmaking/matchmaking_engine.h"
#include <QDebug>

MatchmakingEngine::MatchmakingEngine(UserPoolManager* userPool, UserRegistry* userRegistry, GameRegistry* gameRegistry, GameExecutor* gameExecutor, QObject* parent)
    : QObject(parent), m_userPoolManager(userPool), m_userRegistry(userRegistry), m_gameRegistry(gameRegistry), m_gameExecutor(gameExecutor) {
    connect(&m_timer, &QTimer::timeout, this, &MatchmakingEngine::processMatchmaking);

    // Handle game results
    connect(m_gameExecutor, &GameExecutor::gameFinished, this, [&](const QString& gameName, const GameResult& result) {
        if (!result.success) {
            qWarning() << "Game execution failed for" << gameName;
            return;
        }

        const QStringList& players = result.players;
        const QString& winner = result.winner;

        // Change players back to Free state
        m_userPoolManager->changeUserState(players[0], UserState::Free);
        m_userPoolManager->changeUserState(players[1], UserState::Free);

        // Update the winner's rating
        if (!winner.isEmpty()) {
            m_userRegistry->updateUserRating(winner, gameName, m_userRegistry->getUser(winner)->getRating(gameName) + 1);
        }

        emit matchFinished(players[0], players[1], winner);
    });
}

void MatchmakingEngine::start(int intervalMs) {
    m_timer.start(intervalMs);
}

void MatchmakingEngine::stop() {
    m_timer.stop();
}

void MatchmakingEngine::processMatchmaking() {
    const QSet<QString>& waitingUsers = m_userPoolManager->getUsersByState(UserState::Waiting);

    for (const QString& username : waitingUsers) {
        auto preferredGames = m_userRegistry->getUser(username)->getPreferredGames();
        if (preferredGames.isEmpty()) continue;

        auto [opponent, game] = findOpponent(username, preferredGames, m_ratingRange);
        if (opponent.isEmpty() || !game) continue;

        // Change states to Busy
        m_userPoolManager->changeUserState(username, UserState::Busy);
        m_userPoolManager->changeUserState(opponent, UserState::Busy);

        emit matchStarted(username, opponent, game->getName());

        m_gameExecutor->runGame(game.get(), {{"player1", username}, {"player2", opponent}});
    }
}

std::pair<QString, std::shared_ptr<const Game>> MatchmakingEngine::findOpponent(const QString& username, const QSet<QString>& preferredGames, int ratingRange) {
    const QSet<QString>& waitingUsers = m_userPoolManager->getUsersByState(UserState::Waiting);

    for (const QString& opponent : waitingUsers) {
        if (opponent == username) continue;

        const auto oponentUesr = m_userRegistry->getUser(opponent);
        QSet<QString> opponentPreferredGames = oponentUesr->getPreferredGames();
        QSet<QString> commonGames = opponentPreferredGames & preferredGames;

        if (commonGames.isEmpty()) continue;

        for (const QString& gameName : commonGames) {
            int userRating = m_userRegistry->getUser(username)->getRating(gameName);
            int opponentRating = m_userRegistry->getUser(opponent)->getRating(gameName);

            if (qAbs(userRating - opponentRating) <= ratingRange) {
                auto game = m_gameRegistry->getGame(gameName);
                return {opponent, game};
            }
        }
    }
    return {"", nullptr}; // No suitable opponent found
}

bool MatchmakingEngine::requestMatch(const QString& username) {
    QMutexLocker locker(&m_mutex); // Protect shared resources
    try {
        // Check if the user is free
        if (m_userPoolManager->getUserState(username) != UserState::Free) {
            qWarning() << "User" << username << "is not free.";
            return false;
        }

        // Change user state to Waiting
        m_userPoolManager->changeUserState(username, UserState::Waiting);

        // Trigger matchmaking process
        processMatchmaking();
        return true;
    } catch (const std::exception& e) {
        qWarning() << "Error requesting match for user" << username << ":" << e.what();
        return false;
    }
}

