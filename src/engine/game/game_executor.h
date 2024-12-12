#ifndef GAME_EXECUTOR_H
#define GAME_EXECUTOR_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QProcess>
#include <iostream>
#include <QMutex>
#include "game.h"

struct GameResult {
    QStringList players;  // Players involved in the game
    QString winner;       // Winner's name (empty if it's a draw)
    bool success;         // Whether the game ran successfully
};

class GameExecutor : public QObject {
    Q_OBJECT

public:

    // Runs the given game with the specified named arguments
    void runGame(const Game* game, const QMap<QString, QString>& namedArgs);

    // Setter for the output stream
    void setOutputStream(std::ostream& outputStream);

signals:
    void gameStarted(const QString& gameName);
    void gameFinished(const QString& gameName, const GameResult& result);

private:
    void runExecutableGame(const Game* game, const QMap<QString, QString>& namedArgs);
    QStringList convertNamedArgsToCommandLine(const QMap<QString, QString>& namedArgs) const;

};

#endif // GAME_EXECUTOR_H
