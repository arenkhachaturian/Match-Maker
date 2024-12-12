#include <QtTest/QtTest>

#include "engine/pool/user_pool_manager.h"
#include "test_user_pool_mgr.h"

void TestUserPoolManager::testAddUser() {
    UserPoolManager pool;
    pool.addUser("john_doe");
    QSet<QString> freeUsers = pool.getUsersByState(UserState::Free);

    QVERIFY(freeUsers.contains("john_doe"));
}

void TestUserPoolManager::testRemoveUser() {
    UserPoolManager pool;
    pool.addUser("john_doe");
    pool.removeUser("john_doe");

    QSet<QString> freeUsers = pool.getUsersByState(UserState::Free);
    QVERIFY(!freeUsers.contains("john_doe"));
}

void TestUserPoolManager::testChangeUserState() {
    UserPoolManager pool;
    pool.addUser("john_doe");
    pool.changeUserState("john_doe", UserState::Waiting);

    QSet<QString> freeUsers = pool.getUsersByState(UserState::Free);
    QSet<QString> waitingUsers = pool.getUsersByState(UserState::Waiting);

    QVERIFY(!freeUsers.contains("john_doe"));
    QVERIFY(waitingUsers.contains("john_doe"));
}

void TestUserPoolManager::testGetUsersByState() {
    UserPoolManager pool;
    pool.addUser("john_doe", UserState::Busy);
    QSet<QString> busyUsers = pool.getUsersByState(UserState::Busy);

    QVERIFY(busyUsers.contains("john_doe"));
    QVERIFY(busyUsers.size() == 1);
}

void TestUserPoolManager::testGetUserState() {
    UserPoolManager pool;
    pool.addUser("john_doe", UserState::Waiting);
    QCOMPARE(pool.getUserState("john_doe"), UserState::Waiting);
}

QTEST_MAIN(TestUserPoolManager)
#include "test_user_pool_mgr.moc"
