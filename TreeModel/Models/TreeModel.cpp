#include "TreeModel.h"

#include <random>

#include <QIcon>
#include <QFileIconProvider>

using namespace Models;

//-----------------------------------------------------------------------------
// TreeItem
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::TreeItem - Инициализирующий конструктор
 * @param inID - ID айтема
 * @param inParentItem - Указатель на родительский айтем
 * @param inParentObject - Указатель на объект-предок
 */
TreeItem::TreeItem(quint64 inID, TreeItem *inParentItem, QObject* inParentObject) : QObject(inParentObject), fID(inID), fParentItem(inParentItem)
{
    initChilde();
}
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::~TreeItem - Диструктор
 */
TreeItem::~TreeItem()
{

}
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::initChilde - Метод инициализирует потомков айтема
 */
void TreeItem::initChilde()
{
    if (!fID)
        return;
    else
    {
        quint8 MaxItemLevel = 5;
        quint8 ItemLevel = 0;

        TreeItem* RootPtr = itemParent();

        while(RootPtr)
        {
            RootPtr = RootPtr->itemParent();
            ItemLevel++;
        }

        if (ItemLevel < MaxItemLevel)
        {
            std::random_device RandomDevice;
            std::mt19937 RandomEngen(RandomDevice());

            std::uniform_int_distribution<quint8> ItemDist(1,20);
            std::uniform_int_distribution<quint64> IdDist(1,10000);

            quint8 ItemsCount = ItemDist(RandomEngen);

            for (quint8 Index = 0; Index < ItemsCount; ++Index)
            {
                quint64 NewItemID = IdDist(RandomEngen);

                std::shared_ptr<TreeItem> NewItem = std::make_shared<TreeItem>(NewItemID, this, this);
                NewItem->setItemText("Item_Level=" + QString::number(ItemLevel+1) + "_ID="  + QString::number(NewItemID));

                if (ItemLevel == MaxItemLevel-1)
                    NewItem->setItemType(eItemType::itFile);
                else
                    NewItem->setItemType(eItemType::itDir);


                this->push_back(NewItem);
            }
        }
    }
}
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::itemParent - Метод вернёт указатель на предка айтема
 * @return Вернёт указатель на родительский айтем
 */
TreeItem *TreeItem::itemParent()
{ return fParentItem; }
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::itemID - Метод вернёт ID айтема
 * @return Вернёт ID айтема
 */
quint64 TreeItem::itemID()
{ return fID; }
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::setItemText - Метод задаст текст айтема
 * @param inText - Новое значение текста айтема
 */
void TreeItem::setItemText(QString inText)
{ fText = inText; }
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::itemText - Метод вернёт текст айтема
 * @return Верёнт текст айтема
 */
QString TreeItem::itemText()
{ return fText; }
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::setItemType - Метод задаст тип айтема
 * @param inType - Новое значение типа айтема
 */
void TreeItem::setItemType(eItemType inType)
{ fType = inType; }
//-----------------------------------------------------------------------------
/**
 * @brief TreeItem::itemType - Метод вернёт тип айтема
 * @return Вернёт тип айтема
 */
eItemType TreeItem::itemType()
{ return fType; }
//-----------------------------------------------------------------------------
// TTreeModel
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::TTreeModel - Конструктор
 * @param inParentObject - Объект предок (По умолчанию nullptr)
 */
TTreeModel::TTreeModel(QObject *inParentObject) : QAbstractItemModel(inParentObject)
{
    initHeadersText(); // Инициализируем текст заголовков
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::~TTreeModel - Диструктор
 */
TTreeModel::~TTreeModel()
{

}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::index - Метод вернёт индекс айтема (если такой есть)
 * @param row - строка айтема
 * @param column - столбец айтема
 * @param parent - индекс предка айтема
 * @return Вернёт индекс айтема если он есть, в противном случаи вернёт невалидный пустой индекс
 */
QModelIndex TTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex Result = QModelIndex(); // Возвращаемый результат

    if (!hasIndex(row, column, parent)) // Если индекс не существует
        Result = QModelIndex();
    else // Индекс существует
    {
        if (!parent.isValid()) // Если нет предка (корневые узлы)
        {
            auto It = this->begin(); // Получаем итератор на начала контейнера коренвых айтемов
            std::advance(It, row); // Смещаем на указанную строку

            Result = createIndex(row, column, It->get()); // Создаём индекс с указателем на данные указанного корневого элемента
        }
        else // Если предок существует (Потомки корневых узлов)
        {
            TreeItem* DataPtr = static_cast<TreeItem*>(parent.internalPointer()); // Приводим даннные предка к указателю на TreeItem
            auto It = DataPtr->begin(); // Получаем итератор на начало контейенера потомков предка
            std::advance(It, row); // Смещаем на указанную строку

            Result = createIndex(row, column, It->get()); // Создаём индекс с указателем на данные полученного потомка указанного корневого элемента
        }
    }

    return Result; // Возвращаем полученный результат
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::parent - Метод вернёт предка указанного потомка (если такой есть)
 * @param child - Индекс потомка
 * @return Вернёт индекс вредка child если он есть, в противном случаи вернёт невалидный пустой индекс
 */
QModelIndex TTreeModel::parent(const QModelIndex &child) const
{
    QModelIndex Result = QModelIndex(); // Возвращаемый результат

    if (!child.isValid()) // Если указанный потомок не валиден
        Result = QModelIndex();
    else // Указанный потомок валиден
    {
        TreeItem* childPtr = static_cast<TreeItem*>(child.internalPointer()); // Приводим даннные потомка к указателю на TreeItem
        TreeItem* rootPtr = childPtr->itemParent(); // Получаем указатель на предка

        if (rootPtr == nullptr) // Еслли у элемента нет предка
            Result = QModelIndex();
        else // Предок есть
        {   // Нужно найти элемент в списке потомков
            auto FindRes = std::find_if(rootPtr->begin(), rootPtr->end(), [&childPtr](const std::shared_ptr<TreeItem>& Item) // Ищим потомка
            { return Item->itemID() == childPtr->itemID(); }); // По его уникальному ID

            if (FindRes == rootPtr->end()) // Не нашлось потомка (НЕ ДОЛЖНО ТАКОГО БЫТЬ!)
                Result = QModelIndex();
            else // Элемент был найден
            {
                int Row = std::distance(rootPtr->begin(), FindRes); // Вычисляем позицию элемент
                Result = createIndex(Row, static_cast<quint8>(eColIndex::ciText), rootPtr); // Создаём индекс с указанием на данные предка
            }
        }
    }

    return Result; // Возвращаем полученный результат
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::rowCount - Метод вернтёт количество строк (элементов) указанного айтема
 * @param parent - Индекс айтема "пердка" (По умолчанию пуст\не валиден)
 * @return Вернёт количество строк (элементов) указанного айтема
 */
int TTreeModel::rowCount(const QModelIndex &parent) const
{
    int Result = 0;

    if (!parent.isValid()) // Если нет предка (корневые узлы)
        Result = this->size(); // Возвращаем количество элементов в контейнере корневых элементов
    else // Если предок существует (Потомки корневых узлов)
    {
        TreeItem* DataPtr = static_cast<TreeItem*>(parent.internalPointer()); // Приводим даннные предка к указателю на TreeItem
        Result = DataPtr->size(); // Возвращаем количество элементов в контейнере потомкав полученного предка
    }

    return Result; // Возвращаем полученный результа
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::columnCount - Метод вернёт колчиестов столбцов указанного айтема
 * @param parent - Индекс айтема "пердка" (По умолчанию пуст\не валиден)
 * @return Вернёт колчиестов столбцов указанного айтема
 */
int TTreeModel::columnCount(const QModelIndex &parent) const
{   // В этой модели количество столбцов во всех уровнях одинаковое
    Q_UNUSED(parent) // Не используемый параметр
    return static_cast<quint8>(eColIndex::ciColCount); // Возвращаем количество столбцов
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::data - Метод вернёт данные по индексу указанного айтема
 * @param index - Индекс айтема
 * @param role - Роль данных
 * @return Вернёт данные атема в соответствии с указанной ролью
 */
QVariant TTreeModel::data(const QModelIndex &index, int role) const
{
    QVariant Result = QVariant(); // Возвращаемый результат

    if (!index.isValid()) // Если индекс не валиден
        Result = QVariant();
    else // Индекс валиден
    {
        TreeItem* ItemPtr = static_cast<TreeItem*>(index.internalPointer()); // Приводим даннные индекса к указателю на TreeItem

        switch (role) // Проверяем запрашиваемую роль
        {
            case Qt::DisplayRole: // Роль отображения
            {
                switch (static_cast<eColIndex>(index.column())) // Проверяем запрашиваемый столбец
                {
                    case eColIndex::ciText: { Result = ItemPtr->itemText(); break; } // Возвращаем текст айтема
                    case eColIndex::ciID: { Result = ItemPtr->itemID(); break; } // Возвращаем ID айтема
                    case eColIndex::ciType: // Возвращаем тип айтема
                    {
                        switch (ItemPtr->itemType()) // Проверяем тип
                        {
                            case eItemType::itDir: { Result = "Директория"; break; } // Возвращаем как директорию
                            case eItemType::itFile: { Result = "Файл"; break; } // Возвращаем как файл
                            default: Result = "Неизвестный"; // Возвращаем как неизвестный
                        }
                        break;
                    }
                    default: Result = QVariant(); // Во всех остальных случаях возвращаем невалидный QVariant
                }
                break;
            }
            case Qt::DecorationRole: // Роль декорации
            {
                if (static_cast<eColIndex>(index.column()) != eColIndex::ciText) // Не столбец текста
                    Result = QVariant(); // Возвращаем не валидный QVariant
                else // Столбец текста
                {
                    QFileIconProvider iconProvider; // Класс провайдер системных иконок

                    switch (ItemPtr->itemType()) // Проверяем тип
                    {
                        case eItemType::itDir: { Result = iconProvider.icon(QFileIconProvider::Folder); break; } // Возвращаем иконку директории
                        case eItemType::itFile: { Result = iconProvider.icon(QFileIconProvider::File); break; } // Возвращаем иконку файла
                        default: Result = QVariant(); // Возвращаем как не валидный QVariant
                    }
                }
                break;
            }
            default: Result = QVariant(); // Во всех остальных случаях возвращаем невалидный QVariant
        }
    }

    return Result; // Возвращаем полученный результа
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::headerData - Метод вернёт данные для указанного столбца
 * @param section - Порядковый номер стобца
 * @param orientation - Ориентация столбца (вертикальная\горизонтальная)
 * @param role - Роль данных
 * @return Вернёт данные указанного столба в соответствии с указанной ролью
 */
QVariant TTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant Result = QVariant(); // Возвращаемый результат

    if (section < columnCount()) // Если столбец в допустимом диапозоне
        if (role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal) // Если роль "Отображдение" и ориентация колонок "горизонтальная"
            Result = fHeadersText[section]; // Возвращаем заголовок указанного стоолбца

    return Result; // Возвращаем полученный результа
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::init - Метод инициализирует модель
 * @return Вернёт влаг успешности операции
 */
bool TTreeModel::init()
{
    std::random_device RandomDevice;
    std::mt19937 RandomEngen(RandomDevice());

    std::uniform_int_distribution<quint8> RootsDist(1,20);
    std::uniform_int_distribution<quint64> IdDist(1,10000);

    quint8 RootsCount = RootsDist(RandomEngen);

    for (quint8 Index = 0; Index < RootsCount; ++Index)
    {
        quint64 NewItemID = IdDist(RandomEngen);

        std::shared_ptr<TreeItem> NewRootItem = std::make_shared<TreeItem>(NewItemID, nullptr, this);
        NewRootItem->setItemText("RootItem_" + QString::number(NewItemID));
        NewRootItem->setItemType(eItemType::itDir);

        this->push_back(NewRootItem);
    }

    return true;
}
//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::initHeadersText - Метод инициализирует заголовки
 */
void TTreeModel::initHeadersText()
{   // Инициализируем текст заголовков
    fHeadersText[static_cast<quint8>(eColIndex::ciText)] = "Имя";
    fHeadersText[static_cast<quint8>(eColIndex::ciType)] = "Тип";
    fHeadersText[static_cast<quint8>(eColIndex::ciID)] = "ID";
}
//-----------------------------------------------------------------------------
