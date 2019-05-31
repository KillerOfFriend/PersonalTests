#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>

#include "Models/TreeModel.h"
#include "Models/TableModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<Models::TTreeModel> AbstractTreeModel = nullptr; // Указатель на модель дерева
    std::unique_ptr<Models::TTableModel> AbstractTableModel = nullptr; // Указатель на табличную модель

    void initTreeModel();
    void initTableModel();
};

#endif // MAINWINDOW_H
