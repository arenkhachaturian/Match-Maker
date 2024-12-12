#ifndef TEST_GAME_EXECUTOR_H
#define TEST_GAME_EXECUTOR_H


#include <QtTest/QtTest>

class TestGameExecutor : public QObject {
    Q_OBJECT

private slots:
    void testRunGameWithWinner();
    void testRunGameWithDraw();
};

#endif
