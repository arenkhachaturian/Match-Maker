#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QString>
#include <QDir>
#include "engine/pool/user_pool_manager.h"
#include "engine/user/user_registry.h"
#include "engine/game/game_registry.h"
#include "engine/game/game_executor.h"
#include "engine/matchmaking/matchmaking_engine.h"

class TestMatchmakingEngine final : public QObject
{
    Q_OBJECT

private slots:
    void testMatchmaking();
};

void TestMatchmakingEngine::testMatchmaking()
{
    UserPoolManager userPool;
    UserRegistry userRegistry;
    GameRegistry gameRegistry;
    GameExecutor gameExecutor;

    QString executablePath = QDir::toNativeSeparators("D:/projects/MatchMakingApp/tests/games/afw.bat");

    gameRegistry.addGame(Game(QString("Tic Tac Toe"), executablePath));
    gameRegistry.addGame(Game(QString("Chess"), executablePath));

    User user1("Alice");
    user1.setPreferredGames({"Tic Tac Toe", "Chess"});
    user1.updateRating("Tic Tac Toe", 5);
    user1.updateRating("Chess", 3);

    User user2("Bob");
    user2.setPreferredGames({"Tic Tac Toe"});
    user2.updateRating("Tic Tac Toe", 4);

    userRegistry.addUser(user1);
    userRegistry.addUser(user2);

    userPool.addUser("Alice", UserState::Waiting);
    userPool.addUser("Bob", UserState::Waiting);

    MatchmakingEngine matchmakingEngine(&userPool, &userRegistry, &gameRegistry, &gameExecutor);

    QSignalSpy matchStartedSpy(&matchmakingEngine, &MatchmakingEngine::matchStarted);
    QSignalSpy matchFinishedSpy(&matchmakingEngine, &MatchmakingEngine::matchFinished);

    connect(&matchmakingEngine, &MatchmakingEngine::matchFinished, [&]()
            { matchmakingEngine.stop(); });

    matchmakingEngine.start(100);

    QTest::qWait(1000);

    QCOMPARE(matchStartedSpy.count(), 1);
    auto matchStartedArgs = matchStartedSpy.takeFirst();
    QSet<QString> expectedPlayers{"Alice", "Bob"};
    QSet<QString> actualPlayersAtStart{
        matchStartedArgs.at(0).toString(),
        matchStartedArgs.at(1).toString()};
    QCOMPARE(actualPlayersAtStart, expectedPlayers);
    QCOMPARE(matchStartedArgs.at(2).toString(), QString("Tic Tac Toe"));

    QCOMPARE(matchFinishedSpy.count(), 1);
    auto matchFinishedArgs = matchFinishedSpy.takeFirst();
    QSet<QString> actualPlayersAtEnd{
        matchFinishedArgs.at(0).toString(),
        matchFinishedArgs.at(1).toString()};
    QCOMPARE(actualPlayersAtEnd, expectedPlayers);
    QCOMPARE(matchFinishedArgs.at(2).toString(), matchStartedArgs.at(0).toString()); // first always Winner
}

QTEST_MAIN(TestMatchmakingEngine)
#include "test_matchmaking_engine.moc"
