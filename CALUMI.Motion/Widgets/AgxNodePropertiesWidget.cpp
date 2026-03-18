//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNodePropertiesWidget.h"
#include "Utilities/AgxDefinitions.h"
#include "Widgets/AgxLineEdit.h"
#include <Utilities/DialogPool.h>
#include "Models/AgxGraphicsScene.h"
#include "Utilities/AgxJsonHelper.h"
#include "Utilities/UndoRedoCommands.h"
#include "AgxPropertyBlockWidget.h"
#include "Utilities/PropertySheetEntryUtil.h"
#include "AgxNodes/AgxNode.h"
#include "Models/AgxPort.h"
#include "CALUMIMotion.h"
#include "MiniGraphicsView.h"
#include "Widgets/Dialog/AgxSimpleDialog.h"

AgxNodePropertiesWidget::AgxNodePropertiesWidget(QWidget* parent, bool bblockSignals) : QWidget(parent), IAgxEmbedSceneData()
{
	blockSignals(bblockSignals);

	_MainVBoxLayout = new QVBoxLayout();
	_LeftFormLayout = new QFormLayout();
	_hiddenLeftFormLayout = new QFormLayout();
	_RightFormLayout = new QFormLayout();
	_hiddenRightFormLayout = new QFormLayout();
	QHBoxLayout* formLayout = new QHBoxLayout();
	_hiddenLayout = new QHBoxLayout();
	formLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);
	_hiddenLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);

	_LeftFormLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);
	_hiddenLeftFormLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);
	_hiddenLeftFormLayout->setHorizontalSizeConstraint(QLayout::SetMinimumSize);
	_RightFormLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);
	_hiddenRightFormLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);
	_hiddenRightFormLayout->setHorizontalSizeConstraint(QLayout::SetMinimumSize);
	formLayout->addLayout(_LeftFormLayout);
	_hiddenLayout->addLayout(_hiddenLeftFormLayout);
	formLayout->addLayout(_RightFormLayout);
	_hiddenLayout->addLayout(_hiddenRightFormLayout);
	_MainVBoxLayout->addLayout(_hiddenLayout);
	_MainVBoxLayout->addLayout(formLayout);
	_MainVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);
	_MainVBoxLayout->setHorizontalSizeConstraint(QLayout::SetMinimumSize);
	setLayout(_MainVBoxLayout);
	setContentsMargins(3, 3, 3, 3);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	setMinimumSize(10, 10);
	QString objname = "AgxNodeProp";
	setObjectName(objname);
	setStyleSheet("QWidget#" + objname + " {background-color: transparent; padding: 1px;}");

	SendWidthAdjustment();
}

AgxNodePropertiesWidget::~AgxNodePropertiesWidget()
{
}

void AgxNodePropertiesWidget::SetUpCustomDropDown(AgxLineEditContainer* line, const QList<TermRef>& list, const QStringList& keyPath)
{
	connect(line, &AgxLineEditContainer::ContentDoubleClicked, line, [this,line,list, keyPath]() {
						
						QStringList stringList;
						int current = 0;

						for (auto term : list) {
							if (term && term().tag.compare(line->text(), Qt::CaseInsensitive) == 0)
								current = list.indexOf(term);
						}

						if (auto result = AgxSimpleDialog::GetDropDown(this, "Select Entry", "", list, current, false)) {
							QJsonObject input;
							input["value"] = result().tag;
							SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
						}

				  });

	SendWidthAdjustment();
}

QList<AgxLineEditContainer*> AgxNodePropertiesWidget::CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxNode* signalSender, bool split)
{
	QList<AgxLineEditContainer*> outputList;
	for (int i = 0; i < dataRef->size(); i++)
	{
		auto& dataRefItem = (*dataRef)[i];
		QString key = dataRefItem.Tag();

		auto output = AgxWidgetUtil::CreateEntry(dataRefItem.columnType, {key}, this);


		output->setContentText(dataRefItem.value);

		if (dataRefItem.columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(output, dataRefItem.CustomDropDownList(), {key});
		}

		if (split && !_nextEntryLeft)
		{
			_RightFormLayout->addRow(dataRefItem.Label(), output);
			_RightFormLayout->setRowVisible(output, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, output](bool enabled) {
						_RightFormLayout->setRowVisible(output, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_RightFormLayout->labelForField(output)))
			{
				label->setEnabled(dataRefItem.isPresent);
				output->setContentState(dataRefItem.isPresent);
				
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output](bool enabled) {
						label->setEnabled(enabled);
						output->setContentState(enabled);
						});
				
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
							label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = true;
		} else {
			_LeftFormLayout->addRow(dataRefItem.Label(), output);
			_LeftFormLayout->setRowVisible(output, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, output](bool enabled) {
						_LeftFormLayout->setRowVisible(output, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_LeftFormLayout->labelForField(output)))
			{
				label->setEnabled(dataRefItem.isPresent);
				output->setContentState(dataRefItem.isPresent);
				
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output](bool enabled) {
						label->setEnabled(enabled);
						output->setContentState(enabled);
						});

				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
					label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = false;
		}
		
		output->setContentState(dataRefItem.isPresent);
		
		connect(output, &AgxLineEditContainer::ContentStateChanged, &dataRefItem, [this, &dataRefItem, key](bool enabled) {
					QJsonObject input;
					input["isPresent"] = enabled;
					SendInsertPropertySheetDataCommand(QStringListToQJsonObject({ key }, input));
				});

		connect(signalSender, &AgxNode::PropertySheetUpdated, output, [output, &dataRefItem]() {
			output->setContentText(dataRefItem.value);
			output->RefreshContentTooltip(dataRefItem.value);
				});
		

		outputList.append(output);
	}


	SendWidthAdjustment();
	return outputList;
}

QList<AgxLineEditContainer*> AgxNodePropertiesWidget::CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxGraphModel* signalSender, bool split)
{
	QList<AgxLineEditContainer*> outputList;
	for (int i = 0; i < dataRef->size(); i++)
	{
		auto& dataRefItem = (*dataRef)[i];

		QString key = dataRefItem.Tag();
		auto output = AgxWidgetUtil::CreateEntry(dataRefItem.columnType, { key }, this);


		output->setContentText(dataRefItem.value);

		if (dataRefItem.columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(output, dataRefItem.CustomDropDownList(), { key });
		}

		QLabel* entryLabel = new QLabel(dataRefItem.Label());

		if (split && !_nextEntryLeft)
		{
			_RightFormLayout->addRow(entryLabel, output);
			_RightFormLayout->setRowVisible(output, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, output](bool enabled) {
				_RightFormLayout->setRowVisible(output, enabled);
					});
			//if (QLabel* label = dynamic_cast<QLabel*>(_RightFormLayout->labelForField(output)))
			{
				//checkBox->setChecked(dataRefItem.isPresent);
				entryLabel->setEnabled(dataRefItem.isPresent);
				//output->setEnabledState(dataRefItem.isPresent);
				output->setContentState(dataRefItem.isPresent);
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, entryLabel, [this, entryLabel, output](bool enabled) {
					
					entryLabel->setEnabled(enabled);
					output->setContentState(enabled);
					
						});

				connect(this, &AgxNodePropertiesWidget::LanguageChanged, entryLabel, [entryLabel, &dataRefItem]() {
					
					entryLabel->setText(dataRefItem.Label());
					
						});
			}
			_nextEntryLeft = true;
		} else {
			_LeftFormLayout->addRow(dataRefItem.Label(), output);
			_LeftFormLayout->setRowVisible(output, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _LeftFormLayout, [this, output](bool enabled) {
				_LeftFormLayout->setRowVisible(output, enabled);
					});
			//if (QLabel* label = dynamic_cast<QLabel*>(_LeftFormLayout->labelForField(output)))
			{
				entryLabel->setEnabled(dataRefItem.isPresent);
				output->setContentState(dataRefItem.isPresent);
				
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, entryLabel, [this, entryLabel, output](bool enabled) {
					
					entryLabel->setEnabled(enabled);
					output->setContentState(enabled);
					
						});
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, entryLabel, [entryLabel, &dataRefItem]() {
					entryLabel->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = false;
		}

		output->setContentState(dataRefItem.isPresent);

		connect(output, &AgxLineEditContainer::ContentStateChanged, &dataRefItem, [this, &dataRefItem, key](bool enabled) {
			QJsonObject input;
			input["isPresent"] = enabled;
			SendInsertPropertySheetDataCommand(QStringListToQJsonObject({ key }, input));
				});

		connect(signalSender, &AgxGraphModel::PropertySheetUpdated, output, [output, dataRef, i]() {
			if (dataRef->size() <= i) return;

			output->setContentText(dataRef->at(i).value);
			output->RefreshContentTooltip(dataRef->at(i).value);
				});
		outputList.append(output);
	}

	SendWidthAdjustment();
	return outputList;
}

QList<AgxLineEditContainer*> AgxNodePropertiesWidget::CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxPort* signalSender, bool split, QStringList path)
{
	path.append("temp");

	QList<AgxLineEditContainer*> outputList;
	for (int i = 0; i < dataRef->size(); i++)
	{
		auto& dataRefItem = (*dataRef)[i];

		QString key = dataRefItem.Tag();
		
		path.back() = key;

		auto output = AgxWidgetUtil::CreateEntry(dataRefItem.columnType, path, this);

		output->setContentText(dataRefItem.value);

		if (dataRefItem.columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(output, dataRefItem.CustomDropDownList(), path);
		}

		if (split && !_nextEntryLeft)
		{
			_RightFormLayout->addRow(dataRefItem.Label(), output);
			_RightFormLayout->setRowVisible(output, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, output](bool enabled) {
				_RightFormLayout->setRowVisible(output, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_RightFormLayout->labelForField(output)))
			{

				label->setEnabled(dataRefItem.isPresent);
				output->setContentState(dataRefItem.isPresent);

				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output](bool enabled) {
					
					label->setEnabled(enabled);
					output->setContentState(enabled);
					
						});
				
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
				
					label->setText(dataRefItem.Label());
					
						});
			}
			_nextEntryLeft = true;
		} else {
			_LeftFormLayout->addRow(dataRefItem.Label(), output);
			_LeftFormLayout->setRowVisible(output, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _LeftFormLayout, [this, output](bool enabled) {
				_LeftFormLayout->setRowVisible(output, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_LeftFormLayout->labelForField(output)))
			{
				label->setEnabled(dataRefItem.isPresent);
				output->setContentState(dataRefItem.isPresent);
				
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output](bool enabled) {
				
					label->setEnabled(enabled);
					output->setContentState(enabled);
					
						});
				
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
				
					label->setText(dataRefItem.Label());
					
						});
			}
			_nextEntryLeft = false;
		}

		output->setContentState(dataRefItem.isPresent);

		connect(output, &AgxLineEditContainer::ContentStateChanged, &dataRefItem, [this, &dataRefItem, path](bool enabled) {
			
			QJsonObject input;
			input["isPresent"] = enabled;
			SendInsertPropertySheetDataCommand(QStringListToQJsonObject(path, input));
			
				});

		connect(signalSender, &AgxPort::PropertySheetUpdated, output, [output, &dataRefItem]() {

			output->setContentText(dataRefItem.value);
			output->RefreshContentTooltip(dataRefItem.value);

				});

		outputList.append(output);
	}

	SendWidthAdjustment();
	return outputList;
}

QList<QLabel*> AgxNodePropertiesWidget::CreateHiddenEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, AgxNode* signalSender, bool split, const QList<TermRef>& priorityOrder)
{
	QList<QLabel*> outputList;

	auto keyOrder = priorityOrder;

	for (auto& keyTest : priorityOrder) {
		if (!dataRef->keys().contains(keyTest)) keyOrder.removeAll(keyTest);
	}

	for (auto& nonPrioKey : dataRef->keys()) {
		if (!keyOrder.contains(nonPrioKey)) keyOrder.append(nonPrioKey);
	}

	for (int i = 0; i < keyOrder.size(); i++)
	{
		auto key = keyOrder.at(i);
		QLabel* entry = new QLabel(dataRef->value(key).second);
		QLabel* label = new QLabel(key().translation);

		//bool flip = _hiddenRightFormLayout->rowCount() < _hiddenLeftFormLayout->rowCount();

		if (split && !_nextEntryLeft)
		{
			_hiddenRightFormLayout->addRow(label, entry);
			_nextEntryLeft = true;
		} else {
			_hiddenLeftFormLayout->addRow(label, entry);
			_nextEntryLeft = false;
		}
		connect(signalSender, &AgxNode::PropertySheetUpdated, entry, [entry, dataRef, key]() {
			entry->blockSignals(true);
			entry->setText(dataRef->value(key).second);
			entry->blockSignals(false);
				});

		connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, key]() {
			label->setText(key().translation);
				});

		entry->setEnabled(false);
		outputList.append(entry);
	}

	SendWidthAdjustment();
	return outputList;
}

QList<QLabel*> AgxNodePropertiesWidget::CreateHiddenEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, AgxGraphModel* signalSender, bool split, const QList<TermRef>& priorityOrder)
{
	QList<QLabel*> outputList;

	auto keyOrder = priorityOrder;

	for (auto& keyTest : priorityOrder) {
		if (!dataRef->keys().contains(keyTest)) keyOrder.removeAll(keyTest);
	}

	for (auto& nonPrioKey : dataRef->keys()) {
		if (!keyOrder.contains(nonPrioKey)) keyOrder.append(nonPrioKey);
	}

	for (int i = 0; i < keyOrder.size(); i++)
	{
		auto& key = keyOrder.at(i);
		QLabel* entry = new QLabel(dataRef->value(key).second);
		QLabel* label = new QLabel(key().translation);

		bool flip = _hiddenRightFormLayout->rowCount() < _hiddenLeftFormLayout->rowCount();

		if (split && i % 2 != static_cast<int>(flip))
		{
			_hiddenRightFormLayout->addRow(label, entry);
		} else {
			_hiddenLeftFormLayout->addRow(label, entry);
		}
		connect(signalSender, &AgxGraphModel::PropertySheetUpdated, entry, [entry, dataRef, key]() {
			entry->blockSignals(true);
			entry->setText(dataRef->value(key).second);
			entry->blockSignals(false);
				});

		connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, key]() {
			label->setText(key().translation);
				});

		entry->setEnabled(false);
		outputList.append(entry);
	}

	SendWidthAdjustment();
	return outputList;
}

QLabel* AgxNodePropertiesWidget::CreateGuidLabel(const QUuid* value, AgxNode* signalSender, bool split)
{
	QLabel* output = new QLabel(value->toString(QUuid::StringFormat::WithoutBraces));

	//bool flip = _hiddenRightFormLayout->rowCount() < _hiddenLeftFormLayout->rowCount();
	if (split && !_nextEntryLeft)
	{
		_hiddenRightFormLayout->addRow("Guid", output);
		_nextEntryLeft = true;
	} else {
		_hiddenLeftFormLayout->addRow("Guid", output);
		_nextEntryLeft = false;
	}
	connect(signalSender, &AgxNode::PropertySheetUpdated, output, [output, value]() {
		output->blockSignals(true);
		output->setText(value->toString(QUuid::StringFormat::WithoutBraces));
		output->blockSignals(false);
			});
	output->setEnabled(false);

	SendWidthAdjustment();
	return output;
}

ModifiedPushButton* AgxNodePropertiesWidget::CreateFlagEntry(const QString& title, AgxNode* signalSender, AgxFlagField* dataRef)
{
	ModifiedPushButton* button = new ModifiedPushButton("Edit");
	button->setMinimumWidth(200);
	button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	QString toolTipColored = dataRef->ToString();
	toolTipColored.replace("\n", "<br>");
	toolTipColored.replace("False", "<font color='dimgrey'>False</font>",Qt::CaseInsensitive);
	toolTipColored.replace("True", "<font color='plum'>True</font>",Qt::CaseInsensitive);
	button->setToolTipDuration(-1);
	button->setToolTip(toolTipColored);
	
	connect(button, &ModifiedPushButton::CustomPressSignal, this, [this, dataRef]() {
				auto dialog = DialogPool_SFBGS::GetInstance().GetAnimationFlagDialog(dataRef->GetValue());
				
				if(dialog->exec() == QDialog::Accepted)
				{
					QJsonObject obj;
					AgxAnimationFlags flags;
					flags.SetValue(dialog->GetValues());
					obj["flags"] = flags.ToJson();
					_scene->undoStack().push(new InsertPropertySheetDataCommand(_scene, _nodeId, obj));
					//dataRef->SetValue(dialog->GetValues());
				}
				
			});
	connect(signalSender, &AgxNode::PropertySheetUpdated, this, [this, dataRef, button]()
			{
				QString toolTipColored = dataRef->ToString();
				toolTipColored.replace("\n", "<br>");
				toolTipColored.replace("False", "<font color='dimgrey'>False</font>", Qt::CaseInsensitive);
				toolTipColored.replace("True", "<font color='plum'>True</font>", Qt::CaseInsensitive);
				button->setToolTip(toolTipColored);
				if (dataRef->GetValue() > 0) button->setText(std::format("Edit ({})",dataRef->GetValue()).c_str());
				else button->setText("Edit");
			});

	_LeftFormLayout->addRow(title, button);
	button->setEnabled(dataRef->IsEnabledState());
	connect(dataRef, &AgxFlagField::StateUpdated, button, [this, button](bool enabled) {
				button->setEnabled(enabled);
			});
	_nextEntryLeft = false;

	SendWidthAdjustment();
	return button;
}

AgxLineEditContainer* AgxNodePropertiesWidget::CreateSimpleLineEdit(QString* sourceData, AgxPort* signalSender, TermRef label, bool split, QStringList path)
{
	AgxLineEditContainer* output = AgxWidgetUtil::CreateEntry(AgxColumnTypes::BasicString, path, this);
	output->blockSignals(true);
	output->setContentText(*sourceData);
	output->blockSignals(false);
	
	QLabel* labelObj = new QLabel(QString());

	if (label) {
		labelObj->setText(label().translation);
	}

	//bool flip = _hiddenRightFormLayout->rowCount() < _hiddenLeftFormLayout->rowCount();
	if (split && !_nextEntryLeft)
	{
		_hiddenRightFormLayout->addRow(labelObj, output);
		_nextEntryLeft = true;
	} else {
		_hiddenLeftFormLayout->addRow(labelObj, output);
		_nextEntryLeft = false;
	}
	connect(signalSender, &AgxPort::PropertySheetUpdated, output, [output, sourceData]() {
							output->blockSignals(true);
							output->setContentText(*sourceData);
							output->blockSignals(false);
																						 });

	connect(this, &AgxNodePropertiesWidget::LanguageChanged, labelObj, [labelObj, label]() { 
							if (label) {
								labelObj->setText(label().translation);
							}
																						   });

	SendWidthAdjustment();
	return output;
}

AgxPropertyBlockWidget* AgxNodePropertiesWidget::CreatePropetryBlock(TermRef blockTitleRef, AgxPropertyBlockData& dataRef)
{
	AgxPropertyBlockWidget* block = new AgxPropertyBlockWidget(blockTitleRef, dataRef, this);
	_MainVBoxLayout->addWidget(block,1, Qt::AlignHCenter);
	_PropertyBlocks.insert(blockTitleRef, block);
	_MainVBoxLayout->addStretch(1);

	block->setVisible(dataRef.IsEnabledState());
	connect(&dataRef, &AgxPropertyBlockData::StateUpdated, block, [this, block](bool enabled) {
				block->setVisible(enabled);
			});

	connect(block, &AgxPropertyBlockWidget::RowsChanged, this, &AgxNodePropertiesWidget::SendWidthAdjustment);

	SendWidthAdjustment();
	return block;
}

void AgxNodePropertiesWidget::CreateEmbeddedNodeGraphButton(std::shared_ptr<AgxGraphicsScene> scene, std::shared_ptr<AgxGraphModel> model)
{
	
		MiniGraphicsView* miniView = new MiniGraphicsView(scene.get());
		_MainVBoxLayout->addWidget(miniView);
		miniView->setMinimumHeight(200);
		miniView->setMinimumWidth(200);
		miniView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

		connect(miniView, &MiniGraphicsView::clicked, this, [this, scene, model]() {
			
			auto widget = QApplication::activeWindow();

			if (auto calumiWindow = dynamic_cast<CALUMIMotion*>(widget))
				calumiWindow->Create_SFBGSTab(scene, model); 
			
				});
	
	SendWidthAdjustment();
	return;
}

void AgxNodePropertiesWidget::FinalizeWidget()
{
	blockSignals(false);
	SendWidthAdjustment();
}

void AgxNodePropertiesWidget::mousePressEvent(QMouseEvent* event)
{
	setFocus();

	QWidget::mousePressEvent(event);
}

void AgxNodePropertiesWidget::SetHiddenEntries(bool hidden)
{
	for (int i = 0; i < _hiddenLeftFormLayout->rowCount(); i++)
	{
		_hiddenLeftFormLayout->setRowVisible(i, hidden);
	}
	for (int i = 0; i < _hiddenRightFormLayout->rowCount(); i++)
	{
		_hiddenRightFormLayout->setRowVisible(i, hidden);
	}
	QTimer::singleShot(0, this, &AgxNodePropertiesWidget::ForceRefresh);
}

void AgxNodePropertiesWidget::ForceRefresh()
{
	updateGeometry();
	if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this))
	{
		iagx->RefreshNode();
	}
}

void AgxNodePropertiesWidget::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::LanguageChange) {
		Q_EMIT LanguageChanged();
	}

	QWidget::changeEvent(event);
}

void AgxNodePropertiesWidget::SendWidthAdjustment()
{
	adjustSize();
	Q_EMIT BroadcastWidth(size().width());
}
