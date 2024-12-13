#ifndef MATCHMAKING_ENGINE_H
#define MATCHMAKING_ENGINE_H

#include "engine/pool/user_pool_manager.h"
#include "engine/user/user_registry.h"
#include "engine/game/game_registry.h"
#include "engine/game/game_executor.h"
#include <QObject>
#include <QString>
#include <QTimer>
#include <QAtomicInt>

constexpr int MAX_MATCHES = 10;

class MatchmakingEngine final : public QObject
{
    Q_OBJECT

public:
    explicit MatchmakingEngine(UserPoolManager *userPool, UserRegistry *userRegistry, GameRegistry *gameRegistry, GameExecutor *gameExecutor, QObject *parent = nullptr);

    void start(int intervalMs = 1000);
    void stop();
    bool requestMatch(const QString &username);

signals:
    void matchStarted(const QString &player1, const QString &player2, const QString &game);
    void matchFinished(const QString &player1, const QString &player2, const QString &result);

private slots:
    void processMatchmaking();
    void handleRegistryUpdate(const QString &username, int row, UserRegistry::ChangeType changeType);

private:
    UserPoolManager *m_userPoolManager;
    UserRegistry *m_userRegistry;
    GameRegistry *m_gameRegistry;
    GameExecutor *m_gameExecutor;
    QTimer m_timer;
    int m_ratingRange = 3;

    std::pair<QString, QString> findOpponent(const QString &username, const QSet<QString> &preferredGames, int ratingRange);

    mutable QMutex m_mutex;
    mutable QAtomicInt matchCounter{0};
};

#endif // MATCHMAKING_ENGINE_H
