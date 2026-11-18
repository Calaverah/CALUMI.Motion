#include "PortList.h"

namespace CALUMI::Motion
{
    PortList::PortList(const PortType type, INode* parent) : QAbstractListModel(parent), m_node(parent), m_portType(type) {}

    int PortList::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(m_list.size());
    }

    QVariant PortList::data(const QModelIndex& index, const int role) const
    {
        if (!index.isValid())
            return {};

        if (index.row() < 0 || index.row() >= m_list.count())
            return {};

        // ReSharper disable once CppUseStructuredBinding
        const auto& node = m_list.at(index.row());

        switch (role)
        {
            case static_cast<int>(Role::PropertyList):
                return QVariant::fromValue(node.propertyList.get());
            case static_cast<int>(Role::PropertyOptions):
                return QVariant::fromValue(m_propOption);
            case static_cast<int>(Role::BlendInputList):
                return QVariant::fromValue(node.blendInput);
            case static_cast<int>(Role::BlendInputOptions):
                return QVariant::fromValue(m_blendOption);
            case static_cast<int>(Role::Id):
                return node.id;
            case static_cast<int>(Role::Title):
                return node.title;
            case static_cast<int>(Role::PortType):
                return QVariant::fromValue(m_portType);
            case static_cast<int>(Role::HasBlendInputList):
                return node.hasBlendInput || !node.blendInput.isEmpty();
            case static_cast<int>(Role::HasPropertyList):
                return !node.propertyList.isNull();
            default:
                return {};
        }
    }

    QHash<int, QByteArray> PortList::roleNames() const
    {
        static QHash<int, QByteArray> roles = {
            {static_cast<int>(Role::Title), "title"},
            {static_cast<int>(Role::Id), "id"},
            {static_cast<int>(Role::PropertyList), "propertyList"},
            {static_cast<int>(Role::BlendInputList), "blendInputList"},
            {static_cast<int>(Role::PropertyOptions), "propertyOptions"},
            {static_cast<int>(Role::BlendInputOptions), "blendInputOptions"},
            {static_cast<int>(Role::HasBlendInputList), "hasBlendInputList"},
            {static_cast<int>(Role::HasPropertyList), "hasPropertyList"},
            {static_cast<int>(Role::PortType), "portType"},
        };
        return roles;
    }

    bool PortList::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_list.count())
            return false;

        auto& port = m_list[index.row()];

        switch (role)
        {
        case static_cast<int>(Role::PropertyOptions):
        case static_cast<int>(Role::BlendInputOptions):
        case static_cast<int>(Role::PropertyList):
        case static_cast<int>(Role::BlendInputList):
        case static_cast<int>(Role::PortType):
                return false;
        case static_cast<int>(Role::HasBlendInputList):
            {
                if (!value.canConvert<bool>())
                    return false;

                setBlendInputList(value.value<bool>(), index.row());
                return true;
            }
        case static_cast<int>(Role::HasPropertyList):
            {
                if (!value.canConvert<bool>())
                    return false;

                setPropertyList(value.value<bool>(), index.row());
                return true;
            }
        case static_cast<int>(Role::Title):
            {
                if (!value.canConvert<QString>())
                    return false;

                port.title = value.toString();
                break;
            }
        case static_cast<int>(Role::Id):
            {
                if (!value.canConvert<PortId>())
                    return false;

                port.id = value.value<PortId>();
                break;
            }
        default:
            return false;
        }

        Q_EMIT dataChanged(index, index, {role});
        return true;
    }

    bool PortList::insertRows(const int row, const int count, const QModelIndex& parent)
    {
        if (parent.isValid() || row < 0 || row > m_list.count() || count <= 0)
            return false;

        m_list.reserve(m_list.size() + count);

        beginInsertRows(parent, row, row + count - 1);
        for (int i = 0; i < count; i++)
        {
            m_list.insert(row + i, {
                .blendInput = {},
                .propertyList = m_propOption == Option::Required ? new PropertyList(this) : Q_NULLPTR,
                .title = "", .id = m_node->nextPortId(),
                .hasBlendInput = m_blendOption == Option::Required
            });
        }

        resetIdHash();
        endInsertRows();
        return true;
    }

    bool PortList::removeRows(const int row, const int count, const QModelIndex& parent)
    {
        if ( parent.isValid() || row < 0 || row + count > m_list.count() || count <= 0)
            return false;

        beginRemoveRows(parent, row, row + count - 1);

        m_list.remove(row, count);

        resetIdHash();
        endRemoveRows();
        return true;
    }

    bool PortList::moveRows(const QModelIndex& sourceParent, const int sourceRow, const int count, const QModelIndex& destinationParent, const int destinationChild)
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

        QList<Port> moveItems;
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

        resetIdHash();
        endMoveRows();
        return true;
    }

    Qt::ItemFlags PortList::flags(const QModelIndex& index) const
    {
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }

    bool PortList::hasPort(const PortId portId) const
    {
        return m_idHash.contains(portId);
    }

    QList<quint32> PortList::portIndices(const PortId portId) const
    {
        if (!m_idHash.contains(portId))
            return {};

        return m_idHash.values(portId);
    }

    void PortList::resetIdHash()
    {
        QMultiHash<PortId, quint32> output;
        output.reserve(m_list.count());

        for (quint32 i = 0; i < m_list.count(); i++)
        {
            output.insert(m_list.at(i).id, i);
        }
        m_idHash = output;
    }

    bool PortList::setPropertyListOption(const Option setting)
    {
        QList roles = {static_cast<int>(Role::PropertyOptions)};

        if (m_list.isEmpty())
        {
            switch (setting)
            {
                case Option::Disallowed:
                    {
                        for (auto& port : m_list)
                        {
                            if (!port.propertyList.isNull())
                                port.propertyList->deleteLater();
                        }
                        roles.append(static_cast<int>(Role::PropertyList));
                        break;
                    }
                case Option::Optional:
                    break;
                case Option::Required:
                    {
                        for (auto& port : m_list)
                        {
                            if (port.propertyList.isNull())
                                port.propertyList = new PropertyList(this);
                        }
                        roles.append(static_cast<int>(Role::PropertyList));
                        break;
                    }
                default:
                    return false;
            }
        }

        m_propOption = setting;

        const auto top = index(0);
        const auto bottom = index(static_cast<int>(m_list.count() - 1));
        Q_EMIT dataChanged(top, bottom, roles);
        return true;
    }

    bool PortList::setBlendInputOption(const Option setting)
    {
        QList roles = {static_cast<int>(Role::BlendInputOptions)};

        if (m_list.isEmpty())
        {
            switch (setting)
            {
                case Option::Disallowed:
                    {
                        for (auto& port : m_list)
                        {
                            port.blendInput.clear();
                            port.hasBlendInput = false;
                        }
                        roles.append(static_cast<int>(Role::BlendInputList));
                        roles.append(static_cast<int>(Role::HasBlendInputList));
                        break;
                    }
                case Option::Required:
                    {
                        for (auto& port : m_list)
                        {
                            port.hasBlendInput = true;
                        }
                        roles.append(static_cast<int>(Role::BlendInputList));
                        roles.append(static_cast<int>(Role::HasBlendInputList));
                    }
                case Option::Optional:
                    break;
                default:
                    return false;
            }
        }

        m_propOption = setting;

        const auto top = index(0);
        const auto bottom = index(static_cast<int>(m_list.count() - 1));
        Q_EMIT dataChanged(top, bottom, roles);
        return true;
    }

    void PortList::setPropertyList(const bool enable, const int index, const bool emitSignal)
    {
        if (index < 0 || index >= m_list.count())
            return;

        auto& port = m_list[index];

        switch (m_propOption)
        {
            case Option::Disallowed:
                {
                    if (!port.propertyList.isNull())
                    {
                        port.propertyList->deleteLater();
                        if (emitSignal)
                        {
                            const auto idx = this->index(index);
                            Q_EMIT dataChanged(idx, idx, {static_cast<int>(Role::PropertyList), static_cast<int>(Role::HasPropertyList)});
                        }
                    }
                    return;
                }
            case Option::Optional:
                {
                    if (enable && port.propertyList.isNull())
                        port.propertyList = new PropertyList(this);
                    else if (!enable && !port.propertyList.isNull())
                        port.propertyList->deleteLater();
                    else
                        return;

                    if (emitSignal)
                    {
                        const auto idx = this->index(index);
                        Q_EMIT dataChanged(idx, idx, {static_cast<int>(Role::PropertyList), static_cast<int>(Role::HasPropertyList)});
                    }
                    return;
                }
            case Option::Required:
                {
                    if (port.propertyList.isNull())
                    {
                        port.propertyList = new PropertyList(this);
                        if (emitSignal)
                        {
                            const auto idx = this->index(index);
                            Q_EMIT dataChanged(idx, idx, {static_cast<int>(Role::PropertyList), static_cast<int>(Role::HasPropertyList)});
                        }
                    }
                    // ReSharper disable once CppRedundantControlFlowJump
                    return;
                }
        }
    }

    void PortList::setBlendInputList(const bool enable, const int index, const bool emitSignal)
    {
        if (index < 0 || index >= m_list.count())
            return;

        auto& port = m_list[index];

        switch (m_blendOption)
        {
        case Option::Disallowed:
            {
                if (!port.blendInput.isEmpty())
                {
                    port.blendInput.clear();
                    if (emitSignal)
                    {
                        const auto idx = this->index(index);
                        Q_EMIT dataChanged(idx, idx, {static_cast<int>(Role::BlendInputList), static_cast<int>(Role::HasBlendInputList)});
                    }
                }
                return;
            }
        case Option::Optional:
            {
                port.hasBlendInput = enable;
                if (!enable && !port.blendInput.isEmpty())
                    port.blendInput.clear();

                if (emitSignal)
                {
                    const auto idx = this->index(index);
                    Q_EMIT dataChanged(idx, idx, {static_cast<int>(Role::BlendInputList), static_cast<int>(Role::HasBlendInputList)});
                }
                return;
            }
        case Option::Required:
            {
                if (!port.hasBlendInput)
                    port.hasBlendInput = true;

                if (emitSignal)
                {
                    const auto idx = this->index(index);
                    Q_EMIT dataChanged(idx, idx, {static_cast<int>(Role::BlendInputList), static_cast<int>(Role::HasBlendInputList)});
                }
                // ReSharper disable once CppRedundantControlFlowJump
                return;
            }
        }
    }
}
