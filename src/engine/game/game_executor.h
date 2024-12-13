#ifndef GAME_EXECUTOR_H
#define GAME_EXECUTOR_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QProcess>
#include <iostream>
#include <QMutex>
#include "game.h"

struct GameResult
{
    QStringList players;
    QString winner;
    bool success;
};

class GameExecutor final : public QObject
{
    Q_OBJECT

public:
    void runGame(const Game &game, const QMap<QString, QString> &namedArgs);
    void setOutputStream(std::ostream &outputStream);

signals:
    void gameStarted(const QString &gameName);
    void gameFinished(const QString &gameName, const GameResult &result);

private:
    void runExecutableGame(const Game &game, const QMap<QString, QString> &namedArgs);
    QStringList convertNamedArgsToCommandLine(const QMap<QString, QString> &namedArgs) const;
};

#endif // GAME_EXECUTOR_H
