#ifndef JSON_IO_HANDLER_H
#define JSON_IO_HANDLER_H

#include "io_handler.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class JsonIOHandler final : public IOHandler
{
public:
    explicit JsonIOHandler(const QString &filePath);

    QList<User> readUsers() const override;
    void writeUsers(const QList<User> &users) const override;

    QList<Game> readGames() const override;
    void writeGames(const QList<Game> &games) const override;

private:
    QString m_filePath;
};

#endif // JSON_IO_HANDLER_H
