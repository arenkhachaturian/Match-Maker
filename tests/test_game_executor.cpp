#include <QtTest/QtTest>
#include <QSignalSpy>
#include "engine/game/game_executor.h"
#include "engine/game/game.h"
#include <QDebug>
#include <fstream>

class TestGameExecutor : public QObject {
    Q_OBJECT

private slots:
    void testRunGameWithWinner();
    void testRunGameWithDraw();
};

void TestGameExecutor::testRunGameWithWinner() {
    // Mock game with the Python script as executable
    Game mockGame("Mock Game");
    QString path = QDir::toNativeSeparators(QDir::cleanPath("D:/projects/MatchMakingApp/tests/games/game.bat"));
    mockGame.setExecutable(path);

    // Create the GameExecutor and set the output stream
    GameExecutor executor;

    // Spies for the signals
    QSignalSpy startedSpy(&executor, &GameExecutor::gameStarted);
    QSignalSpy finishedSpy(&executor, &GameExecutor::gameFinished);

    // Named arguments for the mock game
    QMap<QString, QString> namedArgs;
    namedArgs["player1"] = "Alice";
    namedArgs["player2"] = "Bob";
    namedArgs["winner"] = "Alice";

    // Run the mock game
    executor.runGame(&mockGame, namedArgs);

    // Wait for the process to finish
    QTest::qWait(1000);

    // Verify the signals
    QCOMPARE(startedSpy.count(), 1);
    QCOMPARE(finishedSpy.count(), 1);

    // Verify the arguments passed in the signal
    QList<QVariant> finishedArgs = finishedSpy.takeFirst();
    GameResult result = finishedArgs.at(1).value<GameResult>();

    QCOMPARE(result.success, true);
    QCOMPARE(result.winner, QString("Alice"));
}

void TestGameExecutor::testRunGameWithDraw() {
    // Mock game with the Python script as executable
    Game mockGame("Mock Game");
    QString path = QDir::toNativeSeparators(QDir::cleanPath("D:/projects/MatchMakingApp/tests/games/game.bat"));
    mockGame.setExecutable(path); // Use Python explicitly to call the script

    // Create the GameExecutor
    GameExecutor executor;

    // Spies for the signals
    QSignalSpy startedSpy(&executor, &GameExecutor::gameStarted);
    QSignalSpy finishedSpy(&executor, &GameExecutor::gameFinished);

    // Named arguments for the mock game
    QMap<QString, QString> namedArgs;
    namedArgs["player1"] = "Alice";
    namedArgs["player2"] = "Bob";

    // Run the mock game
    executor.runGame(&mockGame, namedArgs);

    // Wait for the process to finish
    QTest::qWait(1000);

    // Verify the signals
    QCOMPARE(startedSpy.count(), 1);
    QCOMPARE(finishedSpy.count(), 1);

    // Verify the arguments passed in the signal
    QList<QVariant> finishedArgs = finishedSpy.takeFirst();
    GameResult result = finishedArgs.at(1).value<GameResult>();

    QCOMPARE(result.success, true);
    QCOMPARE(result.winner, QString(""));
}

QTEST_MAIN(TestGameExecutor)
#include "test_game_executor.moc"
