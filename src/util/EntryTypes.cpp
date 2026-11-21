#include "EntryTypes.h"
#include <QVariant>
#include <memory>

namespace CALUMI::Motion
{
    EntryTypesEnum& EntryTypesEnum::instance()
    {
        static auto entryTypes = new EntryTypesEnum();
        return *entryTypes;
    }

    EntryTypesEnum* EntryTypesEnum::create(const QQmlEngine* qmlEngine, const QJSEngine* jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        EntryTypesEnum* entryEnum = &instance();
        QJSEngine::setObjectOwnership(entryEnum, QJSEngine::CppOwnership);

        return entryEnum;
    }

    bool EntryTypesEnum::CanConvert(const QVariant& value, const EntryTypes types)
    {
        const auto valueType = value.typeId();

        if (types == EntryType::NONE)
            return false;

        switch (valueType)
        {
            case QMetaType::Int:
            case QMetaType::UInt:
                return types.testAnyFlag(EntryType::INT);
            case QMetaType::QString:
                return types.testAnyFlags(EntryType::ANYVAR | EntryType::STRING | EntryType::ACTION | EntryType::EVENT
                                        | EntryType::PREFIX | EntryType::SUFFIX | EntryType::STATE
                                        | EntryType::SYNCSYS);
            case QMetaType::Bool:
                return types.testAnyFlag(EntryType::BOOL);
            case QMetaType::Float:
            case QMetaType::Double:
                return types.testAnyFlag(EntryType::FLOAT);
            case QMetaType::QVector3D:
                return types.testAnyFlag(EntryType::VECTOR);
            default:
                break;
        }

        if (types.testAnyFlag(EntryType::DROPDOWNVALUE) && value.canConvert<DropdownValue>())
            return true;

        return false;
    }

    QHash<int, QByteArray> PropertyList::roleNames() const
    {
        static QHash<int, QByteArray> output = {
            { static_cast<int>(Role::AllowedTypes),     "allowedTypes" },
            { static_cast<int>(Role::UniqueDropdown),   "uniqueDropdown" },
            { static_cast<int>(Role::Label),            "label" },
            { static_cast<int>(Role::Tag),              "tag" },
            { static_cast<int>(Role::TagId),            "tagId" },
            { static_cast<int>(Role::Value),            "value" }
        };

        return output;
    }

    int PropertyList::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(m_list.size());
    }

    Qt::ItemFlags PropertyList::flags(const QModelIndex& index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }

    QVariantList PropertyList::values(const QString& tag, const bool caseSensitive) const
    {
        const auto cSense = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

        QVariantList output;
        output.reserve(m_list.size());

        auto tagsMatchCheck = [&tag, &cSense](const PropertyEntry& e)
        {
            const auto t = TagDictionary::Tag(e.tagId);
            return tag.compare(t, cSense) == 0;
        };

        for (const auto& entry : m_list | std::views::filter(tagsMatchCheck))
        {
            output.append(entry.value);
        }

        output.shrink_to_fit();
        return output;
    }

    QVariant PropertyList::data(const QModelIndex& index, const int role) const
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_list.size())
            return {};

        // ReSharper disable once CppUseStructuredBinding
        const auto& entry = m_list.at(index.row());

        switch (role)
        {
        case Qt::DisplayRole:
            return QString("%1 : %2").arg(TagDictionary::Label(entry.tagId)).arg(entry.value.toString());
        case static_cast<int>(Role::Value):
            return entry.value;
        case static_cast<int>(Role::AllowedTypes):
            return QVariant::fromValue(entry.allowedTypes);
            case static_cast<int>(Role::UniqueDropdown):
            return DropdownCollection::GetUniqueDropdown(entry.uniqueDropdown, true);
        case static_cast<int>(Role::Tag):
            return TagDictionary::Tag(entry.tagId);
        case static_cast<int>(Role::Label):
            return TagDictionary::Label(entry.tagId);
        case static_cast<int>(Role::TagId):
            return entry.tagId;
        default:
            break;
        }

        return {};
    }

    bool PropertyList::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_list.size())
            return {};

        // ReSharper disable once CppUseStructuredBinding
        auto& entry = m_list[index.row()];
        QList roles = {role};

        switch (role)
        {
            case static_cast<int>(Role::Value):
                {
                    if (!EntryTypesEnum::CanConvert(value, entry.allowedTypes))
                        return false;

                    if (value.canConvert<DropdownValue>())
                        entry.value = QVariant::fromValue(DropdownValue(entry.uniqueDropdown, value.value<DropdownValue>().index()));
                    else
                        entry.value = value;
                    break;
                }
            case static_cast<int>(Role::AllowedTypes):
                {
                    if (!value.canConvert<EntryTypes>())
                        return false;

                    entry.allowedTypes = value.value<EntryTypes>();
                    break;
                }
            case static_cast<int>(Role::UniqueDropdown):
                {
                    if (value.canConvert<DropId>())
                    {
                        entry.uniqueDropdown = value.value<DropId>();
                    }
                    else if (value.canConvert<QStringList>())
                    {
                        const auto idx = DropdownCollection::RegisterUniqueDropDown(value.value<QStringList>());
                        if (idx == InvalidDropId)
                            return false;
                        entry.uniqueDropdown = idx;
                    }
                    else
                    {
                        return false;
                    }
                    break;
                }
            case static_cast<int>(Role::TagId):
                {
                    if (!value.canConvert<TagId>())
                        return false;

                    entry.tagId = value.value<TagId>();
                    roles = {role, static_cast<int>(Role::Tag), static_cast<int>(Role::Label)};
                    break;
                }
            default:
                return false;
        }

        Q_EMIT dataChanged(index, index, roles);
        return true;
    }

    bool PropertyList::insertRow(const int row, const PropertyEntry entry, const QModelIndex& parent) // NOLINT(*-unnecessary-value-param)
    {
        if (entry.tagId == InvalidTagId)
            return false;

        if (parent.isValid() || row < 0 || row >= m_list.size())
            return false;

        beginInsertRows(parent, row, row);
        m_list.insert(row, entry);
        endInsertRows();

        return true;
    }

    bool PropertyList::insertRows(const int row, const int count, const QModelIndex& parent)
    {
        if (parent.isValid() || row < 0 || row >= m_list.size() || count <= 0)
            return false;

        beginInsertRows(parent, row, row + count - 1);
        for (int i = 0; i < count; i++)
        {
            m_list.insert(row, {});
        }
        endInsertRows();

        return true;
    }

    bool PropertyList::removeRows(const int row, const int count, const QModelIndex& parent)
    {
        if (parent.isValid() || row < 0 || row + count >= m_list.count() || count <= 0)
            return false;

        beginRemoveRows(parent, row, row + count - 1);
        m_list.remove(row, count);
        endRemoveRows();

        return true;
    }

    bool PropertyList::moveRows(const QModelIndex& sourceParent, const int sourceRow, const int count, const QModelIndex& destinationParent, const int destinationChild)
    {
        if (sourceParent.isValid() || destinationParent.isValid())
            return false;

        if (sourceRow < 0 || sourceRow + count > m_list.count() || count <= 0)
            return false;

        if (destinationChild < 0 || destinationChild > m_list.count())
            return false;

        if (destinationChild >= sourceRow && destinationChild <= sourceRow + count)
            return false;

        if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
            return false;

        QList<PropertyEntry> moveItems;
        moveItems.reserve(count);
        for (int i = 0; i < count; i++)
        {
            moveItems.append(m_list.takeAt(sourceRow));
        }

        int inPos = destinationChild;
        if (destinationChild > sourceRow)
            inPos -= count;

        for (int i = 0; i < count; i++)
        {
            m_list.insert(inPos + i, moveItems.at(i));
        }

        endMoveRows();
        return true;
    }

    PropertyTable::PropertyTable(const QList<PropertyEntry>& columns, QObject* parent) : QAbstractTableModel(parent), m_columns(columns) {}

    int PropertyTable::columnCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(m_columns.size());
    }

    int PropertyTable::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;

        if (m_table.isEmpty() || m_columns.isEmpty())
            return 0;

        return static_cast<int>((m_table.size() + m_columns.size() - 1) / m_columns.size());
    }

    bool PropertyTable::hasPartialRow() const
    {
        if (m_columns.isEmpty())
            return false;

        return m_table.count() % m_columns.count() != 0;
    }

    int PropertyTable::trueIndex(const int row, const int column, const QModelIndex& parent) const
    {
        if (parent.isValid())
            return -1;

        return static_cast<int>(row * m_columns.size() + column);
    }

    QVariant PropertyTable::data(const QModelIndex& index, const int role) const
    {
        if (!index.isValid() || index.column() < 0 || index.column() >= m_columns.count())
            return {};

        const auto trueIdx = trueIndex(index.row(), index.column(), index.parent());

        if (trueIdx < 0 || trueIdx >= m_table.size())
            return {};

        const auto& entry = m_table.at(trueIdx);
        // ReSharper disable once CppUseStructuredBinding
        const auto& header = m_columns.at(index.column());

        switch (role)
        {
            case Qt::DisplayRole:
                return QString("%1 : %2").arg(TagDictionary::Label(header.tagId).arg(entry.toString()));
            case static_cast<int>(Role::Label):
                return TagDictionary::Label(header.tagId);
            case static_cast<int>(Role::Tag):
                return TagDictionary::Tag(header.tagId);
            case static_cast<int>(Role::TagId):
                return QVariant::fromValue(header.tagId);
            case static_cast<int>(Role::AllowedTypes):
                return QVariant::fromValue(header.allowedTypes);
            case static_cast<int>(Role::DefaultValue):
                return header.value;
            case static_cast<int>(Role::UniqueDropdown):
                return DropdownCollection::GetUniqueDropdown(header.uniqueDropdown, true);
            case static_cast<int>(Role::Value):
                return entry;
            default:
                return {};
        }
    }

    bool PropertyTable::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        if (!index.isValid() || index.column() < 0 || index.column() >= m_columns.count())
            return false;

        const auto trueIdx = trueIndex(index.row(), index.column(), index.parent());

        if (trueIdx < 0 || trueIdx >= m_table.size())
            return false;

        const auto& header = m_columns.at(index.column());

        switch (role)
        {
            case static_cast<int>(Role::Value):
                {
                    if (!EntryTypesEnum::CanConvert(value, header.allowedTypes))
                        return false;

                    m_table[trueIdx] = value;
                    break;
                }
            default:
                return false;
        }

        Q_EMIT dataChanged(index, index, {role});
        return true;
    }

    QVariant PropertyTable::headerData(const int section, const Qt::Orientation orientation, const int role) const
    {
        if (role != Qt::DisplayRole)
            return {};

        if (orientation == Qt::Horizontal && section >= 0 && section < m_columns.count())
            return TagDictionary::Label(m_columns.at(section).tagId);


        if (orientation == Qt::Vertical)
            return section + 1;

        return {};
    }

    bool PropertyTable::insertRows(const int row, const int count, const QModelIndex& parent)
    {
        if (parent.isValid() || count <= 0 || row < 0 || row > rowCount(parent))
            return false;

        beginInsertRows(parent, row, row + count - 1);
        const auto start = trueIndex(row, 0, parent);

        m_table.reserve(m_table.count() + count);
        for (int ri = 0; ri < count; ri++)
        {
            m_table.insert(start, m_columns.size(), QVariant());
            for (int ci = 0; ci < m_columns.size(); ci++)
            {
                m_table[start + ci] = m_columns.at(ci).value;
            }
        }

        endInsertRows();
        return true;
    }

    bool PropertyTable::removeRows(const int row, const int count, const QModelIndex& parent)
    {
        if (parent.isValid() || row < 0 || row + count > rowCount(parent))
            return false;

        beginRemoveRows(parent, row, row + count - 1);

        const auto start = trueIndex(row, 0, parent);
        const auto eCount = m_columns.count() * count;
        m_table.remove(start, eCount);

        endRemoveRows();
        return true;
    }

    bool PropertyTable::moveRows(const QModelIndex& sourceParent, const int sourceRow, const int count, const QModelIndex& destinationParent, const int destinationChild)
    {
        if (sourceParent.isValid() || destinationParent.isValid() || count <= 0)
            return false;

        if (sourceRow < 0 || sourceRow + count >= rowCount(sourceParent))
            return false;

        if (destinationChild < 0 || destinationChild > rowCount(destinationParent))
            return false;

        if (destinationChild >= sourceRow && destinationChild <= sourceRow + count)
            return false;

        if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
            return false;

        // ReSharper disable CppTooWideScopeInitStatement
        const auto start = trueIndex(sourceRow, 0, sourceParent);
        const auto end = (sourceRow + count) * m_columns.count();
        const auto dest = destinationChild * m_columns.count();
        // ReSharper restore CppTooWideScopeInitStatement

        if (dest < start)
            std::rotate(m_table.begin() + dest, m_table.begin() + start, m_table.begin() + end);
        else
            std::rotate(m_table.begin() + start, m_table.begin() + end, m_table.begin() + dest);

        endMoveRows();
        return true;
    }

    QHash<int, QByteArray> PropertyTable::roleNames() const
    {
        static QHash<int, QByteArray> output = {
            { static_cast<int>(Role::Value), "value" },
            { static_cast<int>(Role::AllowedTypes), "allowedTypes" },
            { static_cast<int>(Role::DefaultValue), "defaultValue" },
            { static_cast<int>(Role::Label), "label" },
            { static_cast<int>(Role::TagId), "tagId" },
            { static_cast<int>(Role::Tag), "tag" },
            { static_cast<int>(Role::UniqueDropdown), "uniqueDropdown" }
        };
        return output;
    }

    Qt::ItemFlags PropertyTable::flags(const QModelIndex& index) const
    {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }

    QModelIndex PropertyTree::index(const int row, const int column, const QModelIndex& parent) const
    {
        if (!hasIndex(row, column, parent))
            return {};

        //this means top level so we store the top level id: 0 for internal id
        if (!parent.isValid())
            return createIndex(row, column, s_root_id);


        return createIndex(row, column, parent.row());
    }

    int PropertyTree::rowCount(const QModelIndex& parent) const
    {
        if (!parent.isValid())
            return static_cast<int>(m_tables.count());

        if (parent.internalId() == s_root_id)
        {
            const auto& table = m_tables.at(parent.row());

            if (table.data.empty() || table.columns.empty())
                return 0;

            return static_cast<int>((table.data.size() + table.columns.size() - 1) / table.columns.size());
        }

        //does not go deeper than each table's rows
        return 0;
    }

    int PropertyTree::columnCount(const QModelIndex& parent) const
    {
        if (!parent.isValid())
            return 1;

        if (parent.internalId() == s_root_id)
        {
            if (parent.row() < 0 || parent.row() >= m_tables.count())
                return 0;

            return static_cast<int>(m_tables.at(parent.row()).columns.size());
        }

        return 0;
    }

    QVariant PropertyTree::data(const QModelIndex& index, const int role) const
    {
        if (!index.isValid())
            return {};

        if (index.internalId() == s_root_id)
        {
            if (index.row() < 0 || index.row() >= m_tables.count())
                return {};

            const auto& table = m_tables.at(index.row());

            switch (role)
            {
            case Qt::DisplayRole:
            case static_cast<int>(Role::Label):
                return table.label();
            case static_cast<int>(Role::Tag):
                return table.tag();
            case static_cast<int>(Role::TagId):
                return table.tagId;
            default:
                return {};
            }
        }

        if (index.internalId() >= m_tables.size())
            return {};

        const auto& table = m_tables.at(static_cast<qsizetype>(index.internalId()));
        const auto trueIdx = table.trueIndex(index.row(), index.column());

        if (trueIdx < 0 || trueIdx >= table.data.count())
            return {};

        const auto& entryValue = table.data.at(trueIdx);

        if (index.column() < 0 || index.column() >= table.columns.size())
            return {};

        // ReSharper disable once CppUseStructuredBinding
        const auto& header = table.columns.at(index.column());

        switch (role)
        {
            case Qt::DisplayRole:
            case static_cast<int>(Role::Value):
                return entryValue;
            case static_cast<int>(Role::Label):
                return TagDictionary::Label(header.tagId);
            case static_cast<int>(Role::Tag):
                return TagDictionary::Tag(header.tagId);
            case static_cast<int>(Role::TagId):
                return header.tagId;
            case static_cast<int>(Role::AllowedTypes):
                return QVariant::fromValue(header.allowedTypes);
            case static_cast<int>(Role::DefaultValue):
                return header.value;
            case static_cast<int>(Role::UniqueDropdown):
                return QVariant::fromValue(header.uniqueDropdown);
            default:
                return {};
        }
    }

    bool PropertyTree::setData(const QModelIndex& index, const QVariant& value, const int role)
    {
        if (!index.isValid())
            return false;

        if (index.internalId() == s_root_id)
            return false;

        const int tableIdx = static_cast<int>(index.internalId());

        if (tableIdx < 0 || tableIdx >= m_tables.count())
            return false;

        auto& table = m_tables[tableIdx];
        const auto trueIdx = table.trueIndex(index.row(), index.column());

        if (trueIdx < 0 || trueIdx >= table.data.count())
            return false;

        const auto& entryValue = table.data.at(trueIdx);

        if (index.column() < 0 || index.column() >= table.columns.size())
            return false;

        const auto& header = table.columns.at(index.column());
        switch (role)
        {
        case Qt::DisplayRole:
        case static_cast<int>(Role::Value):
            {
                if (!EntryTypesEnum::CanConvert(entryValue, header.allowedTypes))
                    return false;

                table.data[trueIdx] = value;
                Q_EMIT dataChanged(index, index,{Qt::DisplayRole, static_cast<int>(Role::Value)});
                return true;
            }
        default:
            return false;
        }
    }

    bool PropertyTree::addTable(const int row, const QList<PropertyEntry>& columns, const QString& localLabel)
    {
        if (row < 0 || row > m_tables.count())
            return false;

        beginInsertRows({}, row, row);
        m_tables.insert(row, {.columns = columns, .data = {}, .tagId = InvalidTagId, .localLabel = localLabel});
        endInsertRows();
        return true;
    }

    bool PropertyTree::addTable(const int row, const QList<PropertyEntry>& columns, const TagId tag)
    {
        if (row < 0 || row > m_tables.count())
            return false;

        beginInsertRows({}, row, row);
        m_tables.insert(row, {.columns = columns, .data = {}, .tagId = tag, .localLabel = ""});
        endInsertRows();
        return true;
    }

    bool PropertyTree::removeTable(const int row)
    {
        if (row < 0 || row >= m_tables.count())
            return false;

        beginRemoveRows({}, row, row);
        m_tables.removeAt(row);
        endRemoveRows();
        return true;
    }

    bool PropertyTree::insertRows(const int row, const int count, const QModelIndex& parent)
    {
        if (row < 0 || count <= 0)
            return false;

        if (!parent.isValid())
        {
            if (row >= m_tables.count())
                return false;

            beginInsertRows(parent, row, row + count - 1);
            for (int i = 0; i < count; i++)
            {
                m_tables.insert(row + i, {.localLabel = "New Table"});
            }
            endInsertRows();
            return true;
        }

        if (parent.internalId() == s_root_id && parent.column() == 0)
        {
            const auto tableIdx = parent.row();
            auto& table = m_tables[tableIdx];

            if (table.columns.empty())
                return false;

            const auto trueStart = table.trueIndex(row, 0);

            if (trueStart < 0 || trueStart >= table.data.size())
                return false;

            beginInsertRows(parent, row, row + count - 1);

            QVariantList addedRows;
            addedRows.reserve(table.columns.size() * count);
            for (int rowCount = 0; rowCount < count; rowCount++)
            {
                for (const auto & column : table.columns)
                {
                    addedRows.push_back(column.value);
                }
            }
            table.data.reserve(table.data.size() + addedRows.size());
            table.data.insert(trueStart, addedRows);

            endInsertRows();
            return true;
        }

        return false;
    }

    bool PropertyTree::removeRows(const int row, const int count, const QModelIndex& parent)
    {
        if (count <= 0 || row < 0)
            return false;

        if (!parent.isValid())
        {
            if (row + count > m_tables.count())
                return false;

            beginRemoveRows(parent, row, row + count - 1);
            m_tables.remove(row, count);
            endRemoveRows();
            return true;
        }

        if (parent.internalId() == s_root_id && parent.column() == 0)
        {
            const auto tableIdx = parent.row();
            auto& table = m_tables[tableIdx];

            const auto trueStart = table.trueIndex(row, 0);

            if (trueStart < 0 || trueStart + count > table.data.size())
                return false;

            beginRemoveRows(parent, row, row + count - 1);
            table.data.remove(trueStart, count);
            endRemoveRows();
            return true;
        }

        return false;
    }

    bool PropertyTree::moveRows(const QModelIndex& sourceParent, const int sourceRow, const int count, const QModelIndex& destinationParent, const int destinationChild)
    {
        if (count <= 0 || sourceRow < 0 || destinationChild < 0)
            return false;

        if (sourceParent != destinationParent)
            return false;

        if (sourceRow < destinationChild && destinationChild <= sourceRow + count)
            return false;

        if (!sourceParent.isValid() && !destinationParent.isValid())
        {
            if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
                return false;

            const auto selectBegin = std::next(m_tables.begin(), sourceRow);
            const auto selectEnd = std::next(m_tables.begin(), sourceRow + count);

            if (sourceRow > destinationChild)
            {
                const auto dest = std::next(m_tables.begin(), destinationChild);
                std::rotate(dest, selectBegin, selectEnd);
            }
            else
            {
                const auto dest = std::next(m_tables.begin(), destinationChild + count);
                std::rotate(selectBegin, selectEnd, dest);
            }

            endMoveRows();
            return true;
        }

        if (sourceParent.internalId() == s_root_id && sourceParent.column() == 0)
        {
            const auto tableIdx = sourceParent.row();
            auto& table = m_tables[tableIdx];
            const auto selectStart = std::next(table.data.begin(), table.trueIndex(sourceRow, 0));
            const auto selectEnd = std::next(table.data.begin(), table.trueIndex(sourceRow + 1, 0) - 1);

            if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
                return false;

            if (sourceRow > destinationChild)
            {
                const auto destStart = std::next(table.data.begin(), table.trueIndex(destinationChild, 0));
                std::rotate(destStart, selectStart, selectEnd);
            }
            else
            {
                const auto destStart = std::next(table.data.begin(), table.trueIndex(destinationChild, 0) + count);
                std::rotate(selectStart, selectEnd, destStart);
            }

            endMoveRows();
            return true;
        }

        return false;
    }

    QModelIndex PropertyTree::parent(const QModelIndex& child) const
    {
        if (!child.isValid() || child.internalId() == s_root_id)
            return {};

        const auto tableIdx = static_cast<int>(child.internalId());
        if (tableIdx < 0 || tableIdx >= m_tables.count())
            return {};

        return createIndex(tableIdx, 0, s_root_id);
    }
}
