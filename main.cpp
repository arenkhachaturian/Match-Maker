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

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Main window
    MainWindow w;

    // File path for JSON data
    QString filePath = QDir::toNativeSeparators(QDir::cleanPath("D:/projects/MatchMakingApp/data.json"));

    // Initialize IO handler
    JsonIOHandler ioHandler(filePath);

    // Create registries
    auto userRegistry = std::make_unique<UserRegistry>();
    auto gameRegistry = std::make_unique<GameRegistry>();

    // Read users and games from file
    auto users = ioHandler.readUsers();
    auto games = ioHandler.readGames();

    // Populate registries
    for (const auto& user : users) {
        userRegistry->addUser(user);
    }
    for (const auto& game : games) {
        gameRegistry->addGame(game);
    }

    // Create user pool
    auto userPoolManager = std::make_unique<UserPoolManager>();
    for (const auto& user : users) {
        userPoolManager->addUser(user.getUsername());
    }

    // Initialize matchmaking engine
    auto matchmakingEngine = std::make_unique<MatchmakingEngine>(
        userPoolManager.get(),
        userRegistry.get(),
        gameRegistry.get(),
        new GameExecutor // Ensure thread-safe execution
    );

    // Models
    auto dashboardModel = std::make_unique<DashboardModel>(gameRegistry.get(), userRegistry.get());
    auto userTableModel = std::make_unique<UserTableModel>(userRegistry.get());

    // Widgets
    auto dashboardWidget = new DashboardWidget(dashboardModel.get());
    auto userTableWidget = new UserTableWidget(userTableModel.get(), dashboardModel.get());

    // Toolbar
    auto appToolbar = new AppToolbar;

    // Setup main window
    w.setupWindow(userTableWidget, dashboardWidget, appToolbar);
    w.resize(1200, 600); // Default size
    w.show();

    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [&]() {
        auto users = userRegistry->getAllUsers();

        if (!users.isEmpty()) {
            // Pick a random user from the registry
            int randomIndex = QRandomGenerator::global()->bounded(users.size());
            const QString& username = users[randomIndex].getUsername();

            matchmakingEngine->requestMatch(username);

            qDebug() << "Requested match for user:" << username;
        } else {
            qDebug() << "No users available in the registry.";
        }
    });

    // Start the timer, firing every 2 seconds
    timer->start(2000);

    // Start matchmaking engine in the background
    matchmakingEngine->start(100);

    return a.exec();
}
