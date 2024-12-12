#include "app_toolbar.h"
#include <QMenu>
#include <QAction>
#include <QToolBar>

#include "app_toolbar.h"
#include <QToolButton>
#include <QMenu>

AppToolbar::AppToolbar(QWidget* parent) : QToolBar(parent) {
    setupActions();
}

void AppToolbar::setupActions() {
    // File menu
    QMenu* fileMenu = new QMenu("File", this);
    QAction* saveDashboardAction = new QAction("Save Dashboard", this);
    connect(saveDashboardAction, &QAction::triggered, this, &AppToolbar::saveDashboardRequested);
    fileMenu->addAction(saveDashboardAction);

    QToolButton* fileButton = new QToolButton(this);
    fileButton->setText("File");
    fileButton->setMenu(fileMenu);
    fileButton->setPopupMode(QToolButton::InstantPopup);
    addWidget(fileButton); // Add the File menu to the toolbar

    // Edit menu
    QMenu* editMenu = new QMenu("Edit", this);
    QAction* addUserAction = new QAction("Add User", this);
    QAction* removeUserAction = new QAction("Remove User", this);
    connect(addUserAction, &QAction::triggered, this, &AppToolbar::addUserRequested);
    connect(removeUserAction, &QAction::triggered, this, &AppToolbar::removeUserRequested);
    editMenu->addAction(addUserAction);
    editMenu->addAction(removeUserAction);

    QToolButton* editButton = new QToolButton(this);
    editButton->setText("Edit");
    editButton->setMenu(editMenu);
    editButton->setPopupMode(QToolButton::InstantPopup);
    addWidget(editButton); // Add the Edit menu to the toolbar

    // View menu
    QMenu* viewMenu = new QMenu("View", this);
    QAction* toggleDashboardAction = new QAction("Show/Hide Dashboard", this);
    QAction* toggleUserListAction = new QAction("Show/Hide User List", this);
    connect(toggleDashboardAction, &QAction::triggered, this, &AppToolbar::toggleDashboardViewRequested);
    connect(toggleUserListAction, &QAction::triggered, this, &AppToolbar::toggleUserListViewRequested);
    viewMenu->addAction(toggleDashboardAction);
    viewMenu->addAction(toggleUserListAction);

    QToolButton* viewButton = new QToolButton(this);
    viewButton->setText("View");
    viewButton->setMenu(viewMenu);
    viewButton->setPopupMode(QToolButton::InstantPopup);
    addWidget(viewButton); // Add the View menu to the toolbar
}
