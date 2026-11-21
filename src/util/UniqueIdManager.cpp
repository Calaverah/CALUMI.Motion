#include "UniqueIdManager.h"

namespace CALUMI::Motion
{
    QUuid UniqueIdManager::createId(const QUuid& id)
    {
        const auto context = m_rootContext && !m_rootContext->objectName().isEmpty() ? m_rootContext->objectName() : "local id manager";
        auto targetId = id;

        if (hasId(targetId))
        {
            qCInfo(lcUID) << "Id: " << targetId.toString() << " already registered to " << context;
            while (hasId(targetId))
            {
                targetId = QUuid::createUuid();
            }
        }

        qCInfo(lcUID) << "Id: " << targetId.toString() << " registered to " << context;

        m_registeredIds.insert(targetId);
        return targetId;
    }

    bool UniqueIdManager::hasId(const QUuid& id) const
    {
        return m_registeredIds.contains(id);
    }
}
