#include "ModelWidget.h"
#include "ui_ModelWidget.h"

//-----------------------------------------------------------------------------
TModelWidget::TModelWidget(QWidget *inParent) :
    QWidget(inParent),
    ui(new Ui::TModelWidget)
{
    ui->setupUi(this);
}
//-----------------------------------------------------------------------------
TModelWidget::~TModelWidget()
{
    delete ui;
}
//-----------------------------------------------------------------------------
QTreeView* TModelWidget::getTreeView()
{ return ui->treeView; }
//-----------------------------------------------------------------------------
QTableView* TModelWidget::getTableView()
{ return ui->tableView; }
//-----------------------------------------------------------------------------
QListView* TModelWidget::getListView()
{ return ui->listView; }
//-----------------------------------------------------------------------------
