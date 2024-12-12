#include "json_io_handler.h"
#include "engine/game/game.h"

JsonIOHandler::JsonIOHandler(const QString& filePath)
    : m_filePath(filePath)
{}

QList<User> JsonIOHandler::readUsers() const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for reading:" << m_filePath;
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray usersArray = doc.object().value("users").toArray();

    QList<User> users;
    for (const QJsonValue& value : usersArray) {
        QJsonObject userObj = value.toObject();
        User user(userObj.value("username").toString());

        user.setFirstName(userObj.value("firstName").toString());
        user.setLastName(userObj.value("lastName").toString());

        QJsonArray gamesArray = userObj.value("preferredGames").toArray();
        for (const QJsonValue& game : gamesArray) {
            user.addPreferredGame(game.toString());
        }

        QJsonObject ratingsObj = userObj.value("ratings").toObject();
        for (const QString& gameName : ratingsObj.keys()) {
            user.updateRating(gameName, ratingsObj.value(gameName).toInt());
        }

        users.append(user);
    }

    return users;
}

void JsonIOHandler::writeUsers(const QList<User>& users) const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << m_filePath;
        return;
    }

    QJsonArray usersArray;
    for (const User& user : users) {
        QJsonObject userObj;
        userObj["username"] = user.getUsername();
        userObj["firstName"] = user.getFirstName();
        userObj["lastName"] = user.getLastName();

        QJsonArray gamesArray;
        for (const QString& game : user.getPreferredGames()) {
            gamesArray.append(game);
        }
        userObj["preferredGames"] = gamesArray;

        QJsonObject ratingsObj;
        for (const QString& gameName : user.getRatings().keys()) {
            ratingsObj[gameName] = user.getRating(gameName);
        }
        userObj["ratings"] = ratingsObj;

        usersArray.append(userObj);
    }

    QJsonObject rootObj;
    rootObj["users"] = usersArray;

    QJsonDocument doc(rootObj);
    file.write(doc.toJson());
    file.close();
}

QList<Game> JsonIOHandler::readGames() const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for reading:" << m_filePath;
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray gamesArray = doc.object().value("games").toArray();

    QList<Game> games;
    for (const QJsonValue& value : gamesArray) {
        QJsonObject gameObj = value.toObject();
        Game game(gameObj.value("name").toString(), gameObj.value("executablePath").toString());
        games.append(game);
    }

    return games;
}

void JsonIOHandler::writeGames(const QList<Game>& games) const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << m_filePath;
        return;
    }

    QJsonArray gamesArray;
    for (const Game& game : games) {
        QJsonObject gameObj;
        gameObj["name"] = game.getName();
        gamesArray.append(gameObj);
    }

    QJsonObject rootObj;
    rootObj["games"] = gamesArray;

    QJsonDocument doc(rootObj);
    file.write(doc.toJson());
    file.close();
}
