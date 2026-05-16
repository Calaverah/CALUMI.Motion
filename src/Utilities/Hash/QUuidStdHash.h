#pragma once
#include <quuid.h>

template<>
struct std::hash<QUuid>
{
    size_t operator()(const QUuid& uid) const noexcept { return qHash(uid); }
};
