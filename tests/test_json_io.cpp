#include "test_json_io.h"
#include "engine/game.h"

void TestJsonIOHandler::testWriteAndReadUsers()
{
    QString filePath = "test_users.json";
    JsonIOHandler handler(filePath);

    QList<User> users;
    User user1("john_doe");
    user1.setFirstName("John");
    user1.setLastName("Doe");
    user1.addPreferredGame("Chess");
    user1.addPreferredGame("Poker");
    user1.updateRating("Chess", 5);
    user1.updateRating("Poker", 4);

    User user2("jane_doe");
    user2.setFirstName("John");
    user2.setLastName("Doe");
    user2.addPreferredGame("Tennis");
    user2.updateRating("Tennis", 3);

    users.append(user1);
    users.append(user2);

    handler.writeUsers(users);

    QList<User> loadedUsers = handler.readUsers();

    QCOMPARE(loadedUsers.size(), 2);
    QCOMPARE(loadedUsers[0].getUsername(), QString("john_doe"));
    QCOMPARE(loadedUsers[0].getFirstName(), QString("John"));
    QCOMPARE(loadedUsers[0].getPreferredGames().contains("Chess"), true);
    QCOMPARE(loadedUsers[0].getRating("Chess"), 5);
    QCOMPARE(loadedUsers[1].getUsername(), QString("jane_doe"));
    QCOMPARE(loadedUsers[1].getPreferredGames().contains("Tennis"), true);
}

void TestJsonIOHandler::testWriteAndReadGames()
{
    QString filePath = "test_games.json";
    JsonIOHandler handler(filePath);

    QList<Game> games;
    games.append(Game("Chess"));
    games.append(Game("Poker"));

    handler.writeGames(games);

    QList<Game> loadedGames = handler.readGames();

    QCOMPARE(loadedGames.size(), 2);
    QCOMPARE(loadedGames[0].getName(), QString("Chess"));
    QCOMPARE(loadedGames[1].getName(), QString("Poker"));
}

QTEST_MAIN(TestJsonIOHandler)
#include "test_json_io.moc"
