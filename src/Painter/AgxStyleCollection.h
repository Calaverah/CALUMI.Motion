//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once


#include "AgxConnectionStyle.h"
#include "AgxGraphicsViewStyle.h"
#include "AgxNodeStyle.h"


class AgxStyleCollection
{
private:
    AgxStyleCollection() = default;

    AgxStyleCollection(const AgxStyleCollection&) = delete;
    AgxStyleCollection(AgxStyleCollection&&) = delete;

    AgxStyleCollection& operator=(const AgxStyleCollection&) = delete;
    AgxStyleCollection& operator=(AgxStyleCollection&&) = delete;

    static AgxStyleCollection& instance()
    {
        static AgxStyleCollection collection;

        return collection;
    }

public:
    inline static AgxNodeStyle const& nodeStyle() { return instance()._nodeStyle; }

    inline static AgxConnectionStyle const& connectionStyle() { return instance()._connectionStyle; } 

    inline static AgxGraphicsViewStyle const& flowViewStyle() { return instance()._flowViewStyle; }

    inline static void setNodeStyle(AgxNodeStyle ns) { instance()._nodeStyle = ns; }

    inline static void setConnectionStyle(AgxConnectionStyle cs) { instance()._connectionStyle = cs; }

    inline static void setGraphicsViewStyle(AgxGraphicsViewStyle vs) { instance()._flowViewStyle = vs; }

private:
    AgxNodeStyle _nodeStyle;

    AgxConnectionStyle _connectionStyle;

    AgxGraphicsViewStyle _flowViewStyle;
};


