#include <QtTest/QtTest>
#include <QSignalSpy>
#include "engine/game/game.h"

class TestGame final : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor();
    void testSetName();
    void testExecutablePath();
};

void TestGame::testConstructor() {
    Game ticTacToe("Tic Tac Toe");
    QCOMPARE(ticTacToe.getName(), QString("Tic Tac Toe"));

    Game unnamedGame("");
    QCOMPARE(unnamedGame.getName(), QString(""));
}

void TestGame::testSetName() {
    Game game("Initial Name");
    QCOMPARE(game.getName(), QString("Initial Name"));
}

void TestGame::testExecutablePath() {
    Game ticTacToe("Tic Tac Toe");
    ticTacToe.setExecutablePath("/fake/path");
    QCOMPARE(ticTacToe.getExecutablePath(), QString("/fake/path"));
}

QTEST_MAIN(TestGame)
#include "test_game.moc"
