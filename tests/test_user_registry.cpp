#include "engine/user.h"
#include "engine/user_registry.h"
#include <QtTest/QtTest>

class TestUserRegistry : public QObject {
    Q_OBJECT

private slots:
    void testAddUser();
    void testRemoveUser();
    void testGetUser();
    void testUpdateUserRating();
    void testSignals();
};

void TestUserRegistry::testAddUser() {
    UserRegistry registry;
    User user("john_doe");

    QVERIFY(registry.addUser(user));  // Adding a new user should succeed
    QVERIFY(!registry.addUser(user)); // Adding the same user again should fail
}

void TestUserRegistry::testRemoveUser() {
    UserRegistry registry;
    User user("john_doe");

    registry.addUser(user);
    QVERIFY(registry.removeUser("john_doe"));  // Removing an existing user should succeed
    QVERIFY(!registry.removeUser("john_doe")); // Removing a non-existent user should fail
}

void TestUserRegistry::testGetUser() {
    UserRegistry registry;
    User user("john_doe");

    registry.addUser(user);
    auto retrievedUser = registry.getUser("john_doe");
    QVERIFY(retrievedUser != nullptr);                // User should exist
    QCOMPARE(retrievedUser->getUsername(), QString("john_doe"));

    auto nonExistentUser = registry.getUser("jane_doe");
    QVERIFY(nonExistentUser == nullptr); // Non-existent user should return nullptr
}

void TestUserRegistry::testUpdateUserRating() {
    UserRegistry registry;
    User user("john_doe");

    registry.addUser(user);
    QVERIFY(registry.updateUserRating("john_doe", "chess", 5)); // Updating rating should succeed

    auto retrievedUser = registry.getUser("john_doe");
    QVERIFY(retrievedUser != nullptr);
    QCOMPARE(retrievedUser->getRating("chess"), 5);

    QVERIFY(!registry.updateUserRating("jane_doe", "chess", 5)); // Updating a non-existent user should fail
}

void TestUserRegistry::testSignals() {
    UserRegistry registry;
    User user("john_doe");

    // Monitor the signal emission
    QSignalSpy signalSpy(&registry, &UserRegistry::userChanged);

    registry.addUser(user);

    signalSpy.wait(1000);
    QCOMPARE(signalSpy.count(), 1); // Signal should be emitted once

    QList<QVariant> arguments = signalSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("john_doe")); // Username
    QCOMPARE(arguments.at(2).value<UserRegistry::ChangeType>(), UserRegistry::ChangeType::Created);

    registry.updateUserRating("john_doe", "chess", 5);

    signalSpy.wait(1000); // Wait for the signal
    QCOMPARE(signalSpy.count(), 1); // Signal should be emitted once

    arguments = signalSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("john_doe")); // Username
    QCOMPARE(arguments.at(2).value<UserRegistry::ChangeType>(), UserRegistry::ChangeType::Updated);

    registry.removeUser("john_doe");

    signalSpy.wait(1000); // Wait for the signal
    QCOMPARE(signalSpy.count(), 1); // Signal should be emitted once

    arguments = signalSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("john_doe")); // Username
    QCOMPARE(arguments.at(2).value<UserRegistry::ChangeType>(), UserRegistry::ChangeType::Removed);
}

QTEST_MAIN(TestUserRegistry)
#include "test_user_registry.moc"
