#ifndef TEST_USER_H
#define TEST_USER_H

#include <QtTest/QtTest>
#include "engine/user.h"

class TestUser : public QObject
{
    Q_OBJECT

private slots:
    // Tests for constructors
    void testConstructorSimple();
    void testConstructorComplex();

    // Tests for getters and setters
    void testSetAndGetFirstName();
    void testSetAndGetLastName();
    void testSetAndGetPreferredGames();
    void testSetAndGetRatings();

    // Tests for preferred games
    void testAddPreferredGame();
    void testRemovePreferredGame();

    // Tests for ratings
    void testUpdateRating();
    void testDefaultRating();
};

#endif // TEST_USER_H
