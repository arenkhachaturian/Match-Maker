#include "user.h"

// Constructor with only username
User::User(const QString& username)
    : m_username(username), m_firstName(""), m_lastName("")
{}

// Constructor with username, first name, last name, preferred games, and ratings
User::User(const QString& username,
           const QString& firstName,
           const QString& lastName,
           const QSet<QString>& preferredGames,
           const QMap<QString, int>& ratings)
    : m_username(username),
    m_firstName(firstName),
    m_lastName(lastName),
    m_preferredGames(preferredGames),
    m_ratings(ratings)
{}

// Getters
const QString& User::getUsername() const {
    return m_username;
}

const QString& User::getFirstName() const {
    return m_firstName;
}

const QString& User::getLastName() const {
    return m_lastName;
}

const QSet<QString>& User::getPreferredGames() const {
    return m_preferredGames;
}

const QMap<QString, int>& User::getRatings() const {
    return m_ratings;
}

// Setters
void User::setFirstName(const QString& firstName) {
    m_firstName = firstName;
}

void User::setLastName(const QString& lastName) {
    m_lastName = lastName;
}

void User::setPreferredGames(const QSet<QString>& games) {
    m_preferredGames = games;
}

void User::setRatings(const QMap<QString, int>& ratings) {
    m_ratings = ratings;
}

// Add/Remove games
void User::addPreferredGame(const QString& gameName) {
    m_preferredGames.insert(gameName); // QSet ensures uniqueness
}

void User::removePreferredGame(const QString& gameName) {
    m_preferredGames.remove(gameName);
}

// Ratings management
void User::updateRating(const QString& gameName, int rating) {
    m_ratings[gameName] = rating;
}

int User::getRating(const QString& gameName) const {
    // Return the rating for the game, or 0 if the game has no rating
    return m_ratings.value(gameName, 0);
}

bool User::operator==(const User& other) const {
    return this->getUsername() == other.getUsername();
}