#pragma once
#include <quuid.h>

namespace std {
    template<>
    struct hash<QUuid>
    {
        inline size_t operator()(const QUuid& uid) const { return qHash(uid); }
    };
}