#include "stdafx.h"
#include "AgxNtMirrorModifier.h"

namespace SFBGS {
	AgxNtMirrorModifier::AgxNtMirrorModifier(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
	{
		_nameProperty = QStringLiteral("Mirror Modifier");
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Mirror, "True", AgxColumnTypes::BasicBool));

		_BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
	}
	QString AgxNtMirrorModifier::name() const
	{
		return QStringLiteral("NT_MIRROR_MODIFIER");
	}
	QString AgxNtMirrorModifier::caption() const
	{
		return tr("Mirror Modifier");
	}
	unsigned int AgxNtMirrorModifier::nPorts(AgxPortType portType) const
	{
		return 1;
	}
	AgxNodeType AgxNtMirrorModifier::GetNodeType() const
	{
		return AgxNodeType::NT_MIRROR_MODIFIER;
	}

	std::shared_ptr<AgxPort> AgxNtMirrorModifier::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
	{
		auto port = SFBGSNode::_AddPort(portType, index, data);

		if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
		{
			if (portType == AgxPortType::In) {
				sfbgsPort->SetName("passthrough");
				Q_EMIT sfbgsPort->PropertySheetUpdated();
			}
		}
		return port;
	}

}