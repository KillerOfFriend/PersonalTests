#include "TableModel.h"

#include <random>

using namespace Models;

//-----------------------------------------------------------------------------
// TTableItem
//-----------------------------------------------------------------------------
TTableItem::TTableItem(QObject* inParent) : QObject(inParent)
{

}
//-----------------------------------------------------------------------------
TTableItem::~TTableItem()
{

}
//-----------------------------------------------------------------------------
void TTableItem::setItemID(quint64 inID) // Метод задаст ID айтема
{ fID = inID; }
//-----------------------------------------------------------------------------
quint64 TTableItem::itemID() // Метод вернёт ID айтема
{ return fID; }
//-----------------------------------------------------------------------------
void TTableItem::setItemName(QString inName) // Метод задаст имя айтема
{ fName = inName; }
//-----------------------------------------------------------------------------
QString TTableItem::itemName() // Метод вернёт имя айтема
{ return fName; }
//-----------------------------------------------------------------------------
void TTableItem::setItemDateTime(QDateTime inDateTime) // Метод задаст время\дату айтема
{ fDateTime = inDateTime; }
//-----------------------------------------------------------------------------
QDateTime TTableItem::itemDateTime() // Метод вренёт время\дату айтема
{ return fDateTime; }
//-----------------------------------------------------------------------------
void TTableItem::setItemDouble(double inDouble) // Метод задаст Double значение айтема
{ fDouble = inDouble; }
//-----------------------------------------------------------------------------
double TTableItem::itemDouble() // Метод вернёт Double значение айтема
{ return fDouble; }
//-----------------------------------------------------------------------------
// TTableModel
//-----------------------------------------------------------------------------
TTableModel::TTableModel(QObject* inParent) : QAbstractTableModel(inParent)
{
    initHeadersText();
}
//-----------------------------------------------------------------------------
TTableModel::~TTableModel()
{

}
//-----------------------------------------------------------------------------
int TTableModel::rowCount(const QModelIndex &parent) const // Метод вернтёт количество строк (элементов) указанного айтема
{
    Q_UNUSED(parent) // Параметр не используется в табличной модели
    return this->size(); // Возвращаем количество строк
}
//-----------------------------------------------------------------------------
int TTableModel::columnCount(const QModelIndex &parent) const // Метод вернёт колчиестов столбцов указанного айтема
{
    Q_UNUSED(parent) // Параметр не используется в табличной модели
    return static_cast<quint8>(eColIndex::ciColCount); // Возвращаем количество столбцов
}
//-----------------------------------------------------------------------------
QVariant TTableModel::data(const QModelIndex &index, int role) const // Метод вернёт данные по индексу указанного айтема
{
    QVariant Result = QVariant(); // Возвращаемый результат

    if (!index.isValid() ||  // Если индекс не валиден
            ( index.row() < 0 || index.row() >= rowCount() ) || // Или не удовлетворяет
            ( index.column() < 0 || index.column() >= columnCount() )
            ) // ОДЗ
        Result = QVariant();
    else // Индекс валиден
    {
        auto ItemIt = this->begin(); // Получаем итератор на первый элемет в контейенре
        std::advance(ItemIt, index.row()); // Смещаем его на указанную строку

        switch (role) // Проверяем запрашиваемую роль
        {
            case Qt::DisplayRole: // Роль отображения
            {
                switch (static_cast<eColIndex>(index.column())) // Проверяем запрашиваемый столбец
                {
                    case eColIndex::ciID: { Result = ItemIt->second->itemID(); break; } // Возвращаем ID айтема
                    case eColIndex::ciName: { Result = ItemIt->second->itemName(); break; } // Возвращаем имя айтема
                    case eColIndex::ciDateTime: { Result = ItemIt->second->itemDateTime(); break; } // Возвращаем дату\время
                    case eColIndex::ciDouble: { Result = ItemIt->second->itemDouble(); break; } // Возвращаем  Duble значение

                    default: Result = QVariant(); // Во всех остальных случаях возвращаем невалидный QVariant
                }
                break;
            }
            default: Result = QVariant(); // Во всех остальных случаях возвращаем невалидный QVariant
        }
    }

    return Result; // Возвращаем полученный результа
}
//-----------------------------------------------------------------------------
QVariant TTableModel::headerData(int section, Qt::Orientation orientation, int role) const // Метод вернёт данные для указанного столбца
{
    QVariant Result = QVariant(); // Возвращаемый результат

    if (section < columnCount()) // Если столбец в допустимом диапозоне
        if (role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal) // Если роль "Отображдение" и ориентация колонок "горизонтальная"
            Result = fHeadersText[section]; // Возвращаем заголовок указанного стоолбца

    return Result; // Возвращаем полученный результа
}
//-----------------------------------------------------------------------------
bool TTableModel::init() // Метод инициализирует модель
{
    std::random_device RandomDevice;
    std::mt19937 RandomEngen(RandomDevice());

    std::uniform_int_distribution<quint8> ItemDist(1,20);
    std::uniform_int_distribution<quint64> IdDist(1,10000);
    std::uniform_real_distribution<double> DoubleDist(1.0, 100.0);

    quint8 ItemsCount = ItemDist(RandomEngen);

    for (quint8 Index = 0; Index < ItemsCount; ++Index)
    {
        std::shared_ptr<TTableItem> NewItem = std::make_shared<TTableItem>();
        NewItem->setItemID(IdDist(RandomEngen));
        NewItem->setItemName("Item" + QString::number(Index));
        NewItem->setItemDateTime(QDateTime::currentDateTime());
        NewItem->setItemDouble(DoubleDist(RandomEngen));

        this->insert(std::make_pair(NewItem->itemID(), NewItem));
    }
}
//-----------------------------------------------------------------------------
void TTableModel::initHeadersText() // Метод инициализирует заголовки
{   // Инициализируем текст заголовков
    fHeadersText[static_cast<quint8>(eColIndex::ciID)] = "ID";
    fHeadersText[static_cast<quint8>(eColIndex::ciName)] = "Имя";
    fHeadersText[static_cast<quint8>(eColIndex::ciDateTime)] = "Время/Дата";
    fHeadersText[static_cast<quint8>(eColIndex::ciDouble)] = "Double значение";
}
//-----------------------------------------------------------------------------
