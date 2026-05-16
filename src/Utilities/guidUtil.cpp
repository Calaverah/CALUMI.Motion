#include "stdafx.h"
#include "guidUtil.h"

bool guidUtil::addGuid(const QString& guid)
{
    constexpr QUuid qguid;
    qguid.fromString(guid);
    return addGuid(qguid);
}

bool guidUtil::addGuid(const QUuid& guid)
{
    const size_t size = m_guids.size();

    if (m_guids.contains(guid))
        return false;

    m_guids.insert(guid);

    return size < m_guids.size();
}

void guidUtil::releaseGuid(const QString& guid)
{
    constexpr QUuid qGuid;
    qGuid.fromString(guid);
    releaseGuid(qGuid);
}

void guidUtil::releaseGuid(const QUuid & guid)
{
    m_guids.remove(guid);
}

guidObject::guidObject()
{
    setGuid(m_guid, false);
}

guidObject::~guidObject()
{
    guidUtil::GetInstance().releaseGuid(m_guid);
}

QUuid guidObject::getGuid() const
{
    return m_guid;
}

const QUuid* guidObject::getGuidRef() const
{
    return &m_guid;
}

QUuid guidObject::setGuid(const QUuid guid, const bool keepOldIfInvalid)
{
    if (bool isUnique = guidUtil::GetInstance().addGuid(guid))
    {
        m_guid = guid;
    }
    else if(!keepOldIfInvalid)
    {
        while (!isUnique)
        {
            QUuid newGuid = QUuid::createUuid();
            isUnique = guidUtil::GetInstance().addGuid(newGuid);
            if (isUnique)
            {
                m_guid = newGuid;
            }
        }
    }

    return m_guid;
}

QUuid guidObject::setGuid(const QString& guid, const bool keepOldIfInvalid)
{
    return setGuid(QUuid::fromString(guid),keepOldIfInvalid);
}
