cmake_minimum_required(VERSION 3.16)

project(MatchMakingApp VERSION 0.1 LANGUAGES CXX)

# Enable Qt-specific features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

include_directories(src)

enable_testing()

# Define reusable function for test setup
function(add_test_executable TEST_NAME SOURCES)
    add_executable(${TEST_NAME} ${SOURCES})
    target_link_libraries(${TEST_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# Define engine sources
set(ENGINE_SOURCES
    src/engine/user/user.cpp src/engine/user/user.h
    src/engine/game/game.cpp src/engine/game/game.h
    src/engine/io/io_handler.h
    src/engine/io/json_io_handler.cpp src/engine/io/json_io_handler.h
    src/engine/pool/user_pool_manager.cpp src/engine/pool/user_pool_manager.h
    src/engine/game/game_executor.cpp src/engine/game/game_executor.h
    src/engine/user/user_registry.cpp src/engine/user/user_registry.h
    src/engine/matchmaking/matchmaking_engine.cpp src/engine/matchmaking/matchmaking_engine.h
    src/engine/game/game_registry.cpp src/engine/game/game_registry.h
)

# Define model sources
set(MODEL_SOURCES
    src/models/user_table/user_table_model.cpp src/models/user_table/user_table_model.h
    src/models/dashboard/dashboard_model.cpp src/models/dashboard/dashboard_model.h
    src/models/dashboard/dashboard_serializer.cpp src/models/dashboard/dashboard_serializer.h
)

# Define viewer sources
set(VIEWER_SOURCES
    src/viewers/user_table/user_table_widget.cpp src/viewers/user_table/user_table_widget.h
    src/viewers/user_table/user_table_context_menu.cpp src/viewers/user_table/user_table_context_menu.h
    src/viewers/dashboard/dashboard_widget.cpp src/viewers/dashboard/dashboard_widget.h
    src/viewers/dialogs/add_user_dialog.cpp src/viewers/dialogs/add_user_dialog.h
    src/viewers/dialogs/remove_user_dialog.cpp src/viewers/dialogs/remove_user_dialog.h
    src/viewers/toolbar/app_toolbar.cpp src/viewers/toolbar/app_toolbar.h
    src/viewers/mainwindow/mainwindow.cpp src/viewers/mainwindow/mainwindow.h
)

# UI Files
set(UI_FILES src/viewers/mainwindow/mainwindow.ui)

# Define test cases
add_test_executable(test_engine
    "src/engine/user/user.cpp;tests/test_user.cpp"
)

add_test_executable(test_game
    "src/engine/game/game.cpp;tests/test_game.cpp"
)

add_test_executable(test_json_io
    "src/engine/io/json_io_handler.cpp;src/engine/user/user.cpp;src/engine/game/game.cpp;tests/test_json_io.cpp"
)

add_test_executable(test_user_pool_mgr
    "${ENGINE_SOURCES};tests/test_user_pool_mgr.cpp"
)

add_test_executable(test_game_executor
    "${ENGINE_SOURCES};tests/test_game_executor.cpp"
)

add_test_executable(test_user_registry
    "${ENGINE_SOURCES};tests/test_user_registry.cpp"
)

add_test_executable(test_matchmaking_engine
    "${ENGINE_SOURCES};tests/test_matchmaking_engine.cpp"
)

# Application sources
set(PROJECT_SOURCES
    ${ENGINE_SOURCES}
    ${MODEL_SOURCES}
    ${VIEWER_SOURCES}
    ${UI_FILES}
)

# Configure main application
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
else()
    if(ANDROID)
        add_library(MatchMakingApp SHARED ${PROJECT_SOURCES})
    else()
        add_executable(MatchMakingApp main.cpp ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(MatchMakingApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Bundle and installation settings
set_target_properties(MatchMakingApp PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MatchMakingApp
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
