#ifndef USER_H
#define USER_H

#include <QString>
#include <QSet>
#include <QMap>

class User
{
public:
    // Constructors
    explicit User(const QString& username);
    User(const QString& username,
         const QString& firstName,
         const QString& lastName,
         const QSet<QString>& preferredGames,
         const QMap<QString, int>& ratings);

    // Getters
    const QString& getUsername() const;
    const QString& getFirstName() const;
    const QString& getLastName() const;
    const QSet<QString>& getPreferredGames() const;
    const QMap<QString, int>& getRatings() const;

    // Setters
    void setFirstName(const QString& firstName);
    void setLastName(const QString& lastName);
    void setPreferredGames(const QSet<QString>& games);
    void setRatings(const QMap<QString, int>& ratings);

    // Add/Remove games
    void addPreferredGame(const QString& gameName);
    void removePreferredGame(const QString& gameName);

    // Ratings management
    void updateRating(const QString& gameName, int rating);
    int getRating(const QString& gameName) const;

    // operators
    bool operator==(const User& other) const;

private:
    QString m_username;
    QString m_firstName;
    QString m_lastName;
    QSet<QString> m_preferredGames;
    QMap<QString, int> m_ratings;

    friend uint qHash(const User& user, uint seed);
};

inline uint qHash(const User& user, uint seed = 0) {
    return qHash(user.getUsername(), seed);
}

#endif // USER_H
