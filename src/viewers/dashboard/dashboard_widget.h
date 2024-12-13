#ifndef DASHBOARD_WIDGET_H
#define DASHBOARD_WIDGET_H

#include <QWidget>
#include <QTreeView>

class DashboardModel;

class DashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWidget(DashboardModel *model, QWidget *parent = nullptr);
    void saveDashboardToFile();

private slots:
    void showContextMenu(const QPoint &pos);
    void saveGameRatingsToFile(const QString &gameName);

private:
    QTreeView *m_treeView;
    DashboardModel *m_model;
};

#endif // DASHBOARD_WIDGET_H
