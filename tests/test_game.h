#ifndef TEST_GAME_H
#define TEST_GAME_H

#include <QtTest/QtTest>
#include "engine/game.h"

class TestGame : public QObject
{
    Q_OBJECT

private slots:
    void testConstructor();       // Test the constructor
    void testSetName();           // Test setting and getting the game name
    void testExecutablePath();    // Test setting and getting the executable path
};

#endif // TEST_GAME_H
