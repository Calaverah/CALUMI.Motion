//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QPointer>
#include <QObject>

#pragma warning(pop)
#include "Utilities/AgxDefinitions.h"
#include "Widgets/AgxNodePropertiesWidget.h"
#include "Utilities/guidUtil.h"
#include "Models/AgxPort.h"
#include "Utilities/AgxDictionary.h"
#include "Painter/AgxStyleCollection.h"

struct AgxNodeDataType
{
	QString id;
	QString name;
};

class AgxNodeData
{
public:
	virtual ~AgxNodeData() = default;

	virtual bool sameType(AgxNodeData const& nodeData) const
	{
		return this->type().id == nodeData.type().id;
	}

	/// Type for inner use
	virtual AgxNodeDataType type() const = 0;
};

enum class AgxNodeType : uint8_t
{
	NT_ANIMATION_CORRECTED_NODE, //Unused
	NT_ANIMATION_IMPACT_SELECTOR,
	NT_ANIMATION_NODE,
	NT_ASSIGN_ISTATE,
	NT_ASSIGN_VARIABLE,
	NT_BLEND_NODE,
	NT_BLEND_TREE_EMBEDDED,
	NT_BONE_CONSTRAINT,
	NT_CLONE_POSE,
	NT_CONVERT_BONE_DATA_TO_VARIABLES,
	NT_COPY_BONE_WEIGHTS,
	NT_CRITICALLY_DAMPEN_VARIABLE, //Unused
	NT_CUMULATIVE_ANIMATION,
	NT_CURVED_PATH_BLENDER,
	NT_DAMPEN_VARIABLE, //Unused
	NT_DIRECT_AT,
	NT_DUAL_DIRECT_AT,
	NT_DYNAMIC_ANIMATION,
	NT_DYNAMIC_GRAPH_REFERENCE,
	NT_EFFECT_SEQUENCE,
	NT_EVALUATE_CONDITION_VARIABLE,
	NT_EVENT_CONTROLLER,
	NT_EVENT_FROM_RANGE, //Unused
	NT_EVERY_N_EVENTS_MODIFIER,
	NT_FOOT_IK,
	NT_GAMEBRYO_SEQUENCE,
	NT_GRAPH_REF,
	NT_INVALID, //Unused
	NT_LINEAR_VARIABLE,
	NT_LOCOMOTION_BLEND,
	NT_LOOK_AT, //Unused
	NT_MASS_SPRING_DAMPEN_VARIABLE,
	NT_MATERIAL_LAYER_SEQUENCE,
	NT_MERGE_NODE,
	NT_MIRROR_MODIFIER,
	NT_MODIFY_GROUP, //Unused
	NT_MOMENTUM_ANIMATION,
	NT_MOMENTUM_SWITCHBACK,
	NT_MULTI_FOOT_IK,
	NT_NORMALIZE_ROTATION,
	NT_NUM_ANIMATION_NODES, //Unused
	NT_PAIRED_ANIMATION,
	NT_PARTICLE_SEQUENCE,
	NT_PATHING_ANIMATIONS,
	NT_PHYSICS_CONTACT_LISTENER,
	NT_POST_BONE_MODIFIER_CONTROL,
	NT_RAGDOLL, //Unused
	NT_RAGDOLL_DRIVE,
	NT_RAGDOLL_GET_UP,
	NT_RANDOM_ANIMATION_NODE,
	NT_RIG_SWITCH,
	NT_ROLLING_BONE, //Unused
	NT_ROOT_TWIST,
	NT_ROTATION_VARIABLE,
	NT_SET_ORIENT,
	NT_SET_POS,
	NT_SINGLE_BONE_IK, //Unused
	NT_SPEED_SCALE,
	NT_STAGGER_METER,
	NT_STATE_MACHINE_EMBEDDED,
	NT_STATE_VARIABLE_CONTROL,
	NT_SWAP_GRAPH,
	NT_SWITCH_NODE,
	NT_TAG_PROPAGATION,
	NT_TIMER_EVENT,
	NT_TRANSLATION_ADJUSTMENT,
	NT_TWO_BONE_IK,
	NT_VARIABLE_COMBINER,

	Comment = 0xFD,
	DEBUG = 0xFE,
	UNDEFINED = 0xFF
};

class AgxGraphModel;
class AgxGraphicsScene;
class AgxGraphicsView;


struct AgxNodeValidationState
{
	enum class State : int {
		Valid = 0,      ///< All required inputs are present and correct.
		Warning = 1,    ///< Some inputs are missing or questionable, processing may be unreliable.
		Error = 2,      ///< Inputs or settings are invalid, preventing successful computation.
	};
	bool isValid() const { return m_state == State::Valid; }
	const QString& message() { return m_stateMessage; }
	const State& state() const { return m_state; }

	State m_state{ State::Valid };
	QString m_stateMessage{ "" };
};

class AgxStyleCollection;

class AgxNode  : public QObject
{
	Q_OBJECT

public:
	explicit AgxNode(AgxGraphModel* rootGraphRef);
	~AgxNode() override;

	virtual AgxNodeType GetNodeType() const = 0;
	virtual QString name() const = 0;
	virtual QString typeName() const { return name(); }
	virtual QString nameProperty() const { return m_nameProperty; }

	virtual QString caption() const = 0;
	virtual bool captionVisible() const { return true; }
	virtual QString SubCaption() const { return QString(); }

	virtual bool AltState() { return false; }
	virtual void SetAltState(bool enabled = false) {}

	virtual void SetUpNode(const AgxGameType& type) { m_gameType = type; }
	virtual bool resizable() const { return false; }

	virtual void SetNameProperty(const QString& newName);
	virtual bool CanSetNameProperty() const { return false; }

	virtual AgxPortType CanModifyPorts() const { return AgxPortType::Both; }

	virtual const QString& getGroupId() const { return m_groupName; }
	virtual void setGroupId(const QString& groupId) { m_groupName = groupId; }
	
	virtual void save(pugi::xml_node& parent, QVector<AgxConnectionId> connections, QVector<AgxNodeId> sortedIds, QPointF pos) {}
	virtual QJsonObject save() const;
	virtual void load(const QJsonObject& data);

	//destructive load operation. removes read elements and alerts on any data leftover
	virtual void load(pugi::xml_node& xmlNode);

	virtual AgxConnectionPolicy portConnectionPolicy(AgxPortType, AgxPortIndex) const;

	virtual const AgxNodeStyle& nodeStyle() const;
	virtual void setNodeStyle(const AgxNodeStyle& style);

	virtual void AmendValidationState(const QString& messageToAdd, const AgxNodeValidationState::State& minState);
	virtual void setValidationState(const AgxNodeValidationState& validationState);
	virtual AgxNodeValidationState validationState() const { return m_nodeValidationState; }

	virtual unsigned int nPorts(AgxPortType portType) const = 0;

	virtual void ToggleCollapse();
	virtual bool isCollapsed() const { return m_collapsed; }

	virtual void insertPropertySheetData(const QJsonObject& data);
	//Setting Cleared will request data serialization as reset values, however this will not change any data in the node itself
	virtual QJsonObject getPropertySheetData(bool cleared = false) const;

	virtual void setInData(std::shared_ptr<AgxNodeData> nodeData, const AgxPortIndex portIndex) {}
	virtual std::shared_ptr<AgxNodeData> outData(AgxPortIndex const port) { return std::shared_ptr<AgxNodeData>(); }


	virtual AgxPropertyBlockData* getPropertyBlock(const QString& block);
	virtual AgxPropertyBlockData* getPropertyBlock(TermRef ref);

	virtual std::shared_ptr<AgxGraphicsScene> GetEmbeddedNodeGraph() { return m_embeddedGraphScene; }
	virtual void SetUpEmbeddedNodeGraph();
	virtual void CloseEmbeddedView();

	virtual QWidget* GetSideBarContent();
	virtual QWidget* GetNodePropertyWidget() { return nullptr; }
	virtual void SetSidebarVisibility(bool state);

	virtual AgxNodeDataType dataType(AgxPortType portType, AgxPortIndex portIndex) const { return AgxNodeDataType{ "Port", "Port" }; }

public Q_SLOTS:
	virtual void inputPortAdded(AgxPortIndex idx) {}
	virtual void outputPortAdded(AgxPortIndex idx) {}
	virtual void inputPortRemoved(AgxPortIndex idx) {}
	virtual void outputPortRemoved(AgxPortIndex idx) {}

	virtual void inputConnectionCreated(const AgxConnectionId& connection);
	virtual void inputConnectionDeleted(const AgxConnectionId& connection);
	virtual void outputConnectionCreated(const AgxConnectionId& connection) {}
	virtual void outputConnectionDeleted(const AgxConnectionId& connection) {}

Q_SIGNALS:
	void statusUpdate(float loadPercentage, const QString& message = QString());
	void PropertySheetUpdated();
	void ParentGraphTypeUpdated(const AgxGraphType& type);
	void embeddedWidgetSizeUpdated();
	void dataUpdated(const AgxPortIndex& index);
	void dataInvalidated(const AgxPortIndex& index);
	void computingStarted();
	void computingFinished();
	void portsAboutToBeDeleted(const AgxPortType& portType, const AgxPortIndex& first, const AgxPortIndex& last);
	void portsDeleted();
	void portsAboutToBeInserted(const AgxPortType& portType, const AgxPortIndex& first, const AgxPortIndex& last);
	void portsInserted();


public:
	virtual void SetNodeIdRef(const AgxNodeId& nodeId);

	virtual std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index = 0xFFFFFFFF, QJsonObject data = QJsonObject());
	virtual void _RemovePort(AgxPortType portType, AgxPortIndex index = 0xFFFFFFFF, bool preserve = false);
	virtual QJsonObject PortData(AgxPortType portType, AgxPortIndex index);
	virtual void SetPortData(AgxPortType portType, AgxPortIndex index, const QJsonObject& dataSet);
	virtual unsigned int PortCount(AgxPortType portType) const;
	virtual void SetPortCount(AgxPortType portType, unsigned int count);
	virtual AgxPortIndex GetPortIndex(AgxPort* port) const;
	virtual AgxPortType GetPortType(AgxPort* port) const;
	virtual void _ExternalPortCommand(AgxPortType portType, AgxPortIndex index = 0xFFFFFFFF, const QString& command = "", const QString& payload = "");

	virtual QString portCaption(AgxPortType portType, AgxPortIndex idx) const;
	virtual bool portCaptionVisible(AgxPortType portType, AgxPortIndex idx) const;

	virtual void ResetPorts();

	virtual QWidget* embeddedWidget() { return nullptr; }

protected:
	AgxGameType m_gameType = AgxGameType::None;

	QVector<std::shared_ptr<AgxPort>> m_inPorts;
	QVector<std::shared_ptr<AgxPort>> m_outPorts;
	AgxPortId m_nextPortId;
	virtual AgxPortId NewPortId() { return m_nextPortId++; }

	bool m_collapsed : 1 = false;
	bool m_hiddenState : 1 = true;

	QString m_nameProperty;
	QString m_groupName;

	QPointer<AgxNodePropertiesWidget> m_nodePropertiesWidget;
	QPointer<QWidget> m_sidebarContent;
	virtual void InitializeWidget();

	QMap<TermRef, AgxPropertyBlockData> m_PropertyBlocks;
	QList<TermRef> m_blockOrder;

	QVector<AgxPropertyEntryDefinition> m_propertyEntries;
	QMap<QString, QPair<AgxColumnTypes, QString>> m_hiddenEntries;

	std::shared_ptr<AgxGraphicsScene> m_embeddedGraphScene;
	std::shared_ptr<AgxGraphModel> m_embeddedGraphModel;
	AgxGraphModel* m_rootGraphReference;

	QString m_excessData;

	AgxNodeId m_nodeIdRef = InvalidNodeId;

private:
	AgxNodeStyle m_nodeStyle;
	AgxNodeValidationState m_nodeValidationState;
};



//
//================================================================================================================================
//

class SFBGSNode : public AgxNode, public guidObject
{
public:
	explicit SFBGSNode(AgxGraphModel* rootGraphRef);
	~SFBGSNode() override = default;
	
protected:
	QMap<TermRef, QPair<AgxColumnTypes, QString >> m_sfbgsHidden = {{&AgxDictionary::noninstanced,{AgxColumnTypes::BasicBool,"-"}},
																	{&AgxDictionary::UseColor,{AgxColumnTypes::BasicBool,"-"}},
																	{&AgxDictionary::DefaultState,{AgxColumnTypes::BasicBool,"-"}},
																	{&AgxDictionary::PosX,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::PosY,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::ExpPosX,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::ExpPosY,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::NodeType,{AgxColumnTypes::BasicString,"-"}}
	};
	QList<TermRef> m_hiddenOrder = { AgxDictionary::noninstanced,
								 AgxDictionary::NodeType,
								 AgxDictionary::ExpPosX, 
								 AgxDictionary::PosX, 
								 AgxDictionary::ExpPosY, 
								 AgxDictionary::PosY};

	QVector<AgxPropertyEntryDefinition> m_sfbgsProperties = {
		AgxPropertyEntryDefinition(&AgxDictionary::UserId,"0",AgxColumnTypes::BasicInteger)
	};

public:
	void save(pugi::xml_node& parent, QVector<AgxConnectionId> connections, QVector<AgxNodeId> sortedIds, QPointF pos) override;
	QJsonObject save() const override;
	void load(QJsonObject const&) override;
	void load(pugi::xml_node& xmlNode) override;

	void insertPropertySheetData(const QJsonObject& data) override;
	QJsonObject getPropertySheetData(bool cleared = false) const override;

	QString SubCaption() const override;
	bool AltState() override;
	void SetAltState(bool enabled) override;

std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data = QJsonObject()) override;
	void _RemovePort(AgxPortType portType, AgxPortIndex index = 0xFFFFFFFF, bool preserve = false) override;
	void SetPortData(AgxPortType portType, AgxPortIndex index, const QJsonObject& dataSet) override;

	void InitializeWidget(bool split = true);
	QWidget* GetNodePropertyWidget() override;

QWidget* GetSideBarContent() override;

protected:
	int m_divisions = 0;
	AgxAnimationFlags m_flags;
};