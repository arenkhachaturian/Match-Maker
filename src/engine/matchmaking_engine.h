#ifndef MATCHMAKING_ENGINE_H
#define MATCHMAKING_ENGINE_H

#include <QObject>
#include <QString>
#include <QTimer>
#include "engine/pool/user_pool_manager.h"
#include "engine/user_registry.h"
#include "engine/game_registry.h"
#include "engine/game_executor.h"

class MatchmakingEngine : public QObject {
    Q_OBJECT

public:
    explicit MatchmakingEngine(UserPoolManager* userPool, UserRegistry* userRegistry, GameRegistry* gameRegistry, GameExecutor* gameExecutor, QObject* parent = nullptr);

    void start(int intervalMs = 1000); // Start matchmaking
    void stop();                      // Stop matchmaking
    bool requestMatch(const QString& username);

signals:
    void matchStarted(const QString& player1, const QString& player2, const QString& game);
    void matchFinished(const QString& player1, const QString& player2, const QString& result);

private slots:
    void processMatchmaking();

private:
    UserPoolManager* m_userPoolManager;
    UserRegistry* m_userRegistry;
    GameRegistry* m_gameRegistry;
    GameExecutor* m_gameExecutor;
    QTimer m_timer;
    int m_ratingRange = 3;

    std::pair<QString, std::shared_ptr<const Game>> findOpponent(const QString& username, const QSet<QString>& preferredGames, int ratingRange);

    mutable QMutex m_mutex;
};

#endif // MATCHMAKING_ENGINE_H
