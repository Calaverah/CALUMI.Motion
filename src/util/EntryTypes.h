// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include <QAbstractListModel>
#include <qqmlintegration.h>
#include <QString>
#include <QVariant>
#include <QVector3D>

#include "DropdownCollection.h"
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

            DROPDOWNVALUE,
            DEFAULT     = STRING,
            ANY         = ANYBASICVAR | EVENT | PREFIX | SUFFIX | ACTION | SYNCSYS | STATE
        };

        Q_ENUM(EntryType);
        Q_DECLARE_FLAGS(EntryTypes, EntryType);
        Q_FLAG(EntryTypes)

        static bool CanConvert(const QVariant& value, EntryTypes types);
    };

    using EntryType  = EntryTypesEnum::EntryType;
    using EntryTypes = EntryTypesEnum::EntryTypes;

    class PropertyEntry
    {
        Q_GADGET
        Q_PROPERTY(QVariant value MEMBER value);
        Q_PROPERTY(TagId tagId MEMBER tagId);
        Q_PROPERTY(EntryTypes allowedTypes MEMBER allowedTypes);
        Q_PROPERTY(DropId uniqueDropdown MEMBER uniqueDropdown);
    public:
        QVariant value;
        TagId tagId = InvalidTagId;
        EntryTypes allowedTypes = EntryType::DEFAULT;
        DropId uniqueDropdown = InvalidDropId;
    };

    class PropertyList final : public QAbstractListModel
    {
        Q_OBJECT
    public:
        using QAbstractListModel::QAbstractListModel;
        ~PropertyList() override = default;

        enum class Role
        {
            Label = Qt::UserRole + 1,
            TagId,
            Tag,
            Value,
            AllowedTypes,
            UniqueDropdown
        };

        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

        [[nodiscard]] QVariantList values(const QString& tag, bool caseSensitive = false) const;
        [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

        bool insertRow(int row, PropertyEntry entry, const QModelIndex& parent = {});
        bool insertRows(int row, int count, const QModelIndex& parent) override;
        bool removeRows(int row, int count, const QModelIndex& parent) override;
        bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;

        //[[nodiscard]] QMimeData* mimeData(const QModelIndexList& indexes) const override;
        //bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
        //[[nodiscard]] QStringList mimeTypes() const override;
        //[[nodiscard]] bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;

    private:
        QList<PropertyEntry> m_list;
    };

    class PropertyTable final : public QAbstractTableModel
    {
        Q_OBJECT
    public:
        enum class Role
        {
            Label = Qt::UserRole + 1,
            TagId,
            Tag,
            Value,
            DefaultValue,
            AllowedTypes,
            UniqueDropdown
        };

        PropertyTable(const QList<PropertyEntry>& columns, QObject* parent = nullptr);

        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;
        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] bool hasPartialRow() const;
        [[nodiscard]] int trueIndex(int row, int column, const QModelIndex& parent = {}) const;

        [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        bool insertRows(int row, int count, const QModelIndex& parent) override;
        bool removeRows(int row, int count, const QModelIndex& parent) override;
        bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;
        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

    private:
        const QList<PropertyEntry> m_columns;
        QVariantList m_table;
    };

    class PropertyTree final : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        enum class Role
        {
            Label = Qt::UserRole + 1,
            TagId,
            Tag,
            Value,
            DefaultValue,
            AllowedTypes,
            UniqueDropdown
        };

        using QAbstractItemModel::QAbstractItemModel;
        ~PropertyTree() override = default;

        [[nodiscard]] QModelIndex parent(const QModelIndex& child) const override;
        [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex& parent) const override;
        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

        Q_INVOKABLE bool addTable(int row, const QList<PropertyEntry>& columns, const QString& localLabel);
        Q_INVOKABLE bool appendTable(const QList<PropertyEntry>& columns, const QString& label) { return addTable(static_cast<int>(m_tables.size()), columns, label); }
        Q_INVOKABLE bool addTable(int row, const QList<PropertyEntry>& columns, TagId tag);
        Q_INVOKABLE bool appendTable(const QList<PropertyEntry>& columns, const TagId tag) { return addTable(static_cast<int>(m_tables.size()), columns, tag); }
        Q_INVOKABLE bool removeTable(int row);

        bool insertRows(int row, int count, const QModelIndex& parent) override;
        bool removeRows(int row, int count, const QModelIndex& parent) override;
        bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;

    private:
        static constexpr quintptr s_root_id = std::numeric_limits<quintptr>::max();
        struct PropertyTable
        {
            QList<PropertyEntry> columns;
            QVariantList data;
            TagId tagId = InvalidTagId;
            QString localLabel;

            void setLocalLabel(const QString& label)
            {
                localLabel = label;
                tagId = InvalidTagId;
            }

            void setTagId(const TagId id)
            {
                tagId = id;
            }

            [[nodiscard]] QString label() const
            {
                return tagId == InvalidTagId ? localLabel : TagDictionary::Label(tagId);
            }

            [[nodiscard]] QString tag() const
            {
                return tagId == InvalidTagId ? "local_tag" : TagDictionary::Tag(tagId);
            }

            [[nodiscard]] int trueIndex(const int row, const int column) const
            {
                return static_cast<int>(row * columns.size() + column);
            }
        };

        QList<PropertyTable> m_tables;
    };

}

Q_DECLARE_OPERATORS_FOR_FLAGS(CALUMI::Motion::EntryTypes);

