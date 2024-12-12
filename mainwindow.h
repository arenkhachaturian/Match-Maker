#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewers/user_table_widget.h"
#include "viewers/dashboard_widget.h"
#include "viewers/app_toolbar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupWindow(UserTableWidget* userTableWidget, DashboardWidget* dashboardWidget, AppToolbar* toolbar);

private:
    Ui::MainWindow *ui;
    UserTableWidget* m_userTableWidget;
    DashboardWidget* m_dashboardWidget;
    AppToolbar*         m_toolbar;

    void setupConnections();
};
#endif // MAINWINDOW_H
