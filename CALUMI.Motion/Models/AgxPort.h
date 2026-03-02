#pragma once
#pragma warning(push,0)
#include <QJsonObject>
#pragma warning(pop)

#include "Utilities/AgxDefinitions.h"
//#include "Widgets/AgxNodePropertiesWidget.h"
#include "Widgets/SFBGS/SFBGS_SidebarContent.h"

class AgxNode;
class AgxBlendInputModel;

//using an abstract class in the case of other games/features being added that don't use Starfields additional id entry
class AgxPort : public QObject
{
	Q_OBJECT
public:
	AgxPort(AgxNode* parent) : _parentNode(parent) {}
	virtual ~AgxPort() = default;

	virtual AgxPortIndex GetPortIndex();
	virtual void Load(const QJsonObject& data) {}
	virtual void Load(pugi::xml_node& portNode) {}
	virtual void InsertData(const QJsonObject& data) {}
	virtual QJsonObject Save() const { return QJsonObject(); }
	virtual inline bool HasCaption() const { return false; }
	virtual inline QString Caption() const { return "Debug"; }
	virtual inline QWidget* GetEmbeddedWidget() { return nullptr; };
	virtual inline bool HasPropertySheet() { return false; }
	virtual inline void SetId(unsigned int id) { _portId = id; }
	virtual inline unsigned int GetId() const { return _portId; }

	virtual inline void setConnectionState(bool state) {}

	virtual inline void externalCommand(const QString& commandTag, const QString& payload) {}

Q_SIGNALS:
	void PropertySheetUpdated();

protected:
	AgxNode* _parentNode;
	AgxPortId _portId = InvalidAgxPortId;
};



#pragma region SFBGS
class AgxPort_SFBGS : public AgxPort
{
	Q_OBJECT
public:
	AgxPort_SFBGS(AgxNode* parent);
	~AgxPort_SFBGS();

	void SetName(const QString& str);
	inline QString GetName() const { return name; }

	void InsertData(const QJsonObject& data) override;
	void Load(const QJsonObject& data) override;
	void Load(pugi::xml_node& portNode) override;
	QJsonObject Save() const override;

	inline bool HasCaption() const override { return _portId != InvalidAgxPortId; }
	QString Caption() const override;

	void SetPropertySheetEnabled(bool state);

	inline bool GetPropertySheetEnabled() const { return _PropertyEntriesEnabled; }
	void SetPropertySheetOptional(bool initiallyEnabled = true);

	QWidget* GetEmbeddedWidget() override;

	AgxBlendInputModel* GetBlendInput() const;
	void AddBlendInput();

	inline void setConnectionState(bool state) override { _IsConnected = state; Q_EMIT PropertySheetUpdated(); }

	inline bool HasPropertySheet() override { return _PropertyEntries.count() != 0; }
	inline void AddStandardPropertySheet() {
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Event, "", AgxColumnTypes::Event));
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Comment, "", AgxColumnTypes::BasicString));
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Condition, "", AgxColumnTypes::BasicString));
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendTime, "0", AgxColumnTypes::BasicFloat));
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Priority, "0", AgxColumnTypes::BasicInteger));
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AllowSelfTransition, "False", AgxColumnTypes::BasicBool));
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DoNotFollowSyncGroup, "False", AgxColumnTypes::BasicBool));
		_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TransitionType, "Smooth", AgxColumnTypes::CustomDropDown, { "Smooth","Linear" }));
		_PropertyEntriesEnabled = false;
	}

	void externalCommand(const QString& commandTag, const QString& payload) override;

protected:
	//AgxPortId sfbgs_id = InvalidAgxPortId;
	QString name;

	//Property Sheets? (2 columns only it seems)
	QVector<AgxPropertyEntryDefinition> _PropertyEntries;
	bool _PropertyEntriesEnabled = true;
	bool _IsConnected = false;

	AgxBlendInputModel* _BlendInput = nullptr;

	//Property Sheet Widget
	QPointer<SFBGS_SidebarContentItem> _ContentWidget;
	//QPointer<QWidget> _EmbeddedWidget;
};

#pragma endregion


