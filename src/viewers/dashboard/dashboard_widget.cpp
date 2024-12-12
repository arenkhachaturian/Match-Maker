#include "dashboard_widget.h"
#include "models/dashboard/dashboard_serializer.h"
#include <QMenu>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMessageBox>
#include <QVBoxLayout>
#include "dashboard_widget.h"
#include "models/dashboard/dashboard_model.h"

DashboardWidget::DashboardWidget(DashboardModel* model, QWidget* parent)
    : QWidget(parent), m_model(model) {
    m_treeView = new QTreeView(this);

    // Explicitly cast model to QAbstractItemModel
    m_treeView->setModel(static_cast<QAbstractItemModel*>(model));

    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeView, &QTreeView::customContextMenuRequested, this, &DashboardWidget::showContextMenu);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_treeView);
    setLayout(layout);
}


void DashboardWidget::showContextMenu(const QPoint& pos) {
    QModelIndex index = m_treeView->indexAt(pos);

    // Ensure context menu is shown only for game elements (column 0, top-level items)
    if (!index.isValid() || index.parent().isValid()) {
        return;
    }

    QString gameName = m_model->data(index, Qt::DisplayRole).toString();

    QMenu contextMenu;
    QAction* saveAction = contextMenu.addAction("Save to File");

    // Pass the game name directly as a parameter to the slot
    connect(saveAction, &QAction::triggered, this, [this, gameName]() {
        saveGameRatingsToFile(gameName);
    });

    contextMenu.exec(m_treeView->viewport()->mapToGlobal(pos));
}

void DashboardWidget::saveDashboardToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Dashboard", "dashboard.json", "JSON Files (*.json)");
    QJsonDocument jsonDoc = DashboardSerializer::serializeToJson(m_model);
    if (jsonDoc.isEmpty()) {
        QMessageBox::warning(this, "Error", "Dashboard is empty");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    QMessageBox::information(this, "Success", "Dashboard saved successfully to " + fileName);
}

void DashboardWidget::saveGameRatingsToFile(const QString& gameName) {
    QString fileName = QFileDialog::getSaveFileName(this, "Save Ratings", gameName + ".json", "JSON Files (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    QJsonDocument jsonDoc = DashboardSerializer::serializeToJson(m_model, gameName);
    if (jsonDoc.isEmpty()) {
        QMessageBox::warning(this, "Error", "No ratings found for the selected game.");
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing.");
        return;
    }

    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    QMessageBox::information(this, "Success", "Ratings saved successfully to " + fileName);
}
