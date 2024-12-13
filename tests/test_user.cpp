#include <QtTest/QtTest>
#include <QSignalSpy>
#include "engine/user/user.h"

class TestUser final : public QObject
{
    Q_OBJECT

private slots:
    void testConstructorSimple();
    void testConstructorComplex();

    void testSetAndGetFirstName();
    void testSetAndGetLastName();
    void testSetAndGetPreferredGames();
    void testSetAndGetRatings();

    void testAddPreferredGame();
    void testRemovePreferredGame();

    void testUpdateRating();
    void testDefaultRating();
};

void TestUser::testConstructorSimple()
{
    User user("simple_user");
    QCOMPARE(user.getUsername(), QString("simple_user"));
    QCOMPARE(user.getFirstName(), QString());
    QCOMPARE(user.getLastName(), QString());
    QCOMPARE(user.getPreferredGames().isEmpty(), true);
    QCOMPARE(user.getRatings().isEmpty(), true);
}

void TestUser::testConstructorComplex()
{
    QSet<QString> games = {"Chess", "Poker"};
    QMap<QString, int> ratings = {{"Chess", 5}, {"Poker", 4}};
    User user("complex_user", "John", "Doe", games, ratings);

    QCOMPARE(user.getUsername(), QString("complex_user"));
    QCOMPARE(user.getFirstName(), QString("John"));
    QCOMPARE(user.getLastName(), QString("Doe"));
    QCOMPARE(user.getPreferredGames(), games);
    QCOMPARE(user.getRatings(), ratings);
}

void TestUser::testSetAndGetFirstName()
{
    User user("test_user");
    user.setFirstName("Alice");
    QCOMPARE(user.getFirstName(), QString("Alice"));
}

void TestUser::testSetAndGetLastName()
{
    User user("test_user");
    user.setLastName("Smith");
    QCOMPARE(user.getLastName(), QString("Smith"));
}

void TestUser::testSetAndGetPreferredGames()
{
    User user("test_user");
    QSet<QString> games = {"Tennis", "Basketball"};
    user.setPreferredGames(games);
    QCOMPARE(user.getPreferredGames(), games);
}

void TestUser::testSetAndGetRatings()
{
    User user("test_user");
    QMap<QString, int> ratings = {{"GameA", 3}, {"GameB", 2}};
    user.setRatings(ratings);
    QCOMPARE(user.getRatings(), ratings);
}

void TestUser::testAddPreferredGame()
{
    User user("test_user");
    user.addPreferredGame("Chess");
    QCOMPARE(user.getPreferredGames().contains("Chess"), true);
    user.addPreferredGame("Chess");
    QCOMPARE(user.getPreferredGames().size(), 1);
}

void TestUser::testRemovePreferredGame()
{
    User user("test_user");
    user.addPreferredGame("Chess");
    user.removePreferredGame("Chess");
    QCOMPARE(user.getPreferredGames().contains("Chess"), false);
}

void TestUser::testUpdateRating()
{
    User user("test_user");
    user.updateRating("Chess", 3);
    QCOMPARE(user.getRating("Chess"), 3);
    user.updateRating("Chess", 2);
    QCOMPARE(user.getRating("Chess"), 2);
}

void TestUser::testDefaultRating()
{
    User user("test_user");
    QCOMPARE(user.getRating("NonExistentGame"), 0);
}

QTEST_MAIN(TestUser)
#include "test_user.moc"
