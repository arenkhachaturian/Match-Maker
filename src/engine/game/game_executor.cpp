#include "game_executor.h"
#include <QDebug>
#include <QThread>


void GameExecutor::runGame(const Game* game, const QMap<QString, QString>& namedArgs) {
    emit gameStarted(game->getName());
    if (!game->getExecutablePath().isEmpty()) {
        runExecutableGame(game, namedArgs);
    } else {
        // TODO this is incorrect
        emit gameFinished(game->getName(), GameResult{QStringList{}, QString{}, false});
    }
}

QStringList GameExecutor::convertNamedArgsToCommandLine(const QMap<QString, QString>& namedArgs) const {
    QStringList args;
    for (auto it = namedArgs.begin(); it != namedArgs.end(); ++it) {
        args << "--" + it.key() << it.value();
    }
    return args;
}

void GameExecutor::runExecutableGame(const Game* game, const QMap<QString, QString>& namedArgs) {
    // Validate game executable path
    if (game->getExecutablePath().isEmpty()) {
        emit gameFinished(game->getName(), GameResult{{}, "", false});
        qWarning() << "Game executable path is empty for game:" << game->getName();
        return;
    }

    // Launch the process in a dedicated thread
    QString gameName = game->getName();
    QString execPath = game->getExecutablePath();
    QThread* thread = QThread::create([this, gameName, execPath, namedArgs]() {
        QStringList args = convertNamedArgsToCommandLine(namedArgs);
        QProcess process;
        process.start(execPath, args);

        // Check if the process starts successfully
        if (!process.waitForStarted()) {
            emit gameFinished(gameName, GameResult{{}, "", false});
            qWarning() << "Failed to start game process for game:" << gameName;
            return;
        }

        // Wait for the process to finish
        if (!process.waitForFinished()) {
            emit gameFinished(gameName, GameResult{{}, "", false});
            qWarning() << "Game process did not finish for game:" << gameName;
            return;
        }

        // Read outputs
        QByteArray output = process.readAllStandardOutput();
        QByteArray errorOutput = process.readAllStandardError();
        qDebug() << "Standard Output:" << output;
        qDebug() << "Standard Error:" << errorOutput;

        QString resultString = QString::fromUtf8(output).trimmed();
        QStringList resultTokens = resultString.split(' ', Qt::SkipEmptyParts);

        // Construct GameResult
        GameResult gameResult;
        gameResult.success = true;

        // Collect players from named arguments
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

        // Emit the gameFinished signal
        emit gameFinished(gameName, gameResult);
    });

    // Start the thread
    thread->start();

    // Auto-delete thread after execution
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}
