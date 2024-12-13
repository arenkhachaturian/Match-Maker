#include "engine/game/game_registry.h"
#include <stdexcept>
#include <string>

GameRegistry::GameRegistry(QObject *parent)
    : QObject(parent) {}

bool GameRegistry::addGame(const Game &newGame)
{
    if (m_games.contains(newGame))
    {
        return false;
    }
    m_games.insert(newGame);
    emit gameAdded(newGame.getName());
    return true;
}

bool GameRegistry::removeGame(const QString &gameName)
{
    Game tmp(gameName);
    if (m_games.remove(tmp))
    {
        emit gameRemoved(gameName);
        return true;
    }
    return false;
}

const Game& GameRegistry::getGame(const QString &name) const
{
    for (const auto &game : m_games)
    {
        if (game.getName() == name)
        {
            return game;
        }
    }
    throw std::runtime_error(std::string("Game ") + name.toStdString() + " not found");
}

const QSet<Game> &GameRegistry::getAllGames() const
{
    return m_games;
}
