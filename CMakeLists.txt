cmake_minimum_required(VERSION 3.16)

project(MatchMakingApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(src)
enable_testing()
find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(test_engine
    src/engine/user/user.h
    src/engine/user/user.cpp

    tests/test_user.cpp)


# Link the Qt6 Test module to the executable
target_link_libraries(test_engine PRIVATE Qt6::Test)

# Add the test to CTest
add_test(NAME TestEngine COMMAND test_engine)


add_executable(test_game  tests/test_game.cpp src/engine/game/game.cpp)
target_link_libraries(test_game PRIVATE Qt6::Test)
add_test(NAME TestGame COMMAND test_game)

add_executable(test_json_io tests/test_json_io.cpp src/engine/io/json_io_handler.cpp src/engine/user/user.cpp src/engine/game/game.cpp)
target_link_libraries(test_json_io PRIVATE Qt6::Test)
add_test(NAME TestJsonIo COMMAND test_json_io)

set(ENGINE_SOURCES
    src/engine/user/user.cpp
    src/engine/user/user.h
    src/engine/game/game.cpp
    src/engine/game/game.h
    src/engine/io/io_handler.h
    src/engine/io/json_io_handler.h
    src/engine/io/json_io_handler.cpp
    src/engine/pool/user_pool_manager.cpp
    src/engine/pool/user_pool_manager.h
    src/engine/game/game_executor.h
    src/engine/game/game_executor.cpp
    src/engine/user/user_registry.h
    src/engine/user/user_registry.cpp
    src/engine/matchmaking/matchmaking_engine.h
    src/engine/matchmaking/matchmaking_engine.cpp
    src/engine/game/game_registry.cpp
    src/engine/game/game_registry.h
)

add_executable(test_user_pool_mgr  tests/test_user_pool_mgr.cpp ${ENGINE_SOURCES})
target_link_libraries(test_user_pool_mgr PRIVATE Qt6::Test)
add_test(NAME TestUserPoolMgr COMMAND test_user_pool_mgr)

add_executable(test_game_executor  tests/test_game_executor.cpp ${ENGINE_SOURCES})
target_link_libraries(test_game_executor PRIVATE Qt6::Test)
add_test(NAME TestGameExecutor COMMAND test_game_executor)

add_executable(test_user_registry tests/test_user_registry.cpp ${ENGINE_SOURCES})
target_link_libraries(test_user_registry PRIVATE Qt6::Test)
add_test(NAME TestUserRegistr COMMAND test_user_registry)

add_executable(test_matchmaking_engine tests/test_matchmaking_engine.cpp ${ENGINE_SOURCES})
target_link_libraries(test_matchmaking_engine PRIVATE Qt6::Test)
add_test(NAME TestMatchMakingEngine COMMAND test_matchmaking_engine)


set(MODEL_SOURCES
    src/models/user_table/user_table_model.h
    src/models/user_table/user_table_model.cpp
    src/models/dashboard/dashboard_model.cpp
    src/models/dashboard/dashboard_model.h
    src/models/dashboard/dashboard_serializer.h
    src/models/dashboard/dashboard_serializer.cpp)

set(VIEWER_SOURCES
    src/viewers/user_table/user_table_widget.h
    src/viewers/user_table/user_table_widget.cpp
    src/viewers/user_table/user_table_context_menu.h
    src/viewers/user_table/user_table_context_menu.cpp
    src/viewers/dashboard/dashboard_widget.h
    src/viewers/dashboard/dashboard_widget.cpp
    src/viewers/dialogs/add_user_dialog.cpp
    src/viewers/dialogs/add_user_dialog.h
    src/viewers/dialogs/remove_user_dialog.cpp
    src/viewers/dialogs/remove_user_dialog.h
    src/viewers/toolbar/app_toolbar.h
    src/viewers/toolbar/app_toolbar.cpp
    src/viewers/mainwindow/mainwindow.h
    src/viewers/mainwindow/mainwindow.cpp)

set(UI_FILES src/viewers/mainwindow/mainwindow.ui)

set(PROJECT_SOURCES
    ${ENGINE_SOURCES}
    ${MODEL_SOURCES}
    ${VIEWER_SOURCES}
    ${UI_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MatchMakingApp
        MANUAL_FINALIZATION
        main.cpp
        ${PROJECT_SOURCES}
        tests/games/game_for_test.py
        tests/games/game.bat
        tests/games/always_first_wins.py
        tests/games/afw.bat
        data.json
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MatchMakingApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MatchMakingApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MatchMakingApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MatchMakingApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MatchMakingApp)
endif()
set_target_properties(MatchMakingApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MatchMakingApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MatchMakingApp)
endif()
