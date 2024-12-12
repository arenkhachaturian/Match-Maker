#ifndef APP_TOOLBAR_H
#define APP_TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QWidget>

class AppToolbar : public QToolBar {
    Q_OBJECT

public:
    explicit AppToolbar(QWidget* parent = nullptr);

signals:
    void saveDashboardRequested();
    void addUserRequested();
    void removeUserRequested();
    void toggleDashboardViewRequested();
    void toggleUserListViewRequested();

private:
    void setupActions();
};

#endif // APP_TOOLBAR_H
