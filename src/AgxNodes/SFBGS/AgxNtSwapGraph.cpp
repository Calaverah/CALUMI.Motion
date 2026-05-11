//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtSwapGraph.h"
#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/AgxLineEdit.h"
#include <Utilities/AgxJsonHelper.h>

namespace SFBGS {

    AgxNtSwapGraph::AgxNtSwapGraph(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Swap Graph");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Swap Graph", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableCustomBonesWhenBlendingOut, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData blockDef({ 
                                            AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                            AgxPropertyEntryDefinition(&AgxDictionary::GraphName,"graph_name.agx",AgxColumnTypes::BasicString)}, nullptr);
            //enterBlockDef.SetEnabledState(true);

            m_PropertyBlocks.insert(&AgxDictionary::Graphs, blockDef);
        }

        m_blockOrder = { &AgxDictionary::Graphs, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtSwapGraph::name() const
    {
        return QStringLiteral("NT_SWAP_GRAPH");
    }

    QString AgxNtSwapGraph::caption() const
    {
        return tr("Swap Graph");
    }

    unsigned int AgxNtSwapGraph::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
                break;
            default:
                return 0;
                break;
        }
    }

    AgxNodeType AgxNtSwapGraph::GetNodeType() const
    {
        return AgxNodeType::NT_SWAP_GRAPH;
    }
}