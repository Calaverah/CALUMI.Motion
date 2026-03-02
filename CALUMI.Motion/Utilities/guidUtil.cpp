#include "stdafx.h"
#include "guidUtil.h"

bool guidUtil::addGuid(const QString& guid)
{
    QUuid qguid;
    qguid.fromString(guid);
    return addGuid(qguid);
}

bool guidUtil::addGuid(const QUuid& guid)
{
    size_t size = _guids.size();

    if (_guids.contains(guid))
        return false;

    _guids.insert(guid);

    return size < _guids.size();
}

void guidUtil::releaseGuid(const QString& guid)
{
    QUuid qguid;
    qguid.fromString(guid);
    releaseGuid(qguid);
}

void guidUtil::releaseGuid(const QUuid & guid)
{
    _guids.remove(guid);
}

guidObject::guidObject()
{
    setGuid(_guid, false);
}

guidObject::~guidObject()
{
    guidUtil::GetInstance().releaseGuid(_guid);
}

QUuid guidObject::getGuid() const
{
    return _guid;
}

const QUuid* const guidObject::getGuidRef() const
{
    return &_guid;
}

QUuid guidObject::setGuid(QUuid guid, bool keepOldIfInvalid)
{
    bool isUnique = guidUtil::GetInstance().addGuid(guid);
    if (isUnique)
    {
        _guid = guid;
    }
    else if(!keepOldIfInvalid)
    {
        while (!isUnique)
        {
            QUuid newGuid = QUuid::createUuid();
            isUnique = guidUtil::GetInstance().addGuid(newGuid);
            if (isUnique)
            {
                _guid = newGuid;
            }
        }
    }

    return _guid;
}

QUuid guidObject::setGuid(QString guid, bool keepOldIfInvalid)
{
    return setGuid(QUuid::fromString(guid),keepOldIfInvalid);
}
