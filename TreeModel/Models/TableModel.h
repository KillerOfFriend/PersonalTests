#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <memory>
#include <map>

#include <QAbstractTableModel>
#include <QDateTime>
#include <QObject>

namespace Models
{
    //-----------------------------------------------------------------------------
    class TTableItem : public QObject
    {
        Q_OBJECT
    public:
        TTableItem(QObject* inParent = nullptr); // Конструктор по умолчанию
        ~TTableItem(); // Диструктор

        void setItemID(quint64 inID); // Метод задаст ID айтема
        quint64 itemID(); // Метод вернёт ID айтема

        void setItemName(QString inName); // Метод задаст имя айтема
        QString itemName(); // Метод вернёт имя айтема

        void setItemDateTime(QDateTime inDateTime); // Метод задаст время\дату айтема
        QDateTime itemDateTime(); // Метод вренёт время\дату айтема

        void setItemDouble(double inDouble); // Метод задаст Double значение айтема
        double itemDouble(); // Метод вернёт Double значение айтема

    private:
        quint64 fID = 0; // ID айтема
        QString fName = ""; // Имя айтема
        QDateTime fDateTime; // Время\дата
        double fDouble = 0.0; // Double значение

    };
    //-----------------------------------------------------------------------------
    class TTableModel : public QAbstractTableModel, public std::map<quint64, std::shared_ptr<TTableItem>>
    {
        Q_OBJECT
    public:
        enum class eColIndex : quint8 { ciID = 0, ciName, ciDateTime, ciDouble,     ciColCount }; // Список столбцов модели

        TTableModel(QObject* inParent = nullptr); // Конструктор по умолчанию
        ~TTableModel(); // Диструктор
        // Методы требующие обязательной реализации
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

#endif // TABLEMODEL_H
