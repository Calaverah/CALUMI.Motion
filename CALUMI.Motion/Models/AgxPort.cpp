//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxPort.h"
#include "AgxNodes/AgxNode.h"
#include "Models/Blend/AgxBlendInputModel.h"
#include <Widgets/Blend/AgxBlendInputView.h>
#include <Widgets/AgxLineEdit.h>

AgxPort_SFBGS::AgxPort_SFBGS(AgxNode* parent) : AgxPort(parent)
{
}

AgxPort_SFBGS::~AgxPort_SFBGS()
{
	if (_ContentWidget) _ContentWidget->deleteLater();
}

void AgxPort_SFBGS::SetName(const QString& str) { 
	name = str;
	if(_ContentWidget)
		_ContentWidget->SetTitle(QString("[Port] %1").arg(Caption()));
}

void AgxPort_SFBGS::InsertData(const QJsonObject& data)
{
	AgxPort::InsertData(data);

	if (data.contains("name"))
	{
		name = data["name"].toObject()["value"].toString("");
	}

	for (int i = 0; i < _PropertyEntries.size(); i++)
	{
		QString key = _PropertyEntries.at(i).Tag();
		if (data.contains(key))
		{
			if (data[key].toObject().contains("value"))
				_PropertyEntries[i].value = data[key].toObject()["value"].toString();
			if (data[key].toObject().contains("isPresent"))
				_PropertyEntries[i].SetIsPresent(data[key].toObject()["isPresent"].toBool());
		}
	}

	if (data.contains("property-entries-enabled"))
	{
		_PropertyEntriesEnabled = data["property-entries-enabled"].toString().compare("True") == 0 ? true : false;
	}

	if (data.contains("blend-input")) 
	{
		QJsonObject blendInput = data["blend-input"].toObject();
		for (qsizetype i = 0; i < blendInput.size(); i++)
		{
			bool ok = false;
			int idx = blendInput.keys().at(i).toInt(&ok);
			if (ok && idx >= 0) {

				QJsonObject point = blendInput[std::to_string(idx).c_str()].toObject();
				if (point.contains("range")) _BlendInput->modifyRangeValue(idx, point["range"].toString());

				if (point.contains("weight")) _BlendInput->modifyWeightValue(idx, point["weight"].toString());
			}
		}
	}

	Q_EMIT PropertySheetUpdated();
}

void AgxPort_SFBGS::Load(const QJsonObject& data) {
	
	AgxPort::Load(data);

	name = data["name"].toObject()["value"].toString(name);

	if (data.contains("agxPortId")) _portId = data["agxPortId"].toString().toUInt();

	for (int i = 0; i < _PropertyEntries.size(); i++)
	{
		QString key = _PropertyEntries.at(i).Tag();
		if (data.contains(key))
		{
			if (data[key].toObject().contains("value"))
				_PropertyEntries[i].value = data[key].toObject()["value"].toString();
			if (data[key].toObject().contains("isPresent"))
				_PropertyEntries[i].SetIsPresent(data[key].toObject()["isPresent"].toBool());
		}
	}

	if (data.contains("property-entries-enabled"))
	{
		_PropertyEntriesEnabled = _stricmp("True", data["property-entries-enabled"].toString().toStdString().c_str()) == 0 ? true : false;
	}

	if (data.contains("blend-input") && _BlendInput)
	{
		QJsonObject bInput = data["blend-input"].toObject();

		int maxIdx = -1;
		for (auto& point : bInput.keys()) {
			int val = point.toInt();
			if (val > maxIdx) maxIdx = val;
		}

		int count = _BlendInput->getDataRowCount();
		while (count < static_cast<size_t>(maxIdx) + 1) {
			_BlendInput->addDataRow(count);
			count = _BlendInput->getDataRowCount();
		}

		size_t idx = 0;
		for (auto point : bInput) {

			auto pointObj = point.toObject();
			QString rowKey = bInput.keys().at(idx);

			if (pointObj.contains("range")) _BlendInput->modifyRangeValue(rowKey.toInt(), pointObj["range"].toString());
			
			if (pointObj.contains("weight")) _BlendInput->modifyWeightValue(rowKey.toInt(), pointObj["weight"].toString());

			idx++;
		}

	}

	Q_EMIT PropertySheetUpdated();
}

void AgxPort_SFBGS::Load(pugi::xml_node& portNode)
{
	if (portNode.child("link").child("property_sheet")) {
		if (_PropertyEntries.isEmpty()) {
			QMessageBox warning(QMessageBox::Warning, "Warning", std::format("{} Has Property Sheet But Was Missing Sheet In Constructor", Caption().toStdString().c_str()).c_str(),QMessageBox::Ok);
			warning.exec();
			AddStandardPropertySheet();
		}
		_PropertyEntriesEnabled = true;

		for (auto& pEntry : _PropertyEntries) {
			pEntry.SetIsPresent(false);
		}

		auto propertySheetXml = portNode.child("link").child("property_sheet");

		if (_stricmp(propertySheetXml.child("column").child_value("header"), "Property") == 0) {

			for (auto& row : propertySheetXml.children("row")) {
				auto prop = row.first_child();
				auto value = row.last_child();
				for (auto& entry : _PropertyEntries) {
					if (entry.Tag() == prop.child_value("value")) {
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

QJsonObject AgxPort_SFBGS::Save() const { 
	QJsonObject output = AgxPort::Save(); 

	QJsonObject nObj;
	nObj["value"] = name;
	output["name"] = nObj;

	output["agxPortId"] = std::to_string(GetId()).c_str();

	for (int i = 0; i < _PropertyEntries.size(); i++)
	{
		QString key = _PropertyEntries.at(i).Tag();
		QJsonObject valobj;
		valobj["value"] = _PropertyEntries[i].value;
		valobj["isPresent"] = _PropertyEntries[i].isPresent;
		output[key] = valobj;
	}

	output["property-entries-enabled"] = _PropertyEntriesEnabled ? "True" : "False";

	if (_BlendInput) {
		QJsonObject blendInput;
		for (size_t idx = 0; idx < _BlendInput->rowCount(); idx++) {
			QJsonObject obj;
			obj["range"] = _BlendInput->getRangeValue(static_cast<int>(idx));
			obj["weight"] = _BlendInput->getWeightValue(static_cast<int>(idx));
			blendInput[std::to_string(idx).c_str()] = obj;
		}
		output["blend-input"] = blendInput;
	}

	return output; 
}

QString AgxPort_SFBGS::Caption(bool formatted) const {

	if (!formatted) return name;

	if(!name.isEmpty())
		return QString("%1 (Id: %2)").arg(name).arg(_portId);
		//return std::format("Id: {}", _portId).c_str();

	return QString("Id: %1").arg(_portId);
}

void AgxPort_SFBGS::SetPropertySheetEnabled(bool state)
{
	_PropertyEntriesEnabled = state;
	Q_EMIT PropertySheetUpdated();
}

void AgxPort_SFBGS::SetPropertySheetOptional(bool initiallyEnabled)
{
	if (!GetEmbeddedWidget()) return;

	_PropertyEntriesEnabled = initiallyEnabled;
	_ContentWidget->SetupOptionalPropertySheet(true, &_PropertyEntriesEnabled, { "in-ports" , std::to_string(_portId).c_str(), "property-entries-enabled" });
	Q_EMIT PropertySheetUpdated();
	
}

QWidget* AgxPort_SFBGS::GetEmbeddedWidget()
{
	if (!HasPropertySheet()) return nullptr;

	if (!_ContentWidget)
	{
		_ContentWidget = new SFBGS_SidebarContentItem();
		connect(this, &AgxPort::PropertySheetUpdated, _ContentWidget, [this]() { _ContentWidget->SetTitle(std::format("[Port] {}", Caption().toStdString()).c_str()); });
		
		AgxNodePropertiesWidget* inputName = new AgxNodePropertiesWidget();
		auto nameLine = inputName->CreateSimpleLineEdit(&name, this, nullptr, false, { "in-ports" , std::to_string(_portId).c_str(), "name"});
		nameLine->setCheckbox(false);
		QCheckBox tempBox;
		inputName->setContentsMargins(0, 0, tempBox.sizeHint().width() + 11, 0);
		nameLine->setContentPlaceholderText("Name");
		_ContentWidget->InsertAdditionalWidget(inputName, 1, Qt::AlignRight);

		auto propSheet = _ContentWidget->SetupPropertySheet();
		
		propSheet->CreatePropertyEntries(&_PropertyEntries, this, false, { "in-ports" , std::to_string(_portId).c_str() });
		
		connect(this, &AgxPort::PropertySheetUpdated, _ContentWidget, [this, inputName]() {
			_ContentWidget->SetOptionalPropertySheetState(_PropertyEntriesEnabled && _IsConnected);
				});

		if (_BlendInput) {
			AgxBlendInputView* tView = new AgxBlendInputView(_BlendInput);
			tView->SetBasePath({ "in-ports" , std::to_string(_portId).c_str(), "blend-input" });
			_ContentWidget->InsertAdditionalWidget(tView, 1, Qt::AlignRight);
		}

		Q_EMIT _parentNode->PropertySheetUpdated();
		Q_EMIT PropertySheetUpdated();
	}

	return _ContentWidget;
}

AgxBlendInputModel* AgxPort_SFBGS::GetBlendInput() const
{
	return _BlendInput;
}

void AgxPort_SFBGS::AddBlendInput()
{
	_BlendInput = new AgxBlendInputModel(this, this);
}

void AgxPort_SFBGS::externalCommand(const QString& commandTag, const QString& payload)
{
	if (commandTag == "blend-input-insert") {
		if (!_BlendInput) return;
		bool ok = false;
		int row = payload.toInt(&ok);
		if (!ok) row = _BlendInput->getDataRowCount();
		_BlendInput->addDataRow(row);
	}

	if (commandTag == "blend-input-remove") {
		if (!_BlendInput) return;
		bool ok = false;
		int row = payload.toInt(&ok);
		if (!ok) row = _BlendInput->getDataRowCount();
		_BlendInput->removeDataRow(row);
	}
}

void AgxPort_SFBGS::SavePropertySheet(pugi::xml_node& parent)
{
	if (!_PropertyEntriesEnabled) return;

	FormatBasicPropertySheet(parent, _PropertyEntries);
}

AgxPortIndex AgxPort::GetPortIndex() { if (_parentNode) return _parentNode->GetPortIndex(this); return InvalidPortIndex; }

