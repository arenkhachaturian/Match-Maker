#ifndef GAME_REGISTRY_H
#define GAME_REGISTRY_H

#include <QObject>
#include <QString>
#include <QSet>
#include <memory>
#include "engine/game/game.h"

class GameRegistry : public QObject {
    Q_OBJECT

public:
    explicit GameRegistry(QObject* parent = nullptr);

    bool addGame(const Game& newGame);
    bool removeGame(const QString& gameName);

    std::shared_ptr<const Game> getGame(const QString& name) const;

    // Get all games
    const QSet<Game>& getAllGames() const;

signals:
    void gameAdded(const QString& name);
    void gameRemoved(const QString& name);

private:
    QSet<Game> m_games; // QSet of Game objects
};

#endif // GAME_REGISTRY_H
