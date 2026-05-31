//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxPort.h"
#include "AgxNodes/AgxNode.h"
#include "Models/Blend/AgxBlendInputModel.h"
#include <Widgets/Blend/AgxBlendInputView.h>
#include <Widgets/AgxLineEdit.h>

#include "Utilities/AgxFormat.h"

AgxPort_SFBGS::AgxPort_SFBGS(AgxNode* parent) : AgxPort(parent)
{
}

void AgxPort_SFBGS::setName(const QString& str) {
	m_name = str;
	Q_EMIT propertySheetUpdated();
}

void AgxPort_SFBGS::insertData(const QJsonObject& data)
{
	AgxPort::insertData(data);

	if (data.contains("name"))
	{
		m_name = data["name"].toObject()["value"].toString("");
	}

	for (int i = 0; i < m_propertyEntries.size(); i++)
	{
		if (QString key = m_propertyEntries.at(i).Tag(); data.contains(key))
		{
			if (data[key].toObject().contains("value"))
				m_propertyEntries[i].value = data[key].toObject()["value"].toString();
			if (data[key].toObject().contains("isPresent"))
				m_propertyEntries[i].SetIsPresent(data[key].toObject()["isPresent"].toBool());
		}
	}

	if (data.contains("property-entries-enabled"))
	{
		m_propertyEntriesEnabled = data["property-entries-enabled"].toString().compare("True", Qt::CaseInsensitive) == 0;
	}

	if (data.contains("blend-input")) 
	{
		QJsonObject blendInput = data["blend-input"].toObject();
		for (qsizetype i = 0; i < blendInput.size(); i++)
		{
			bool ok = false;
			if (const int idx = blendInput.keys().at(i).toInt(&ok); ok && idx >= 0) {

				QJsonObject point = blendInput[std::to_string(idx).c_str()].toObject();
				if (point.contains("range")) m_blendInput->modifyRangeValue(idx, point["range"].toString());

				if (point.contains("weight")) m_blendInput->modifyWeightValue(idx, point["weight"].toString());
			}
		}
	}

	Q_EMIT propertySheetUpdated();
}

void AgxPort_SFBGS::load(const QJsonObject& data) {
	
	AgxPort::load(data);

	m_name = data["name"].toObject()["value"].toString(m_name);

	if (data.contains("agxPortId")) m_portId = data["agxPortId"].toString().toUInt();

	for (int i = 0; i < m_propertyEntries.size(); i++)
	{
		if (QString key = m_propertyEntries.at(i).Tag(); data.contains(key))
		{
			if (data[key].toObject().contains("value"))
				m_propertyEntries[i].value = data[key].toObject()["value"].toString();
			if (data[key].toObject().contains("isPresent"))
				m_propertyEntries[i].SetIsPresent(data[key].toObject()["isPresent"].toBool());
		}
	}

	if (data.contains("property-entries-enabled"))
	{
		m_propertyEntriesEnabled = data["property-entries-enabled"].toString().compare("True", Qt::CaseInsensitive) == 0;
	}

	if (data.contains("blend-input") && m_blendInput)
	{
		QJsonObject bInput = data["blend-input"].toObject();

		int maxIdx = -1;
		for (auto& point : bInput.keys()) {
			if (const int val = point.toInt(); val > maxIdx) maxIdx = val;
		}

		int count = m_blendInput->getDataRowCount();
		while (count < static_cast<size_t>(maxIdx) + 1) {
			m_blendInput->addDataRow(count);
			count = m_blendInput->getDataRowCount();
		}

		size_t idx = 0;
		for (auto point : bInput) {

			auto pointObj = point.toObject();
			QString rowKey = bInput.keys().at(idx);

			if (pointObj.contains("range")) m_blendInput->modifyRangeValue(rowKey.toInt(), pointObj["range"].toString());
			
			if (pointObj.contains("weight")) m_blendInput->modifyWeightValue(rowKey.toInt(), pointObj["weight"].toString());

			idx++;
		}

	}

	Q_EMIT propertySheetUpdated();
}

void AgxPort_SFBGS::load(pugi::xml_node& portNode)
{
	if (portNode.child("link").child("property_sheet")) {
		if (m_propertyEntries.isEmpty())
		{
			QMessageBox warning(
				QMessageBox::Warning,
				"Warning",
				QString("%1 Has Property Sheet But Was Missing Sheet In Constructor").arg(caption()),
				QMessageBox::Ok);

			warning.exec();
			
			addStandardPropertySheet();
		}
		m_propertyEntriesEnabled = true;

		for (auto& pEntry : m_propertyEntries) {
			pEntry.SetIsPresent(false);
		}

		if (const auto propertySheetXml = portNode.child("link").child("property_sheet"); QString("Property").compare(propertySheetXml.child("column").child_value("header"), Qt::CaseInsensitive) == 0)
		{
			for (auto& row : propertySheetXml.children("row"))
			{
				auto prop = row.first_child();
				auto value = row.last_child();
				for (auto& entry : m_propertyEntries)
				{
					if (entry.Tag() == prop.child_value("value"))
					{
						entry.value = value.child_value("value");
						entry.SetIsPresent(true);
						//propertySheet.remove_child(row);
						break;
					}
				}
			}
		}

	}
}

QJsonObject AgxPort_SFBGS::save() const {
	QJsonObject output = AgxPort::save();

	QJsonObject nObj;
	nObj["value"] = m_name;
	output["name"] = nObj;

	output["agxPortId"] = std::to_string(getId()).c_str();

	for (int i = 0; i < m_propertyEntries.size(); i++)
	{
		QString key = m_propertyEntries.at(i).Tag();
		QJsonObject valObj;
		valObj["value"] = m_propertyEntries[i].value;
		valObj["isPresent"] = m_propertyEntries[i].isPresent;
		output[key] = valObj;
	}

	output["property-entries-enabled"] = m_propertyEntriesEnabled ? "True" : "False";

	if (m_blendInput) {
		QJsonObject blendInput;
		for (size_t idx = 0; idx < m_blendInput->rowCount(); idx++) {
			QJsonObject obj;
			obj["range"] = m_blendInput->getRangeValue(static_cast<int>(idx));
			obj["weight"] = m_blendInput->getWeightValue(static_cast<int>(idx));
			blendInput[std::to_string(idx).c_str()] = obj;
		}
		output["blend-input"] = blendInput;
	}

	return output; 
}

QString AgxPort_SFBGS::caption(const bool formatted) const {

	if (!formatted)
		return m_name;

	if(!m_name.isEmpty())
		return QString("%1 (Id: %2)").arg(m_name).arg(m_portId);
		//return std::format("Id: {}", _portId).c_str();

	return QString("Id: %1").arg(m_portId);
}

void AgxPort_SFBGS::setPropertySheetEnabled(const bool state)
{
	m_propertyEntriesEnabled = state;
	Q_EMIT propertySheetUpdated();
}

void AgxPort_SFBGS::setPropertySheetOptional(const bool initiallyEnabled)
{
	if (!getEmbeddedWidget())
		return;

	m_propertyEntriesEnabled = initiallyEnabled;

	Q_EMIT propertySheetUpdated();
}

QWidget* AgxPort_SFBGS::getEmbeddedWidget() const
{
	if (!hasPropertySheet())
		return nullptr;

	auto contentWidget = new SFBGS_SidebarContentItem();

	connect(this, &QObject::destroyed, contentWidget, &QObject::deleteLater);

	const auto inputName = new AgxNodePropertiesWidget();

	const auto nameLine = inputName->CreateSimpleLineEdit(&m_name, this, nullptr, false, { "in-ports" , QString("%1").arg(m_portId), "name"});
	nameLine->setCheckbox(false);
	const QCheckBox tempBox;
	inputName->setContentsMargins(0, 0, tempBox.sizeHint().width() + 11, 0);
	nameLine->setContentPlaceholderText("Name");
	contentWidget->InsertAdditionalWidget(inputName, 1, Qt::AlignRight);

	const auto propSheet = contentWidget->SetupPropertySheet();

	propSheet->CreatePropertyEntries(&m_propertyEntries, this,
		{	"in-ports" ,
			QString("%1").arg(m_portId)}
	);

	connect(this, &AgxPort::propertySheetUpdated, contentWidget, [this, contentWidget]
	{
		contentWidget->SetTitle(QString("[Port] %1").arg(caption()));
		contentWidget->SetupOptionalPropertySheet(true, &m_propertyEntriesEnabled,
		{	"in-ports" ,
			QString("%1").arg(m_portId),
			"property-entries-enabled" });
		contentWidget->SetOptionalPropertySheetState(m_propertyEntriesEnabled && m_isConnected);
	});

	if (m_blendInput)
	{
		const auto tView = new AgxBlendInputView(m_blendInput);
		tView->SetBasePath({ "in-ports" , std::to_string(m_portId).c_str(), "blend-input" });
		contentWidget->InsertAdditionalWidget(tView, 1, Qt::AlignRight);
	}

	Q_EMIT m_parentNode->PropertySheetUpdated();
	Q_EMIT propertySheetUpdated();

	return contentWidget;
}

AgxBlendInputModel* AgxPort_SFBGS::getBlendInput() const
{
	return m_blendInput;
}

void AgxPort_SFBGS::addBlendInput()
{
	m_blendInput = new AgxBlendInputModel(this, this);
}

void AgxPort_SFBGS::externalCommand(const QString& commandTag, const QString& payload)
{
	if (commandTag == "blend-input-insert") {
		if (!m_blendInput) return;
		bool ok = false;
		int row = payload.toInt(&ok);
		if (!ok) row = m_blendInput->getDataRowCount();
		m_blendInput->addDataRow(row);
	}

	if (commandTag == "blend-input-remove") {
		if (!m_blendInput) return;
		bool ok = false;
		int row = payload.toInt(&ok);
		if (!ok) row = m_blendInput->getDataRowCount();
		m_blendInput->removeDataRow(row);
	}
}

void AgxPort_SFBGS::savePropertySheet(pugi::xml_node& parent)
{
	if (!m_propertyEntriesEnabled) return;

	FormatBasicPropertySheet(parent, m_propertyEntries);
}

AgxPortIndex AgxPort::getPortIndex() { if (m_parentNode) return m_parentNode->GetPortIndex(this); return InvalidPortIndex; }

