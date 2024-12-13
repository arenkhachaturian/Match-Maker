#include <QtTest/QtTest>
#include <QSignalSpy>
#include "engine/game/game_executor.h"
#include "engine/game/game.h"
#include <QDebug>
#include <fstream>

class TestGameExecutor final : public QObject
{
    Q_OBJECT

private slots:
    void testRunGameWithWinner();
    void testRunGameWithDraw();
};

void TestGameExecutor::testRunGameWithWinner()
{
    Game mockGame("Mock Game");
    QString path = QDir::toNativeSeparators(QDir::cleanPath("D:/projects/MatchMakingApp/tests/games/game.bat"));
    mockGame.setExecutablePath(path);

    GameExecutor executor;

    QSignalSpy startedSpy(&executor, &GameExecutor::gameStarted);
    QSignalSpy finishedSpy(&executor, &GameExecutor::gameFinished);

    QMap<QString, QString> namedArgs;
    namedArgs["player1"] = "Alice";
    namedArgs["player2"] = "Bob";
    namedArgs["winner"] = "Alice";

    executor.runGame(mockGame, namedArgs);

    QTest::qWait(1000);

    QCOMPARE(startedSpy.count(), 1);
    QCOMPARE(finishedSpy.count(), 1);

    QList<QVariant> finishedArgs = finishedSpy.takeFirst();
    GameResult result = finishedArgs.at(1).value<GameResult>();

    QCOMPARE(result.success, true);
    QCOMPARE(result.winner, QString("Alice"));
}

void TestGameExecutor::testRunGameWithDraw()
{
    Game mockGame("Mock Game");
    QString path = QDir::toNativeSeparators(QDir::cleanPath("D:/projects/MatchMakingApp/tests/games/game.bat"));
    mockGame.setExecutablePath(path);

    GameExecutor executor;

    QSignalSpy startedSpy(&executor, &GameExecutor::gameStarted);
    QSignalSpy finishedSpy(&executor, &GameExecutor::gameFinished);

    QMap<QString, QString> namedArgs;
    namedArgs["player1"] = "Alice";
    namedArgs["player2"] = "Bob";

    executor.runGame(mockGame, namedArgs);

    QTest::qWait(1000);

    QCOMPARE(startedSpy.count(), 1);
    QCOMPARE(finishedSpy.count(), 1);

    QList<QVariant> finishedArgs = finishedSpy.takeFirst();
    GameResult result = finishedArgs.at(1).value<GameResult>();

    QCOMPARE(result.success, true);
    QCOMPARE(result.winner, QString(""));
}

QTEST_MAIN(TestGameExecutor)
#include "test_game_executor.moc"
