#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <list>
#include <array>
#include <memory>

#include <QObject>
#include <QAbstractItemModel>

namespace Models
{
    //-----------------------------------------------------------------------------
    /**
     * @brief The eItemType enum - Перечисление типов айтемов
     */
    enum class eItemType : quint8
    {
        itUnknown = 0,  // Неизвестный (Не должно такого быть)
        itDir = 1,      // Директория
        itFile = 2      // Файл
    };
    //-----------------------------------------------------------------------------
    /**
     * @brief The TreeItem class - Класс айтема модели
     */
    class TreeItem : public QObject, public std::list<std::shared_ptr<TreeItem>>
    {
        Q_OBJECT
    public:
        TreeItem(quint64 inID, TreeItem* inParentItem,  QObject* inParentObject = nullptr); // Инициализирующий конструктор
        ~TreeItem(); // Диструктор

        TreeItem* itemParent(); // Метод вернёт указатель на предка айтема

        quint64 itemID(); // Метод вернёт ID айтема

        void setItemText(QString inText); // Метод задаст текст айтема
        QString itemText(); // Метод вернёт текст айтема

        void setItemType(eItemType inType); // Метод задаст тип айтема
        eItemType itemType(); // Метод вернёт тип айтема

    private:
        TreeItem* fParentItem = nullptr; // Указаетль на предка

        quint64 fID = 0; // Уникальный идентификатор айтема
        QString fText = ""; // Текс подписи айтема
        eItemType fType = eItemType::itUnknown; // Тип айтема

        void initChilde(); // Метод инициализирует потомков айтема

    };
    //-----------------------------------------------------------------------------
    /**
     * @brief The TTreeModel class - Класс иерархической модели данных
     */
    class TTreeModel : public QAbstractItemModel, public std::list<std::shared_ptr<TreeItem>>
    {   // Класс унаследован от абстрактной модели данных (Интерфейс работы с представлением) и контейнера айтемов (Это будут айтемы первого уровня)
        Q_OBJECT
    public:
        // ! ВЕТВЛЕНИЕ ДЕРЕВА ВСЕГДА ИДЁТ ПО САМОМУ ЛЕВОМУ СТОЛБЦУ. СЛЕДОВАТЕЛЬНО ТЕКСТ ДОЛЖЕН БЫТЬ САМЫМ ЛЕВЫМ !
        enum class eColIndex : quint8 { ciText = 0, ciType, ciID,     ciColCount }; // Список столбцов модели

        TTreeModel(QObject* inParentObject = nullptr); // Конструктор
        ~TTreeModel(); // Диструктор
        // Методы требующие обязательной реализации
        virtual QModelIndex index(int row, int column, const QModelIndex &parent) const; // Метод вернёт индекс айтема (если такой есть)
        virtual QModelIndex parent(const QModelIndex &child) const; // Метод вернёт предка указанного потомка (если такой есть)
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const; // Метод вернтёт количество строк (элементов) указанного айтема
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const; // Метод вернёт колчиестов столбцов указанного айтема
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const; // Метод вернёт данные по индексу указанного айтема
        // Переопределённые методы
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const; // Метод вернёт данные для указанного столбца
        // Свои методы
        bool init(); // Метод инициализирует модель

    private:
        std::array<QString, static_cast<quint8>(eColIndex::ciColCount)> fHeadersText; // Надписи заголовков

        void initHeadersText(); // Метод инициализирует заголовки

    };
    //-----------------------------------------------------------------------------
}

#endif // TREEMODEL_H
