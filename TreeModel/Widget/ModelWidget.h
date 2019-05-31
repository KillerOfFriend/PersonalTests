#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QTableView>
#include <QListView>

namespace Ui {
class TModelWidget;
}

class TModelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TModelWidget(QWidget *inParent = nullptr);
    ~TModelWidget();

    QTreeView* getTreeView();
    QTableView* getTableView();
    QListView* getListView();

private:
    Ui::TModelWidget *ui;
};

#endif // MODELWIDGET_H
