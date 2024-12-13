#include "engine/user/user.h"
#include "engine/user/user_registry.h"
#include <QtTest/QtTest>

class TestUserRegistry final : public QObject
{
    Q_OBJECT

private slots:
    void testAddUser();
    void testRemoveUser();
    void testGetUser();
    void testUpdateUserRating();
    void testSignals();
};

void TestUserRegistry::testAddUser()
{
    UserRegistry registry;
    User user("john_doe");

    QVERIFY(registry.addUser(user));
    QVERIFY(!registry.addUser(user));
}

void TestUserRegistry::testRemoveUser()
{
    UserRegistry registry;
    User user("john_doe");

    registry.addUser(user);
    QVERIFY(registry.removeUser("john_doe"));
    QVERIFY(!registry.removeUser("john_doe"));
}

void TestUserRegistry::testGetUser()
{
    UserRegistry registry;
    User user("john_doe");

    registry.addUser(user);
    QCOMPARE(registry.isExistingUser("john_doe"), true);

    QVERIFY(registry.isExistingUser("jane_doe") == false);
}

void TestUserRegistry::testUpdateUserRating()
{
    UserRegistry registry;
    User user("john_doe");

    registry.addUser(user);
    QVERIFY(registry.updateUserRating("john_doe", "chess", 5));

    QCOMPARE(registry.isExistingUser("john_doe"), true);
    QCOMPARE(registry.getUserRating("john_doe", "chess"), 5);

    QVERIFY(!registry.updateUserRating("jane_doe", "chess", 5));
}

void TestUserRegistry::testSignals()
{
    UserRegistry registry;
    User user("john_doe");

    QSignalSpy signalSpy(&registry, &UserRegistry::userChanged);

    registry.addUser(user);

    signalSpy.wait(1000);
    QCOMPARE(signalSpy.count(), 1);

    QList<QVariant> arguments = signalSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("john_doe"));
    QCOMPARE(arguments.at(2).value<UserRegistry::ChangeType>(), UserRegistry::ChangeType::Created);

    registry.updateUserRating("john_doe", "chess", 5);

    signalSpy.wait(1000);
    QCOMPARE(signalSpy.count(), 1);

    arguments = signalSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("john_doe"));
    QCOMPARE(arguments.at(2).value<UserRegistry::ChangeType>(), UserRegistry::ChangeType::Updated);

    registry.removeUser("john_doe");

    signalSpy.wait(1000);
    QCOMPARE(signalSpy.count(), 1);

    arguments = signalSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("john_doe"));
    QCOMPARE(arguments.at(2).value<UserRegistry::ChangeType>(), UserRegistry::ChangeType::Removed);
}

QTEST_MAIN(TestUserRegistry)
#include "test_user_registry.moc"
