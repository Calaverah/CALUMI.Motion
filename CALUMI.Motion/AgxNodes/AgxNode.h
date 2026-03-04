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
		return (this->type().id == nodeData.type().id);
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
	bool isValid() { return _state == State::Valid; };
	QString const message() { return _stateMessage; }
	State state() { return _state; }

	State _state{ State::Valid };
	QString _stateMessage{ "" };
};

class AgxStyleCollection;

class AgxNode  : public QObject
{
	Q_OBJECT

public:
	AgxNode(AgxGraphModel* rootGraphRef);
	virtual ~AgxNode();

	virtual AgxNodeType GetNodeType() const = 0;
	virtual QString name() const = 0;

	virtual QString caption() const = 0;
	virtual bool captionVisible() const { return true; }
	virtual inline QString SubCaption() const { return QString(); }

	virtual inline bool AltState() { return false; }
	virtual inline void SetAltState(bool enabled = false) {}

	virtual inline void SetUpNode(const AgxGameType& type) { _gameType = type; }
	virtual inline bool resizable() const { return false; }

	virtual void SetNameProperty(QString newName);
	virtual inline bool CanSetNameProperty() const { return false; }

	virtual inline AgxPortType CanModifyPorts() const { return AgxPortType::Both; }

	virtual inline const QString& getGroupId() const { return _groupName; }
	virtual inline void setGroupId(const QString& groupId) { _groupName = groupId; }
	
	virtual QJsonObject save() const;
	//virtual pugi::xml_node saveAsXML() const;
	virtual void load(QJsonObject const&);

	//destructive load operation. removes read elements and alerts on any data leftover
	virtual void load(pugi::xml_node& xmlNode);

	virtual AgxConnectionPolicy portConnectionPolicy(AgxPortType, AgxPortIndex) const;

	virtual const AgxNodeStyle& nodeStyle() const;
	virtual void setNodeStyle(const AgxNodeStyle& style);

	virtual void AmmendValidationState(const QString& messageToAdd, const AgxNodeValidationState::State& minState);
	virtual void setValidatonState(const AgxNodeValidationState& validationState);
	virtual AgxNodeValidationState validationState() const { return _nodeValidationState; }

	virtual unsigned int nPorts(AgxPortType portType) const = 0;

	virtual void ToggleCollapse();
	virtual inline bool isCollapsed() const { return collapsed; }

	virtual void insertPropertySheetData(const QJsonObject& data);
	//Setting Cleared will request data serialization as reset values, however this will not change any data in the node itself
	virtual QJsonObject getPropertySheetData(bool cleared = false) const;

	virtual void setInData(std::shared_ptr<AgxNodeData> nodeData, const AgxPortIndex portIndex) {}
	virtual std::shared_ptr<AgxNodeData> outData(AgxPortIndex const port) { return std::shared_ptr<AgxNodeData>(); }


	virtual AgxPropertyBlockData* getPropertyBlock(const QString& block);
	virtual AgxPropertyBlockData* getPropertyBlock(TermRef ref);

	virtual inline std::shared_ptr<AgxGraphicsScene> GetEmbeddedNodeGraph() { return _EmbeddedGraphScene; }
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
	void dataUpdated(const AgxPortIndex index);
	void dataInvalidated(const AgxPortIndex index);
	void computingStarted();
	void computingFinished();
	void portsAboutToBeDeleted(AgxPortType const portType, AgxPortIndex const first, AgxPortIndex const last);
	void portsDeleted();
	void portsAboutToBeInserted(AgxPortType const portType, AgxPortIndex const first, AgxPortIndex const last);
	void portsInserted();


public:
	virtual inline void SetNodeIdRef(AgxNodeId nodeId) {	_nodeIdRef = nodeId; 
														Q_EMIT PropertySheetUpdated();}

public:
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

	virtual inline QWidget* embeddedWidget() { return nullptr; }

protected:
	AgxGameType _gameType = AgxGameType::None;

	QVector<std::shared_ptr<AgxPort>> _In_Ports;
	QVector<std::shared_ptr<AgxPort>> _Out_Ports;
	AgxPortId _nextPortId;
	virtual inline AgxPortId NewPortId() { return _nextPortId++; }

protected:
	bool collapsed : 1 = false;
	bool hiddenState : 1 = true;

	QString _nameProperty;
	QString _groupName;

	QPointer<AgxNodePropertiesWidget> _NodePropertiesWidget;
	QPointer<QWidget> _sidebarContent;
protected:
	virtual void InitializeWidget();

	QMap<TermRef, AgxPropertyBlockData> _PropertyBlocks;
	QList<TermRef> _BlockOrder;

	QVector<AgxPropertyEntryDefinition> _PropertyEntries;
	QMap<QString, QPair<AgxColumnTypes, QString>> _HiddenEntries;

	std::shared_ptr<AgxGraphicsScene> _EmbeddedGraphScene;
	std::shared_ptr<AgxGraphModel> _EmbeddedGraphModel;
	AgxGraphModel* _RootGraphReference;

	QString _ExcessData;

protected:
	AgxNodeId _nodeIdRef = InvalidNodeId;

private:
	AgxNodeStyle _nodeStyle;
	AgxNodeValidationState _nodeValidationState;
};



//
//================================================================================================================================
//

class SFBGSNode : public AgxNode, public guidObject
{
public:
	SFBGSNode(AgxGraphModel* rootGraphRef);
	virtual ~SFBGSNode() = default;
	
protected:
	QMap<TermRef, QPair<AgxColumnTypes, QString >> _SFBGS_Hidden = {{&AgxDictionary::noninstanced,{AgxColumnTypes::BasicBool,"-"}},
																	{&AgxDictionary::UseColor,{AgxColumnTypes::BasicBool,"-"}},
																	{&AgxDictionary::DefaultState,{AgxColumnTypes::BasicBool,"-"}},
																	{&AgxDictionary::PosX,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::PosY,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::ExpPosX,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::ExpPosY,{AgxColumnTypes::BasicFloat,"-"}},
																	{&AgxDictionary::NodeType,{AgxColumnTypes::BasicString,"-"}}
	};
	QList<TermRef> _HiddenOrder = { AgxDictionary::noninstanced,
								 AgxDictionary::NodeType,
								 AgxDictionary::ExpPosX, 
								 AgxDictionary::PosX, 
								 AgxDictionary::ExpPosY, 
								 AgxDictionary::PosY};

	QVector<AgxPropertyEntryDefinition> _SFBGS_Properties = {
		AgxPropertyEntryDefinition(&AgxDictionary::UserId,"0",AgxColumnTypes::BasicInteger)
	};

public:
	QJsonObject save() const override;
	void load(QJsonObject const&) override;
	void load(pugi::xml_node& xmlNode) override;

	void insertPropertySheetData(const QJsonObject& data) override;
	QJsonObject getPropertySheetData(bool cleared = false) const override;

	QString SubCaption() const override;
	bool AltState() override;
	void SetAltState(bool enabled) override;

public:
	std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data = QJsonObject()) override;
	void _RemovePort(AgxPortType portType, AgxPortIndex index = 0xFFFFFFFF, bool preserve = false) override;
	void SetPortData(AgxPortType portType, AgxPortIndex index, const QJsonObject& dataSet) override;

	void InitializeWidget(bool split = true);
	QWidget* GetNodePropertyWidget() override;

public:
	QWidget* GetSideBarContent() override;

protected:
	int _Divisions = 0;
	AgxAnimationFlags _flags;
};