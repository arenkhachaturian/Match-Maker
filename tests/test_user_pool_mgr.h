#ifndef TEST_POOL_MGR_H
#define TEST_POOL_MGR_H

#include <QtTest/QtTest>

class TestUserPoolManager : public QObject {
    Q_OBJECT

private slots:
    void testAddUser();
    void testRemoveUser();
    void testChangeUserState();
    void testGetUsersByState();
    void testGetUserState();
};

#endif
