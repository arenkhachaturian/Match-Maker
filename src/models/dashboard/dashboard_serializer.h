#ifndef DASHBOARD_SERIALIZER_H
#define DASHBOARD_SERIALIZER_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class DashboardModel;

class DashboardSerializer
{
public:
    static QJsonDocument serializeToJson(const DashboardModel *model, const QString &gameName);
    static QJsonDocument serializeToJson(const DashboardModel *model);
};

#endif // DASHBOARD_SERIALIZER_H
