#ifndef USER_H
#define USER_H

#include <QString>
#include <QSet>
#include <QMap>

class User
{
public:
    explicit User(const QString& username);
    User(const QString& username,
         const QString& firstName,
         const QString& lastName,
         const QSet<QString>& preferredGames,
         const QMap<QString, int>& ratings);

    const QString& getUsername() const;
    const QString& getFirstName() const;
    const QString& getLastName() const;
    const QSet<QString>& getPreferredGames() const;
    const QMap<QString, int>& getRatings() const;

    void setFirstName(const QString& firstName);
    void setLastName(const QString& lastName);
    void setPreferredGames(const QSet<QString>& games);
    void setRatings(const QMap<QString, int>& ratings);

    void addPreferredGame(const QString& gameName);
    void removePreferredGame(const QString& gameName);

    void updateRating(const QString& gameName, int rating);
    int getRating(const QString& gameName) const;

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
