//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QJsonObject>
#pragma warning(pop)

#include "Utilities/AgxDefinitions.h"
//#include "Widgets/AgxNodePropertiesWidget.h"
#include "Widgets/SFBGS/SFBGS_SidebarContent.h"

static const QVector<AgxPropertyEntryDefinition> g_dummyPropertyEntryVector = {};

class AgxNode;
class AgxBlendInputModel;

//using an abstract class in the case of other games/features being added that don't use Starfields additional id entry
class AgxPort : public QObject
{
	Q_OBJECT
public:
	explicit AgxPort(AgxNode* parent) : m_parentNode(parent) {}
	~AgxPort() override = default;

	virtual AgxPortIndex getPortIndex();
	virtual void load(const QJsonObject& data) {}
	virtual void load(pugi::xml_node& portNode) {}
	virtual void insertData(const QJsonObject& data) {}
	virtual QJsonObject save() const { return QJsonObject(); }
	virtual bool hasCaption() const { return false; }
	virtual QString caption(bool formatted = true) const { return "Debug"; }
	virtual QWidget* getEmbeddedWidget() const { return nullptr; }
	virtual bool hasPropertySheet() const { return false; }
	virtual void setId(const unsigned int id) { m_portId = id; }
	virtual unsigned int getId() const { return m_portId; }

	virtual void setConnectionState(bool state) {}
	virtual bool isConnected() const { return false; }

	virtual void externalCommand(const QString& commandTag, const QString& payload) {}

	virtual const QVector<AgxPropertyEntryDefinition>& propertyEntries() { return g_dummyPropertyEntryVector; }
	virtual void savePropertySheet(pugi::xml_node& parent) {}

Q_SIGNALS:
	void propertySheetUpdated() const;

protected:
	AgxNode* m_parentNode;
	AgxPortId m_portId = InvalidAgxPortId;
};



#pragma region SFBGS
class AgxPort_SFBGS : public AgxPort
{
	Q_OBJECT
public:
	explicit AgxPort_SFBGS(AgxNode* parent);
	~AgxPort_SFBGS() override = default;

	void setName(const QString& str);
	QString getName() const { return m_name; }

	void insertData(const QJsonObject& data) override;
	void load(const QJsonObject& data) override;
	void load(pugi::xml_node& portNode) override;
	QJsonObject save() const override;

	bool hasCaption() const override { return m_portId != InvalidAgxPortId; }
	QString caption(bool formatted = true) const override;

	void setPropertySheetEnabled(bool state);

	bool getPropertySheetEnabled() const { return m_propertyEntriesEnabled; }
	void setPropertySheetOptional(bool initiallyEnabled = true);

	QWidget* getEmbeddedWidget() const override;

	AgxBlendInputModel* getBlendInput() const;
	void addBlendInput();

	void setConnectionState(const bool state) override { m_isConnected = state; Q_EMIT propertySheetUpdated(); }
	bool isConnected() const override { return m_isConnected; }

	bool hasPropertySheet() const override { return m_propertyEntries.count() != 0; }
	void addStandardPropertySheet() {
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Event, "", AgxColumnTypes::Event));
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Comment, "", AgxColumnTypes::BasicString));
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Condition, "", AgxColumnTypes::BasicString));
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendTime, "0", AgxColumnTypes::BasicFloat));
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Priority, "0", AgxColumnTypes::BasicInteger));
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AllowSelfTransition, "False", AgxColumnTypes::BasicBool));
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DoNotFollowSyncGroup, "False", AgxColumnTypes::BasicBool));
		m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TransitionType, AgxDictionary::Smooth().tag, AgxColumnTypes::CustomDropDown, {&AgxDictionary::Smooth,&AgxDictionary::Linear}));
		m_propertyEntriesEnabled = false;
	}

	void externalCommand(const QString& commandTag, const QString& payload) override;

	const QVector<AgxPropertyEntryDefinition>& propertyEntries() override { return m_propertyEntries; }
	void savePropertySheet(pugi::xml_node& parent) override;

protected:
	//AgxPortId sfbgs_id = InvalidAgxPortId;
	QString m_name;

	//Property Sheets? (2 columns only it seems)
	QVector<AgxPropertyEntryDefinition> m_propertyEntries;
	bool m_propertyEntriesEnabled = true;
	bool m_isConnected = false;

	AgxBlendInputModel* m_blendInput = nullptr;

};

#pragma endregion


