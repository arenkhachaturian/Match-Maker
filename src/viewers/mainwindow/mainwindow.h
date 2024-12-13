#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "viewers/user_table/user_table_widget.h"
#include "viewers/dashboard/dashboard_widget.h"
#include "viewers/toolbar/app_toolbar.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupWindow(UserTableWidget *userTableWidget, DashboardWidget *dashboardWidget, AppToolbar *toolbar);

private:
    Ui::MainWindow *ui;
    UserTableWidget *m_userTableWidget;
    DashboardWidget *m_dashboardWidget;
    AppToolbar *m_toolbar;

    void setupConnections();
};
#endif // MAINWINDOW_H
