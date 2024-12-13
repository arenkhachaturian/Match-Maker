#include "game_executor.h"
#include <QDebug>
#include <QThread>

void GameExecutor::runGame(const Game &game, const QMap<QString, QString> &namedArgs)
{
    emit gameStarted(game.getName());
    if (!game.getExecutablePath().isEmpty())
    {
        runExecutableGame(game, namedArgs);
    }
    else
    {
        // TODO refactor
        emit gameFinished(game.getName(), GameResult{QStringList{}, QString{}, false});
    }
}

QStringList GameExecutor::convertNamedArgsToCommandLine(const QMap<QString, QString> &namedArgs) const
{
    QStringList args;
    for (auto it = namedArgs.begin(); it != namedArgs.end(); ++it)
    {
        args << "--" + it.key() << it.value();
    }
    return args;
}

void GameExecutor::runExecutableGame(const Game &game, const QMap<QString, QString> &namedArgs)
{
    if (game.getExecutablePath().isEmpty())
    {
        emit gameFinished(game.getName(), GameResult{{}, "", false});
        qWarning() << "Game executable path is empty for game:" << game.getName();
        return;
    }

    QString gameName = game.getName();
    QString execPath = game.getExecutablePath();
    QThread *thread = QThread::create([this, gameName, execPath, namedArgs]()
                                      {
        QStringList args = convertNamedArgsToCommandLine(namedArgs);
        QProcess process;
        process.start(execPath, args);

        if (!process.waitForStarted()) {
            emit gameFinished(gameName, GameResult{{}, "", false});
            qWarning() << "Failed to start game process for game:" << gameName;
            return;
        }

        if (!process.waitForFinished()) {
            emit gameFinished(gameName, GameResult{{}, "", false});
            qWarning() << "Game process did not finish for game:" << gameName;
            return;
        }

        QByteArray output = process.readAllStandardOutput();
        QByteArray errorOutput = process.readAllStandardError();
        QString resultString = QString::fromUtf8(output).trimmed();
        QStringList resultTokens = resultString.split(' ', Qt::SkipEmptyParts);

        GameResult gameResult;
        gameResult.success = true;

        for (auto it = namedArgs.constBegin(); it != namedArgs.constEnd(); ++it) {
            if (it.key().contains("player")) {
                gameResult.players << it.value();
            }
        }

        // Determine the winner
        if (resultTokens.size() == 1) {
            gameResult.winner = resultTokens[0]; // Single name -> Winner
        } else if (resultTokens.size() > 1) {
            gameResult.winner = ""; // Multiple names -> Draw
        } else {
            qWarning() << "Unexpected game result format for game:" << gameName;
        }

        emit gameFinished(gameName, gameResult); });

    thread->start();

    // Auto-delete thread after execution
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}
