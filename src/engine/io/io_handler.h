#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include <QString>
#include <QList>
#include "engine/user/user.h"
#include "engine/game/game.h"

class IOHandler
{
public:
    virtual ~IOHandler() = default;

    // Methods to handle user data
    virtual QList<User> readUsers() const = 0;
    virtual void writeUsers(const QList<User>& users) const = 0;

    // Methods to handle game data
    virtual QList<Game> readGames() const = 0;
    virtual void writeGames(const QList<Game>& games) const = 0;
};

#endif // IO_HANDLER_H
