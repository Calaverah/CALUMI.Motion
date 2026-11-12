#include "PortList.h"

namespace CALUMI::Motion
{
    int PortList::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(m_list.size());
    }

    QHash<int, QByteArray> PortList::roleNames() const
    {
        static QHash<int, QByteArray> roles = {
            {static_cast<int>(Role::Title), "title"},
            {static_cast<int>(Role::Id), "id"},
            {static_cast<int>(Role::Properties), "properties"},
            {static_cast<int>(Role::BlendInput), "blendInput"}
        };
        return roles;
    }
}
