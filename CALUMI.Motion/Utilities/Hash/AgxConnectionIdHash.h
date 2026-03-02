#pragma once

#include <functional>

#include "..\AgxDefinitions.h"

inline void hash_combine(size_t& seed)
{
    Q_UNUSED(seed);
}

template<typename T, typename... Rest>
inline void hash_combine(size_t& seed, const T& v, Rest... rest)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
}

namespace std {
    template<>
    struct hash<AgxConnectionId>
    {
        inline size_t operator()(AgxConnectionId const& id) const
        {
            size_t h = 0;
            hash_combine(h, id.outNodeId, id.outPortIndex, id.inNodeId, id.inPortIndex);
            return h;
        }
    };

    template<>
    struct hash<std::pair<AgxNodeId, AgxPortIndex>>
    {
        inline size_t operator()(std::pair<AgxNodeId, AgxPortIndex> const& nodePort) const
        {
            size_t h = 0;
            hash_combine(h, nodePort.first, nodePort.second);
            return h;
        }
    };

    template<>
    struct hash<std::tuple<AgxNodeId, AgxPortType, AgxPortIndex>>
    {
        using Key = std::tuple<AgxNodeId, AgxPortType, AgxPortIndex>;

        inline size_t operator()(Key const& key) const
        {
            size_t h = 0;
            hash_combine(h, std::get<0>(key), std::get<1>(key), std::get<2>(key));
            return h;
        }
    };
}