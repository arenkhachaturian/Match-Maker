#ifndef TEST_JSON_IO_HANDLER_H
#define TEST_JSON_IO_HANDLER_H

#include <QtTest/QtTest>

#include "engine/io/json_io_handler.h"
#include "engine/user.h"
#include "engine/game.h"

class TestJsonIOHandler : public QObject
{
    Q_OBJECT

private slots:
    void testWriteAndReadUsers();
    void testWriteAndReadGames();
};

#endif // TEST_JSON_IO_HANDLER_H
