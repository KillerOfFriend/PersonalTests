#include "TreeModel.h"

#include <QIcon>
#include <QFileIconProvider>

using namespace Models;

//-----------------------------------------------------------------------------
/**
 * @brief TTreeModel::TTreeModel - Конструктор по умолчанию
 * @param inParentObject - Объект предок (По умолчанию nullptr)
 */
TTreeModel::TTreeModel(QObject *inParentObject) : QAbstractItemModel(inParentObject)
{
    initHeadersText(); // Инициализируем текст заголовков
}
//-----------------------------------------------------------------------------
TTreeModel::TTreeModel(quint64 inRootID, QObject* inParentObject) : QAbstractItemModel(inParentObject) // Инициализирующий конструктор
{
    initHeadersText(); // Инициализируем текст заголовков
    init(inRootID); // Вызываем инициализацию по ID
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
            TTreeModel* DataPtr = static_cast<TTreeModel*>(parent.internalPointer()); // Приводим даннные предка к указателю на TreeItem
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
        TTreeModel* childPtr = static_cast<TTreeModel*>(child.internalPointer()); // Приводим даннные потомка к указателю на TreeItem
        TTreeModel* rootPtr = childPtr->fParentItem; // Получаем указатель на предка

        if (rootPtr == nullptr) // Еслли у элемента нет предка
            Result = QModelIndex();
        else // Предок есть
        {   // Нужно найти элемент в списке потомков
            auto FindRes = std::find_if(rootPtr->begin(), rootPtr->end(), [&childPtr](const std::shared_ptr<TTreeModel>& Item) // Ищим потомка
            { return Item->fID == childPtr->fID; }); // По его уникальному ID

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
        TTreeModel* DataPtr = static_cast<TTreeModel*>(parent.internalPointer()); // Приводим даннные предка к указателю на TreeItem
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
        TTreeModel* ItemPtr = static_cast<TTreeModel*>(index.internalPointer()); // Приводим даннные индекса к указателю на TreeItem

        switch (role) // Проверяем запрашиваемую роль
        {
            case Qt::DisplayRole: // Роль отображения
            {
                switch (static_cast<eColIndex>(index.column())) // Проверяем запрашиваемый столбец
                {
                    case eColIndex::ciText: { Result = ItemPtr->fText; break; } // Возвращаем текст айтема
                    case eColIndex::ciID: { Result = ItemPtr->fID; break; } // Возвращаем ID айтема
                    case eColIndex::ciType: // Возвращаем тип айтема
                    {
                        switch (ItemPtr->fType) // Проверяем тип
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

                    switch (ItemPtr->fType) // Проверяем тип
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
 * @brief TTreeModel::initHeadersText - Метод инициализирует заголовки
 */
void TTreeModel::initHeadersText()
{   // Инициализируем текст заголовков
    fHeadersText[static_cast<quint8>(eColIndex::ciText)] = "Имя";
    fHeadersText[static_cast<quint8>(eColIndex::ciType)] = "Тип";
    fHeadersText[static_cast<quint8>(eColIndex::ciID)] = "ID";
}
//-----------------------------------------------------------------------------
void TTreeModel::init(quint64 inRootID) // Метод инициализирует модель
{
    this->clear(); // Очищаем содержимое контейнера айтемов

    //--
    const quint8 MaxChiles = 20;
    const quint8 MaxLevel = 5;
    quint8 ItemLevel = 0;

    TTreeModel* Parent = this->fParentItem; // Получаем указатель на предка айтема
    while (Parent) // Пока предок есть
    {
        Parent = Parent->fParentItem; // Получаем предка предка)
        ItemLevel++; // Наращиваем уровень
    }
    //--
    fID = random() & 100;
    fText = "Item_Level-" + QString::number(ItemLevel) + " №" + QString::number(this->size());
    if (ItemLevel == 1)
        fType = eItemType::itDir;
    else
        if (ItemLevel == MaxLevel)
            fType = eItemType::itFile;
        else
        {
              if (random() & 2)
                    fType = eItemType::itDir;
              else fType = eItemType::itFile;
        }
    //--

    if (ItemLevel < MaxLevel && fType != eItemType::itFile) // Если уровень меньше максимального
        for(quint8 Index = 0; Index < rand() % MaxChiles; ++Index)
        {
            std::shared_ptr<TTreeModel> NewItem = std::make_shared<TTreeModel>(this);
            NewItem->fParentItem = this;
            NewItem->init(fID);
            this->push_back(NewItem);
        }
}
//-----------------------------------------------------------------------------
quint64 TTreeModel::itemID()
{ return this->fID; }
//-----------------------------------------------------------------------------
QString TTreeModel::itemText()
{ return this->fText; }
//-----------------------------------------------------------------------------
eItemType TTreeModel::itemType()
{ return this->fType; }
//-----------------------------------------------------------------------------
