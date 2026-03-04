#pragma once

#pragma warning(push,0)

#include "3rdPartyDirect/pugiXML/pugixml.hpp"
#include "AgxDictionary.h"
#include <qobject.h>
#include <qmap.h>
#include <cstdint>
#include <qswap.h>
#pragma warning(pop)

using AgxPortCount = unsigned int;

using AgxPortIndex = unsigned int;
static constexpr AgxPortIndex InvalidPortIndex = 0xFFFFFFFF;

using AgxNodeId = unsigned int;
static constexpr AgxNodeId InvalidNodeId = 0xFFFFFFFF;

enum class AgxGameType : uint8_t
{
    None,
    SFBGS //starfield
};

QString AgxGameTypeToString(const AgxGameType& type);
AgxGameType AgxGameTypeFromString(const QString& string);

enum class AgxFileType : uint8_t
{
    UNKNOWN,
    Behavior,
    Animation,
    AnimationX
};

QString AgxFileTypeToString(const AgxFileType& type);
AgxFileType AgxFileTypeFromString(const QString& str);

using AgxPortId = unsigned int;
static constexpr AgxPortId InvalidAgxPortId = 0xFFFFFFFF;

enum class AgxColumnTypes
{
    Default,
    BasicString,
    BasicInteger,
    BasicFloat,
    BasicBool,
    BasicVector,
    BasicMultiVar,
    CustomDropDown,
    CustomInteger,
    CustomFloat,
    CustomBool,
    CustomVector,
    CustomMultiVar,
    Event,
    Prefix,
    Suffix,
    State,
    Action,
    SyncSystem
    //CommentBox
};

QString GetAgxColumnTypeAsString(AgxColumnTypes type);
AgxColumnTypes GetAgxColumnTypeFromString(QString string);
AgxColumnTypes GetAgxCustomVarType(QString value);
AgxColumnTypes GetAgxBasicVarType(QString value);


enum class AgxPortType : uint8_t {
    None    = 0b00,
    In      = 0b01,
    Out     = 0b10,
    Both    = In | Out
};

enum class AgxVarType : uint8_t {
    Integer = 0,
    Float = 1,
    String = 2,
    Boolean = 3,
    Vector = 4,
    Any = 5,
    Undefined = 0xFF
};

QString GetSFBGSVarStringFromColumnType(AgxColumnTypes type);
AgxVarType GetAgxVarTypeFromSFBGS(QString value);
AgxVarType GetAgxVarTypeFromSFBGS(int value);


enum class AgxEventType : uint8_t {
    UNDEFINED,
    Graph_Event,
    Game_Event,
    Both,
    Special
    
};


enum class AgxTrueFalse : uint8_t {
    TRUE,
    FALSE,
    BLANK,
    UNDEFINED
};

const extern QMap<QString, AgxEventType> AgxEventTypeMap;
const extern QStringList AgxIntegerVars;
const extern QStringList AgxFloatVars;
const extern QStringList AgxBooleanVars;
const extern QStringList AgxVectorVars;

const extern QStringList AgxActionVars;
const extern QStringList AgxStateVars;

class AgxFlagField : public QObject
{
    Q_OBJECT
public:
    virtual ~AgxFlagField() = default;

    virtual size_t GetValue() const = 0;
    virtual QString ToString() const = 0;
    virtual void SetValue(size_t) = 0;
    virtual QJsonValue ToJson() const = 0;
    virtual void FromJson(const QJsonValue& data) = 0;
    virtual pugi::xml_node ToXML() = 0;
    virtual void FromXML(pugi::xml_node& node) = 0;
    virtual bool IsEnabledState() const = 0;
    virtual void SetEnabledState(bool enabled) = 0;

signals:
    void StateUpdated(bool enabled);
};

struct AgxAnimationFlags : public AgxFlagField
{
    Q_OBJECT
public:
    struct Data {
        bool 	Additive : 1 = false;
        bool 	Adjust_1st_Person_FOV : 1 = false;
        bool 	Allow_Rotation : 1 = false;
        bool 	Animation_Driven : 1 = false;
        bool 	Block_POV_Switch : 1 = false;
        bool 	Blocking : 1 = false;
        bool 	Cumulative_Rotation : 1 = false;
        bool 	Disable_Foot_IK : 1 = false;
        bool 	Disable_Interpolation : 1 = false;
        bool 	Equip_Not_OK : 1 = false;
        bool 	Force_Copy_Bone_Weights : 1 = false;
        bool 	Force_Idle_Stop : 1 = false;
        bool 	Full_Animation_No_Physics : 1 = false;
        bool 	Graph_Supports_Strafe : 1 = false;
        bool 	Is_Attack_Not_Ready : 1 = false;
        bool 	Is_Busy : 1 = false;
        bool 	Is_In_Flavor : 1 = false;
        bool 	Is_Jumping : 1 = false;
        bool 	Is_Reloading : 1 = false;
        bool 	Is_Sprinting : 1 = false;
        bool 	Is_Synced : 1 = false;
        bool 	Manual_Graph_Change : 1 = false;
        bool 	Motion_Driven : 1 = false;
        bool 	No_IK : 1 = false;
        bool 	Pathing_Interruptible_Idle : 1 = false;
        bool 	Pipboy_Up : 1 = false;
        bool 	Render_1st_Person_in_World : 1 = false;
        bool 	Should_Pivot_to_Camera : 1 = false;
        bool 	Speed_Synced : 1 = false;
        bool 	Supported_Death_Anim : 1 = false;
        bool 	Turn_Animation : 1 = false;
        bool 	Want_Sprint : 1 = false;
        bool 	Wants_Full_Ragdoll : 1 = false;
        char    paddingc : 7 = '\0';
        char    padding[3] = {};
    };

    AgxAnimationFlags() = default;
    ~AgxAnimationFlags() override = default;

    Data data;
    size_t GetValue() const override;
    void SetValue(size_t input) override;
    static QStringList GetStringList(bool spaces = true);
    QString ToString() const override;

    // Inherited via AgxFlagField
    QJsonValue ToJson() const override;
    void FromJson(const QJsonValue& data) override;
    pugi::xml_node ToXML() override;
    void FromXML(pugi::xml_node& node) override;

public:
    inline bool IsEnabledState() const override { return _propertyEnabled; }
    inline void SetEnabledState(bool state) override {
        _propertyEnabled = state;
        Q_EMIT StateUpdated(_propertyEnabled);
    }

private:
    bool _propertyEnabled = true;
};

struct AgxEventInfo {
    AgxEventType eventType = AgxEventType::UNDEFINED;
    AgxTrueFalse booleanField = AgxTrueFalse::BLANK;
};

//Q_DECLARE_METATYPE(AgxEventInfo)

const extern QHash<QString, AgxEventInfo>* AgxEventVars;

const extern QStringList AgxAnimPrefixes;
const extern QStringList AgxAnimSuffixes;

const extern QStringList AgxAnimTriggers;

const extern QStringList AgxSyncs;

//const extern std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();
enum class AgxGraphType : uint8_t {
    SFBGS_Default,
    SFBGS_StateMachine,

    UNDEFINED = 0xFF
};

QString AgxGraphTypeAsString(const AgxGraphType& type);
AgxGraphType AgxGetDefaultGraphType(const AgxGameType& type);

class AgxPropertyEntryDefinition  : public QObject
{
    Q_OBJECT
public:
    QString value;
    AgxColumnTypes columnType = AgxColumnTypes::BasicString;

    bool propertyEnabled = true;
    bool isPresent = true;

private:
    QList<TermRef> customDropDownList = {};
    TermRef _tagReference = nullptr;

public:
    AgxPropertyEntryDefinition(TermRef ref, const QString& value, const AgxColumnTypes& columnType = AgxColumnTypes::BasicString, const QList<TermRef>& customDropDownList = {}, bool isPresent = true)
        : _tagReference(ref), value(value), columnType(columnType), isPresent(isPresent) {
        for (auto entry : customDropDownList) {
            if (entry)
                this->customDropDownList.append(entry);
            else
                this->customDropDownList.append(&AgxDictionary::ErrorTerm);
        }
    }

    AgxPropertyEntryDefinition(const AgxPropertyEntryDefinition& other) {
        _tagReference = other._tagReference;
        value = other.value;
        columnType = other.columnType;
        customDropDownList = other.customDropDownList;
        propertyEnabled = other.propertyEnabled;
        isPresent = other.isPresent;
    }

    inline AgxPropertyEntryDefinition& operator=(const AgxPropertyEntryDefinition& other) {
        _tagReference = other._tagReference;
        value = other.value;
        columnType = other.columnType;
        customDropDownList = other.customDropDownList;
        propertyEnabled = other.propertyEnabled;
        isPresent = other.isPresent;
        return *this;
    }

    inline QString Tag() const { 
        if (_tagReference) 
            return _tagReference().tag; 

        return AgxDictionary::ErrorTerm().tag;
    }

    inline QString Label() const {
        if (_tagReference)
            return _tagReference().translation;

        return AgxDictionary::ErrorTerm().translation;
    }

    inline QList<TermRef> CustomDropDownList() const { return customDropDownList; }

signals:
    void StateUpdated(bool enabled);
    void PresentUpdated(bool enabled);

public:
    inline void SetEnabledState(bool state) {
        propertyEnabled = state;
        Q_EMIT StateUpdated(propertyEnabled);
    }

    inline void SetIsPresent(bool state) {
        isPresent = state;
        Q_EMIT PresentUpdated(isPresent);
    }
};

QString GetPropertyValue(const QVector<AgxPropertyEntryDefinition>& list, const QString& tag, const QString& defValue = QString());
bool SetPropertyValue(QVector<AgxPropertyEntryDefinition>& list, const QString& tag, const QString& value);

class AgxPropertyBlockData : public QObject
{
    Q_OBJECT

public:
    struct Entry {
        QString Value;
        AgxColumnTypes Type;
    };

    AgxPropertyBlockData(const QList<AgxPropertyEntryDefinition>& types = QList<AgxPropertyEntryDefinition>({ AgxPropertyEntryDefinition(&AgxDictionary::ErrorTerm,"",AgxColumnTypes::Default) }), QObject* parent = nullptr);
    AgxPropertyBlockData(const AgxPropertyBlockData& other);

    AgxPropertyBlockData& operator=(const AgxPropertyBlockData& other);

    void AddRow(int index = 0);
    void RemoveRow(int index);
    bool HasRow(int index) const;
    void Reset();

    QList<Entry>* GetRow(int index);
    unsigned int GetRowCount() const;

    AgxColumnTypes GetColumnType(int column) const;
    unsigned int GetColumnCount() const;

    AgxPropertyEntryDefinition GetColumnDefinition(int column) const;

    void insertPropertyBlockData(const QJsonObject& data);
    QJsonObject getPropertyBlockData(bool cleared = false) const;
    virtual void loadDefault(const QJsonObject& blockData);
    void SetEnabledState(bool state);
    inline bool IsEnabledState() const { return propertyEnabled; }

    void load(pugi::xml_node& blockNode);

signals:
    void RowAdded(int index);
    void RowRemoved(int index);
    void DataUpdated();
    void StateUpdated(bool enabled);

public:
    QList<Entry> GetDefaultRow() const;
    void SetRow(int index, const QList<Entry>& data = {});
    void SetRow(int index, const QStringList& data);


private:
    QList<AgxPropertyEntryDefinition> _columnDefinitions;
    QList<QList<Entry>> _data;
    bool propertyEnabled = true;
};


struct AgxGraphDefinition {
    QMap<TermRef, AgxPropertyBlockData> _defaultBlocks;
    QVector<AgxPropertyEntryDefinition> _defaultEntries;
    QList<TermRef> _blockOrder;
    QHash<QString, QString> _tags;
    bool _hasDefault = false;
};

enum class AgxNodeRole {
    Type = 0,             ///< Type of the current node, usually a string.
    Position = 1,         ///< `QPointF` positon of the node on the scene.
    Size = 2,             ///< `QSize` for resizable nodes.
    CaptionVisible = 3,   ///< `bool` for caption visibility.
    Caption = 4,          ///< `QString` for node caption.
    Style = 5,            ///< Custom NodeStyle as QJsonDocument
    InternalData = 6,     ///< Node-stecific user data as QJsonObject
    InPortCount = 7,      ///< `unsigned int`
    OutPortCount = 9,     ///< `unsigned int`
    Widget = 10,          ///< Optional `QWidget*` or `nullptr`
    ValidationState = 11, ///< Enum NodeValidationState of the node
    AlternateState = 12,  ///< 'bool'
    SubCaption = 13,      ///< 'QString'
    CollapseState = 14   ///< 'bool'
};

QString ShortenString(const QString& string, int maxLength = 10);
//Q_ENUM_NS(AgxNodeRole)

enum class AgxPortRole {
    Data = 0,                 ///< `std::shared_ptr<NodeData>`.
    DataType = 1,             ///< `QString` describing the port data type.
    ConnectionPolicyRole = 2, ///< `enum` ConnectionPolicyRole
    CaptionVisible = 3,       ///< `bool` for caption visibility.
    Caption = 4,              ///< `QString` for port caption.
    InternalData = 5
};

enum class AgxConnectionPolicy {
    One,
    Many
};


enum class AgxGraphicsItemsFlag
{
    None =          0x0,
    Node =          0x1,
    Connection =    0x2,

    All = Node | Connection
};

using AgxGraphicsItemsFlags = QFlags<AgxGraphicsItemsFlag>;


enum class AgxNodeFlag {
    NoFlags =   0b00,
    Resizable = 0b01,
    Locked =    0b10
};

using AgxNodeFlags = QFlags<AgxNodeFlag>;

//Q_DECLARE_FLAGS(AgxNodeFlags, AgxNodeFlag)
//Q_FLAG_NS(AgxNodeFlags)
//Q_DECLARE_OPERATORS_FOR_FLAGS(AgxNodeFlags)

struct AgxConnectionId {
    AgxNodeId outNodeId;
    AgxPortIndex outPortIndex;
    AgxNodeId inNodeId;
    AgxPortIndex inPortIndex;
    bool isHidden = false;
};

inline bool operator==(const AgxConnectionId& a, const AgxConnectionId& b) {
    return a.outNodeId == b.outNodeId && a.outPortIndex == b.outPortIndex
        && a.inNodeId == b.inNodeId && a.inPortIndex == b.inPortIndex;
}

inline bool operator!=(const AgxConnectionId& a, const AgxConnectionId& b)
{
    return !(a == b);
}

inline void invertConnection(AgxConnectionId& id)
{
    qSwap(id.outNodeId, id.inNodeId);
    qSwap(id.outPortIndex, id.inPortIndex);
}