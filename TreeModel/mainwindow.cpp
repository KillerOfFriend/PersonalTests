#include "mainwindow.h"
#include "ui_mainwindow.h"


//-----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initTreeModel();
    initTableModel();
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void MainWindow::initTreeModel()
{
    AbstractTreeModel.reset(new Models::TTreeModel(this));
    AbstractTreeModel->init(); // Инициализация по корневому ID


    QTreeView* TreeViewPtr = ui->AbstractModelsWidget->getTreeView();
    TreeViewPtr->setModel(AbstractTreeModel.get()); // Задаём модель отображения
    TreeViewPtr->header()->setSectionResizeMode(static_cast<quint8>(Models::TTreeModel::eColIndex::ciText), QHeaderView::ResizeToContents); // Ширина столбца текста по содержимому
    TreeViewPtr->header()->setSectionResizeMode(static_cast<quint8>(Models::TTreeModel::eColIndex::ciType), QHeaderView::ResizeToContents); // Ширина столбца типа по содержимому
    //ui->treeView->setHeaderHidden(false); // Скрываем отображение шапки
    TreeViewPtr->setColumnHidden(static_cast<quint8>(Models::TTreeModel::eColIndex::ciID), true); // Скрыть отображение столбца ID
}
//-----------------------------------------------------------------------------
void MainWindow::initTableModel()
{
    AbstractTableModel.reset(new Models::TTableModel(this));
    AbstractTableModel->init();

    QTableView* TableView = ui->AbstractModelsWidget->getTableView();
    TableView->setModel(AbstractTableModel.get()); // Задаём модель отображения
    TableView->horizontalHeader()->setSectionResizeMode(static_cast<quint8>(Models::TTableModel::eColIndex::ciID), QHeaderView::ResizeToContents); // Ширина столбца текста по содержимому
    TableView->horizontalHeader()->setSectionResizeMode(static_cast<quint8>(Models::TTableModel::eColIndex::ciName), QHeaderView::ResizeToContents); // Ширина столбца типа по содержимому
    TableView->horizontalHeader()->setSectionResizeMode(static_cast<quint8>(Models::TTableModel::eColIndex::ciDateTime), QHeaderView::ResizeToContents); // Ширина столбца типа по содержимому
    TableView->horizontalHeader()->setSectionResizeMode(static_cast<quint8>(Models::TTableModel::eColIndex::ciDouble), QHeaderView::ResizeToContents); // Ширина столбца типа по содержимому

}
//-----------------------------------------------------------------------------
