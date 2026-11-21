#pragma once
#include "EntryTypes.h"

namespace CALUMI::Motion
{
    using PortId = quint32;

    class INode;

    class PortList : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum class Role
        {
            Title = Qt::UserRole + 1,
            Id,
            HasPropertyList,
            PropertyList,
            PropertyOptions,
            BlendInputList,
            BlendInputOptions,
            HasBlendInputList,
            PortType
        };

        enum class Option : quint8
        {
            Disallowed,
            Optional,
            Required
        };

        Q_ENUM(Option)

        enum class PortType : quint8
        {
            NONE = 0x0,
            IN = 0x1,
            OUT = 0x2,
            BOTH = IN | OUT,
        };

        Q_ENUM(PortType)
        Q_DECLARE_FLAGS(PortTypes, PortType)
        Q_FLAG(PortTypes)

        explicit PortList(PortType type, INode* parent);
        ~PortList() override = default;

        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

        bool insertRows(int row, int count, const QModelIndex& parent) override;
        bool removeRows(int row, int count, const QModelIndex& parent) override;
        bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;
        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

        [[nodiscard]] bool hasPort(PortId portId) const;
        [[nodiscard]] QList<quint32> portIndices(PortId portId) const;

    private:
        void resetIdHash();

    public:
        Q_INVOKABLE bool setPropertyListOption(Option setting);
        Q_INVOKABLE bool setBlendInputOption(Option setting);
        Q_INVOKABLE void setPropertyList(bool enable, int index, bool emitSignal = true);
        Q_INVOKABLE void setBlendInputList(bool enable, int index, bool emitSignal = true);


    private:
        struct Port
        {
            QList<QPointF> blendInput = {};
            QPointer<PropertyList> propertyList = Q_NULLPTR;
            QString title;
            PortId id;
            bool hasBlendInput = false;
            ~Port()
            {
                propertyList->deleteLater();
            }
        };

        QPointer<INode> m_node = Q_NULLPTR;
        QList<Port> m_list;
        QMultiHash<PortId, quint32> m_idHash;
        Option m_propOption = Option::Optional;
        Option m_blendOption = Option::Disallowed;
        PortType m_portType = PortType::NONE;
    };

    using PortType = PortList::PortType;
    using PortTypes = PortList::PortTypes;
}
