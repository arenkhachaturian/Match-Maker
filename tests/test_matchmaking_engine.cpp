#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QString>
#include <QDir>
#include "engine/pool/user_pool_manager.h"
#include "engine/user_registry.h"
#include "engine/game_registry.h"
#include "engine/game_executor.h"
#include "engine/matchmaking_engine.h"

class TestMatchmakingEngine : public QObject {
    Q_OBJECT

private slots:
    void testMatchmaking();
};

void TestMatchmakingEngine::testMatchmaking() {
    // Create necessary components
    UserPoolManager userPool;
    UserRegistry userRegistry;
    GameRegistry gameRegistry;
    GameExecutor gameExecutor;

    // Path to the mock executable
    QString executablePath = QDir::toNativeSeparators("D:/projects/MatchMakingApp/tests/games/afw.bat");

    // Add games to the registry
    gameRegistry.addGame(Game(QString("Tic Tac Toe"), executablePath));
    gameRegistry.addGame(Game(QString("Chess"), executablePath));

    // Add users
    User user1("Alice");
    user1.setPreferredGames({"Tic Tac Toe", "Chess"});
    user1.updateRating("Tic Tac Toe", 5);
    user1.updateRating("Chess", 3);

    User user2("Bob");
    user2.setPreferredGames({"Tic Tac Toe"});
    user2.updateRating("Tic Tac Toe", 4);

    userRegistry.addUser(user1);
    userRegistry.addUser(user2);

    // Add users to the pool
    userPool.addUser("Alice", UserState::Waiting);
    userPool.addUser("Bob", UserState::Waiting);

    // Initialize the matchmaking engine
    MatchmakingEngine matchmakingEngine(&userPool, &userRegistry, &gameRegistry, &gameExecutor);

    // Connect to signals for verification
    QSignalSpy matchStartedSpy(&matchmakingEngine, &MatchmakingEngine::matchStarted);
    QSignalSpy matchFinishedSpy(&matchmakingEngine, &MatchmakingEngine::matchFinished);

    // Stop matchmaking after the first match ends
    connect(&matchmakingEngine, &MatchmakingEngine::matchFinished, [&]() {
        matchmakingEngine.stop();
    });

    // Start matchmaking
    matchmakingEngine.start(100); // 100ms interval

    // Wait for the matchmaking to process
    QTest::qWait(1000); // Ensure enough time for the match to finish and engine to stop

    // Verify matchStarted signal
    QCOMPARE(matchStartedSpy.count(), 1);
    auto matchStartedArgs = matchStartedSpy.takeFirst();
    QSet<QString> expectedPlayers{"Alice", "Bob"};
    QSet<QString> actualPlayersAtStart{
        matchStartedArgs.at(0).toString(),
        matchStartedArgs.at(1).toString()
    };
    QCOMPARE(actualPlayersAtStart, expectedPlayers);
    QCOMPARE(matchStartedArgs.at(2).toString(), QString("Tic Tac Toe"));

    // Verify matchFinished signal
    QCOMPARE(matchFinishedSpy.count(), 1);
    auto matchFinishedArgs = matchFinishedSpy.takeFirst();
    QSet<QString> actualPlayersAtEnd{
        matchFinishedArgs.at(0).toString(),
        matchFinishedArgs.at(1).toString()
    };
    QCOMPARE(actualPlayersAtEnd, expectedPlayers);
    QCOMPARE(matchFinishedArgs.at(2).toString(), matchStartedArgs.at(0).toString()); // first always Winner

    // Ensure the matchmaking engine stopped
    // QVERIFY(!matchmakingEngine.isRunning());
}

QTEST_MAIN(TestMatchmakingEngine)
#include "test_matchmaking_engine.moc"
