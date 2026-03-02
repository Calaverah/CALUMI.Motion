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
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
	setMinimumSize(10, 10);
	QString objname = "AgxNodeProp";
	setObjectName(objname);
	setStyleSheet("QWidget{ padding: 1px;} QWidget#" + objname + " {background-color: transparent}");

	Q_EMIT BroadcastWidth(size().width());
}

AgxNodePropertiesWidget::~AgxNodePropertiesWidget()
{
}

void AgxNodePropertiesWidget::SetUpCustomDropDown(AgxLineEdit* line, const QStringList& list, const QStringList& keyPath)
{
	connect(line, &AgxLineEdit::DoubleClicked, line, [this,line,list, keyPath]() {
						auto popup = new MultiVariableDialog();

						if (list.isEmpty())
							popup->GetComboBox()->addItem("<none>");
						else
							popup->GetComboBox()->addItems(list);

						if (line->text().isEmpty()) {
							popup->GetComboBox()->setCurrentIndex(0);
						} else {
							popup->GetComboBox()->setCurrentText(line->text());
						}

						if (popup->exec() == QDialog::Accepted)
						{
							QJsonObject input;
							input["value"] = popup->GetValue();
							SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
						}
						popup->deleteLater();
				  });

	Q_EMIT BroadcastWidth(size().width());
}

QList<AgxLineEdit*> AgxNodePropertiesWidget::CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxNode* signalSender, bool split)
{
	QList<AgxLineEdit*> outputList;
	for (int i = 0; i < dataRef->size(); i++)
	{
		auto& dataRefItem = (*dataRef)[i];
		QString key = dataRefItem.Tag();
		
		//if (dataRefItem.columnType == AgxColumnTypes::CommentBox) continue;

		auto output = AgxWidgetUtil::CreateEntry(dataRefItem.columnType, {key}, this);


		output->setText(dataRefItem.value);

		if (dataRefItem.columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(output, dataRefItem.customDropDownList, {key});
		}

		QHBoxLayout* containerLayout = new QHBoxLayout();
		QCheckBox* checkBox = new QCheckBox();
		checkBox->setMinimumHeight(output->minimumHeight());

		containerLayout->addWidget(output);
		containerLayout->addWidget(checkBox);
		containerLayout->setContentsMargins(0, 0, 0, 0);
		QWidget* container = new QWidget();
		container->setContentsMargins(0, 0, 0, 0);
		container->setLayout(containerLayout);

		//bool flip = _RightFormLayout->rowCount() < _LeftFormLayout->rowCount();

		if (split && !_nextEntryLeft)
		{
			_RightFormLayout->addRow(dataRefItem.Label(), container);
			_RightFormLayout->setRowVisible(container, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, container](bool enabled) {
						_RightFormLayout->setRowVisible(container, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_RightFormLayout->labelForField(container)))
			{
				//checkBox->setChecked(dataRefItem.isPresent);
				label->setEnabled(dataRefItem.isPresent);
				output->setEnabled(dataRefItem.isPresent);
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output, checkBox](bool enabled) {
						label->setEnabled(enabled);
						output->setEnabled(enabled);
						if (checkBox->isChecked() != enabled) {
							checkBox->blockSignals(true);
							checkBox->setChecked(enabled);
							checkBox->blockSignals(false);
						}
						});
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
							label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = true;
		} else {
			_LeftFormLayout->addRow(dataRefItem.Label(), container);
			_LeftFormLayout->setRowVisible(container, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, container](bool enabled) {
						_LeftFormLayout->setRowVisible(container, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_LeftFormLayout->labelForField(container)))
			{
				label->setEnabled(dataRefItem.isPresent);
				output->setEnabled(dataRefItem.isPresent);
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output, checkBox](bool enabled) {
						label->setEnabled(enabled);
						output->setEnabled(enabled);
						if (checkBox->isChecked() != enabled) {
							checkBox->blockSignals(true);
							checkBox->setChecked(enabled);
							checkBox->blockSignals(false);
						}
						});
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
					label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = false;
		}
		
		checkBox->setChecked(dataRefItem.isPresent);
		
		connect(checkBox, &QCheckBox::checkStateChanged, &dataRefItem, [this, &dataRefItem, checkBox, key] {
					QJsonObject input;
					input["isPresent"] = checkBox->isChecked();
					SendInsertPropertySheetDataCommand(QStringListToQJsonObject({ key }, input));
				});

		connect(signalSender, &AgxNode::PropertySheetUpdated, output, [output, &dataRefItem]() {
			output->blockSignals(true);
			output->setText(dataRefItem.value);
			output->RefreshTooltip(dataRefItem.value);
			output->blockSignals(false);
				});
		

		outputList.append(output);
	}

	Q_EMIT BroadcastWidth(size().width());
	return outputList;
}

QList<AgxLineEdit*> AgxNodePropertiesWidget::CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxGraphModel* signalSender, bool split)
{
	QList<AgxLineEdit*> outputList;
	for (int i = 0; i < dataRef->size(); i++)
	{
		auto& dataRefItem = (*dataRef)[i];

		QString key = dataRefItem.Tag();
		auto output = AgxWidgetUtil::CreateEntry(dataRefItem.columnType, { key }, this);


		output->setText(dataRefItem.value);

		if (dataRefItem.columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(output, dataRefItem.customDropDownList, { key });
		}

		QHBoxLayout* containerLayout = new QHBoxLayout();
		QCheckBox* checkBox = new QCheckBox();
		checkBox->setMinimumHeight(output->minimumHeight());

		containerLayout->addWidget(output);
		containerLayout->addWidget(checkBox);
		containerLayout->setContentsMargins(0, 0, 0, 0);
		QWidget* container = new QWidget();
		container->setContentsMargins(0, 0, 0, 0);
		container->setLayout(containerLayout);

		//bool flip = _RightFormLayout->rowCount() < _LeftFormLayout->rowCount();

		if (split && !_nextEntryLeft)
		{
			_RightFormLayout->addRow(dataRefItem.Label(), container);
			_RightFormLayout->setRowVisible(container, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, container](bool enabled) {
				_RightFormLayout->setRowVisible(container, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_RightFormLayout->labelForField(container)))
			{
				//checkBox->setChecked(dataRefItem.isPresent);
				label->setEnabled(dataRefItem.isPresent);
				output->setEnabled(dataRefItem.isPresent);
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output, checkBox](bool enabled) {
					label->setEnabled(enabled);
					output->setEnabled(enabled);
					if (checkBox->isChecked() != enabled) {
						checkBox->blockSignals(true);
						checkBox->setChecked(enabled);
						checkBox->blockSignals(false);
					}
						});
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
					label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = true;
		} else {
			_LeftFormLayout->addRow(dataRefItem.Label(), container);
			_LeftFormLayout->setRowVisible(container, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _LeftFormLayout, [this, container](bool enabled) {
				_LeftFormLayout->setRowVisible(container, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_LeftFormLayout->labelForField(container)))
			{
				label->setEnabled(dataRefItem.isPresent);
				output->setEnabled(dataRefItem.isPresent);
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output, checkBox](bool enabled) {
					label->setEnabled(enabled);
					output->setEnabled(enabled);
					if (checkBox->isChecked() != enabled) {
						checkBox->blockSignals(true);
						checkBox->setChecked(enabled);
						checkBox->blockSignals(false);
					}
						});
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
					label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = false;
		}

		checkBox->setChecked(dataRefItem.isPresent);

		connect(checkBox, &QCheckBox::checkStateChanged, &dataRefItem, [this, &dataRefItem, checkBox, key] {
			QJsonObject input;
			input["isPresent"] = checkBox->isChecked();
			SendInsertPropertySheetDataCommand(QStringListToQJsonObject({ key }, input));
				});

		connect(signalSender, &AgxGraphModel::PropertySheetUpdated, output, [output, dataRef, i]() {
			if (dataRef->size() <= i) return;

			output->blockSignals(true);
			output->setText(dataRef->at(i).value);
			output->RefreshTooltip(dataRef->at(i).value);
			output->blockSignals(false);
				});
		outputList.append(output);
	}

	Q_EMIT BroadcastWidth(size().width());
	return outputList;
}

QList<AgxLineEdit*> AgxNodePropertiesWidget::CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxPort* signalSender, bool split, QStringList path)
{
	path.append("temp");

	QList<AgxLineEdit*> outputList;
	for (int i = 0; i < dataRef->size(); i++)
	{
		auto& dataRefItem = (*dataRef)[i];

		QString key = dataRefItem.Tag();
		
		path.back() = key;

		auto output = AgxWidgetUtil::CreateEntry(dataRefItem.columnType, path, this);

		output->setText(dataRefItem.value);

		if (dataRefItem.columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(output, dataRefItem.customDropDownList, path);
		}

		QHBoxLayout* containerLayout = new QHBoxLayout();
		QCheckBox* checkBox = new QCheckBox();
		checkBox->setMinimumHeight(output->minimumHeight());

		containerLayout->addWidget(output);
		containerLayout->addWidget(checkBox);
		containerLayout->setContentsMargins(0, 0, 0, 0);
		QWidget* container = new QWidget();
		container->setContentsMargins(0, 0, 0, 0);
		container->setLayout(containerLayout);

		//bool flip = _RightFormLayout->rowCount() < _LeftFormLayout->rowCount();

		if (split && !_nextEntryLeft)
		{
			_RightFormLayout->addRow(dataRefItem.Label(), container);
			_RightFormLayout->setRowVisible(container, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _RightFormLayout, [this, container](bool enabled) {
				_RightFormLayout->setRowVisible(container, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_RightFormLayout->labelForField(container)))
			{
				//checkBox->setChecked(dataRefItem.isPresent);
				label->setEnabled(dataRefItem.isPresent);
				output->setEnabled(dataRefItem.isPresent);
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output, checkBox](bool enabled) {
					label->setEnabled(enabled);
					output->setEnabled(enabled);
					if (checkBox->isChecked() != enabled) {
						checkBox->blockSignals(true);
						checkBox->setChecked(enabled);
						checkBox->blockSignals(false);
					}
						});
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
					label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = true;
		} else {
			_LeftFormLayout->addRow(dataRefItem.Label(), container);
			_LeftFormLayout->setRowVisible(container, dataRefItem.propertyEnabled);
			connect(&dataRefItem, &AgxPropertyEntryDefinition::StateUpdated, _LeftFormLayout, [this, container](bool enabled) {
				_LeftFormLayout->setRowVisible(container, enabled);
					});
			if (QLabel* label = dynamic_cast<QLabel*>(_LeftFormLayout->labelForField(container)))
			{
				label->setEnabled(dataRefItem.isPresent);
				output->setEnabled(dataRefItem.isPresent);
				connect(&dataRefItem, &AgxPropertyEntryDefinition::PresentUpdated, label, [this, label, output, checkBox](bool enabled) {
					label->setEnabled(enabled);
					output->setEnabled(enabled);
					if (checkBox->isChecked() != enabled) {
						checkBox->blockSignals(true);
						checkBox->setChecked(enabled);
						checkBox->blockSignals(false);
					}
						});
				connect(this, &AgxNodePropertiesWidget::LanguageChanged, label, [label, &dataRefItem]() {
					label->setText(dataRefItem.Label());
						});
			}
			_nextEntryLeft = false;
		}

		checkBox->setChecked(dataRefItem.isPresent);

		connect(checkBox, &QCheckBox::checkStateChanged, &dataRefItem, [this, &dataRefItem, checkBox, path] {
			QJsonObject input;
			input["isPresent"] = checkBox->isChecked();
			SendInsertPropertySheetDataCommand(QStringListToQJsonObject(path, input));
				});

		connect(signalSender, &AgxPort::PropertySheetUpdated, output, [output, &dataRefItem]() {
			output->blockSignals(true);
			output->setText(dataRefItem.value);
			output->RefreshTooltip(dataRefItem.value);
			output->blockSignals(false);
				});
		outputList.append(output);
	}

	Q_EMIT BroadcastWidth(size().width());
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

	Q_EMIT BroadcastWidth(size().width());
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

	Q_EMIT BroadcastWidth(size().width());
	return outputList;
}

QLabel* AgxNodePropertiesWidget::CreateGuidLabel(const QUuid* value, AgxNode* signalSender, bool split)
{
	QLabel* output = new QLabel(value->toString());

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

	Q_EMIT BroadcastWidth(size().width());
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

	Q_EMIT BroadcastWidth(size().width());
	return button;
}

AgxLineEdit* AgxNodePropertiesWidget::CreateSimpleLineEdit(QString* sourceData, AgxPort* signalSender, const QString& label, bool split, QStringList path)
{
	AgxLineEdit* output = AgxWidgetUtil::CreateEntry(AgxColumnTypes::BasicString, path, this);
	output->blockSignals(true);
	output->setText(*sourceData);
	output->blockSignals(false);
	//bool flip = _hiddenRightFormLayout->rowCount() < _hiddenLeftFormLayout->rowCount();
	if (split && !_nextEntryLeft)
	{
		_hiddenRightFormLayout->addRow(label, output);
		_nextEntryLeft = true;
	} else {
		_hiddenLeftFormLayout->addRow(label, output);
		_nextEntryLeft = false;
	}
	connect(signalSender, &AgxPort::PropertySheetUpdated, output, [output, sourceData]() {
		output->blockSignals(true);
		output->setText(*sourceData);
		output->blockSignals(false);
			});

	Q_EMIT BroadcastWidth(size().width());
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

	Q_EMIT BroadcastWidth(size().width());
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
	
	Q_EMIT BroadcastWidth(size().width());
	return;
}

void AgxNodePropertiesWidget::FinalizeWidget()
{
	blockSignals(false);
	Q_EMIT BroadcastWidth(size().width());
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
