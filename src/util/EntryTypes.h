// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include <QAbstractListModel>
#include <qqmlintegration.h>
#include <QString>
#include <QVariant>
#include <QVector3D>

#include "INode.h"
#include "TagDictionary.h"

namespace CALUMI::Motion
{
    class EntryTypesEnum : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        ~EntryTypesEnum() override = default;
        static EntryTypesEnum& instance();
        static EntryTypesEnum* create(const QQmlEngine* qmlEngine, const QJSEngine* jsEngine);

        Q_STATIC_LOGGING_CATEGORY(lcEntryTypesEnum,"EntryTypesEnum")

    private:
        using QObject::QObject;

    public:
        enum class EntryType : quint32
        {
            NONE        = 0x0,

            INT         = 0x01,
            FLOAT       = 0x02,
            STRING      = 0x04,
            BOOL        = 0x08,
            VECTOR      = 0x10,
            ANYBASIC    = INT | FLOAT | STRING | BOOL | VECTOR,

            VAR_INT     = 0x020,
            VAR_FLOAT   = 0x040,
            VAR_STRING  = 0x080,
            VAR_BOOL    = 0x100,
            VAR_VECTOR  = 0x200,
            ANYVAR      = VAR_INT | VAR_FLOAT | VAR_STRING | VAR_BOOL | VAR_VECTOR,
            ANYBASICVAR = ANYVAR | ANYBASIC,

            PREFIX      = 0x0400,
            SUFFIX      = 0x0800,
            STATE       = 0x1000,
            ACTION      = 0x2000,
            SYNCSYS     = 0x4000,

            EVENT_GAME  = 0x08000,
            EVENT_GRAPH = 0x10000,
            EVENT_BOTH  = EVENT_GAME | EVENT_GRAPH,
            EVENT_MISC  = 0x20000,
            EVENT       = EVENT_BOTH | EVENT_MISC,

            CUSTOM,
            DEFAULT     = STRING,
            ANY         = ANYBASICVAR | EVENT | PREFIX | SUFFIX | ACTION | SYNCSYS | STATE
        };

        Q_ENUM(EntryType);
        Q_DECLARE_FLAGS(EntryTypes, EntryType);
        Q_FLAG(EntryTypes)
    };

    using EntryType  = EntryTypesEnum::EntryType;
    using EntryTypes = EntryTypesEnum::EntryTypes;

    class PropertyList : public QAbstractListModel
    {
        Q_OBJECT
    public:
        using QAbstractListModel::QAbstractListModel;
        ~PropertyList() override = default;

        enum class Role
        {
            Label = Qt::UserRole + 1,
            Tag,
            Value,
            AllowedTypes,
            Present,
            CustomDropdown
        };

    private:
        struct Entry
        {
            QVariant value;
            TagId tagId = InvalidTagId;
            EntryTypes allowedTypes = EntryType::DEFAULT;
            quint16 customDropdown = 0xFFFF;
            bool present = true;
        };

    public:
        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

        [[nodiscard]] QVariantList values(const QString& tag, bool caseSensitive = false) const;
        [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

        bool insertRow(int row, Entry entry);
        bool insertRows(int row, int count, const QModelIndex& parent) override;
        bool removeRows(int row, int count, const QModelIndex& parent) override;
        bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;

    private:
        [[nodiscard]] QStringList customDropdown(quint16 index) const;

    signals:
        void nonStandardRowInserted();

    private:
        std::vector<Entry> m_list;
         QPointer<INode> m_parent;
    };

    class PropertyTable : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        struct ColumnType
        {
            TagId tagId = InvalidTagId;
            EntryTypes allowedTypes = EntryType::DEFAULT;
            quint16 customDropdown = 0xFFFF;
        };

    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS(CALUMI::Motion::EntryTypes);

