#include "game.h"

Game::Game(const QString& name, const QString& execPath)
    : m_name(name), m_executablePath(execPath) {}

void Game::setExecutable(const QString& executablePath) {
    m_executablePath = executablePath;
}

const QString& Game::getName() const {
    return m_name;
}

const QString& Game::getExecutablePath() const {
    return m_executablePath;
}

bool Game::operator==(const Game& other) const {
    return this->getName() == other.getName();
}
