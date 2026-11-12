#pragma once
#include <QAbstractListModel>

namespace CALUMI::Motion
{
    using PortId = quint32;

    class PortList : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum class Role
        {
            Title = Qt::UserRole + 1,
            Id,
            Properties,
            BlendInput
        };

        using QAbstractListModel::QAbstractListModel;
        ~PortList() override = default;

        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
        [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

        bool insertRows(int row, int count, const QModelIndex& parent) override;
        bool removeRows(int row, int count, const QModelIndex& parent) override;
        bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;
        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;

    private:
        struct Port
        {
            QString title;
            PortId id;
            bool hasBlendInput;
            bool hasProperties;
            QList<QPointF> blendInput;
            //Proplist model
        };

        QList<Port> m_list;
    };
}
