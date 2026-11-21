#pragma once
#include <QQmlEngine>
#include <QUuid>
#include "qloggingcategory.h"

namespace CALUMI::Motion
{
    class UniqueIdManager
    {
    public:
        explicit UniqueIdManager(QObject* context = Q_NULLPTR) : m_rootContext(context) {}
        ~UniqueIdManager() = default;

        Q_STATIC_LOGGING_CATEGORY(lcUID,"UniqueIdManager")

        QUuid createId(const QUuid& id = QUuid::createUuid());
        [[nodiscard]] bool hasId(const QUuid& id) const;

    private:
        QPointer<QObject> m_rootContext;
        QSet<QUuid> m_registeredIds;
    };
}
