#pragma once
#include <QQmlEngine>
#include <qtmetamacros.h>
#include <qmetatype.h>

namespace CALUMI::Motion
{
    class NodeEnumClass
    {
        Q_GADGET
        QML_UNCREATABLE("Cannot create enum parent class")
    public:
        enum NodeType : quint32
        {
            UNDEFINED       = 0x0000,
            FO3             = 0x1000,
            FNV             = 0x2000,
            SSE             = 0x3000,
            FO4             = 0x4000,
            FO76            = 0x5000,
            SF1             = 0x6000,
            SEN             = 0x7000,
        };

        Q_ENUM(NodeType)
    };

    typedef NodeEnumClass::NodeType NodeType;
}
