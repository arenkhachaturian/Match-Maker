#include "test_game.h"

void TestGame::testConstructor() {
    Game ticTacToe("Tic Tac Toe");
    QCOMPARE(ticTacToe.getName(), QString("Tic Tac Toe"));

    Game unnamedGame("");
    QCOMPARE(unnamedGame.getName(), QString(""));
}

void TestGame::testSetName() {
    Game game("Initial Name");
    QCOMPARE(game.getName(), QString("Initial Name"));

    // Assuming setName is implemented in Game
    // game.setName("Updated Name");
    // QCOMPARE(game.getName(), QString("Updated Name"));
}

void TestGame::testExecutablePath() {
    Game ticTacToe("Tic Tac Toe");
    ticTacToe.setExecutable("/path/to/executable");
    QCOMPARE(ticTacToe.getExecutablePath(), QString("/path/to/executable"));
}

QTEST_MAIN(TestGame)
#include "test_game.moc"
