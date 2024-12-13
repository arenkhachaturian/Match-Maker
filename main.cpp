#include "viewers/mainwindow/mainwindow.h"
#include "viewers/user_table/user_table_widget.h"
#include "viewers/toolbar/app_toolbar.h"
#include "models/user_table/user_table_model.h"
#include "engine/user/user_registry.h"
#include "engine/game/game_registry.h"
#include "models/dashboard/dashboard_model.h"
#include "viewers/dashboard/dashboard_widget.h"
#include "engine/pool/user_pool_manager.h"
#include "engine/matchmaking/matchmaking_engine.h"
#include "engine/io/json_io_handler.h"
#include <QApplication>
#include <QSplitter>
#include <QDir>
#include <QRandomGenerator>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    QString filePath = QDir::toNativeSeparators(QDir::cleanPath("D:/projects/MatchMakingApp/data.json"));

    JsonIOHandler ioHandler(filePath);

    auto userRegistry = std::make_unique<UserRegistry>();
    auto gameRegistry = std::make_unique<GameRegistry>();

    auto users = ioHandler.readUsers();
    auto games = ioHandler.readGames();

    for (const auto &user : users)
    {
        userRegistry->addUser(user);
    }
    for (const auto &game : games)
    {
        gameRegistry->addGame(game);
    }

    auto userPoolManager = std::make_unique<UserPoolManager>();
    for (const auto &user : users)
    {
        userPoolManager->addUser(user.getUsername());
    }

    auto matchmakingEngine = std::make_unique<MatchmakingEngine>(
        userPoolManager.get(),
        userRegistry.get(),
        gameRegistry.get(),
        new GameExecutor);

    auto dashboardModel = std::make_unique<DashboardModel>(gameRegistry.get(), userRegistry.get());
    auto userTableModel = std::make_unique<UserTableModel>(userRegistry.get());

    auto dashboardWidget = new DashboardWidget(dashboardModel.get());
    auto userTableWidget = new UserTableWidget(userTableModel.get(), dashboardModel.get());

    auto appToolbar = new AppToolbar;

    w.setupWindow(userTableWidget, dashboardWidget, appToolbar);
    w.resize(1200, 600);
    w.show();

    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [&]()
                     {
        auto users = userRegistry->getAllUsers();

        if (!users.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(users.size());
            const QString& username = users[randomIndex].getUsername();
            qDebug() << "Requesting match for user:" << username;
            matchmakingEngine->requestMatch(username);
        } else {
            qDebug() << "No users available in the registry.";
        } });

    timer->start(2000);

    matchmakingEngine->start(2000);

    return a.exec();
}
