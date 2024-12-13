#ifndef USER_TABLE_CONTEXT_MENU_H
#define USER_TABLE_CONTEXT_MENU_H

#include <QWidget>
#include <QObject>
#include <QMenu>

class UserTableContextMenu final : public QObject {
    Q_OBJECT

public:
    explicit UserTableContextMenu(QWidget* parent = nullptr);

signals:
    void addUserRequested();
    void removeUserRequested();

private slots:
    void showContextMenu(const QPoint& pos);
    void handleAddUser();
    void handleRemoveUser();

private:
    QMenu* m_contextMenu;
    QAction* m_addUserAction;
    QAction* m_removeUserAction;
};

#endif // USER_TABLE_CONTEXT_MENU_H
