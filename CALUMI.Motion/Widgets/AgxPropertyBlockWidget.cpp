//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxPropertyBlockWidget.h"
#include "AgxNodes/AgxNode.h"
#include "Utilities/PropertySheetEntryUtil.h"
#include "Models/AgxGraphicsScene.h"
#include "Widgets/IAgxEmbedSceneData.h"
#include "Utilities/AgxDefinitions.h"
#include "Dialog/AgxSimpleDialog.h"

static int GetColumnWidth(const AgxColumnTypes& type) {
	
	switch (type)
	{
		case AgxColumnTypes::BasicString:
			return 150;
		case AgxColumnTypes::BasicInteger: 
		case AgxColumnTypes::BasicFloat: 
			return 50;
		case AgxColumnTypes::BasicVector: 
		case AgxColumnTypes::BasicBool: 
			return 80;
		case AgxColumnTypes::BasicMultiVar:
		case AgxColumnTypes::CustomDropDown: 
			return 50;
		case AgxColumnTypes::CustomInteger: 
		case AgxColumnTypes::CustomFloat: 
		case AgxColumnTypes::CustomVector: 
		case AgxColumnTypes::CustomBool: 
		case AgxColumnTypes::CustomMultiVar: 
		case AgxColumnTypes::Event: 
			return 150;
		case AgxColumnTypes::Prefix:
		case AgxColumnTypes::Suffix:
			return 100;
		case AgxColumnTypes::State: 
		case AgxColumnTypes::Action: 
		case AgxColumnTypes::SyncSystem: 
			return 150;
		default: 
			return 100;
	}
}

//static void filter_1(QString& text) {
//	QRegularExpression regex("\\_.*\\_");
//	text.replace(regex, "");
//}
//
//static void filter_2(QString& text) {
//	QRegularExpression regex("\\~.*\\~");
//	text.replace(regex, "");
//}

AgxPropertyBlockWidget::AgxPropertyBlockWidget(TermRef ref, AgxPropertyBlockData& dataRef, QWidget* parent) : QWidget(parent), _grid(new QGridLayout()), _label(new QLabel(ref().translation)), _dataRef(&dataRef)
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	QVBoxLayout* mainLayout = new QVBoxLayout();

	mainLayout->addWidget(_label,1, Qt::AlignCenter);
	mainLayout->addLayout(_grid);
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);
	mainLayout->setHorizontalSizeConstraint(QLayout::SetMinimumSize);
	_grid->setVerticalSizeConstraint(QLayout::SetFixedSize);
	
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);

	ModifiedPushButton* addButton = new ModifiedPushButton("Add Entry");

	addButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	
	QSpacerItem* bSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

	buttonLayout->addItem(bSpacer);
	buttonLayout->addWidget(addButton);

	mainLayout->addLayout(buttonLayout);
	//QSpacerItem* vSpacer = new QSpacerItem(0, 5, QSizePolicy::Fixed, QSizePolicy::Fixed);
	//mainLayout->addItem(vSpacer);

	connect(_dataRef, &AgxPropertyBlockData::RowAdded, this, &AgxPropertyBlockWidget::OnRowAdded);
	connect(_dataRef, &AgxPropertyBlockData::RowRemoved, this, &AgxPropertyBlockWidget::OnRowRemoved);

	connect(addButton, &ModifiedPushButton::CustomPressSignal, _dataRef, [this, ref]() {
		//
		if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
		{
			iagx->SendAddRowToPropertyBlockDataCommand(ref().tag, _dataRef->GetRowCount());
				}
			});

	connect(this, &AgxPropertyBlockWidget::LanguageChanged, _label, [this, ref]() { _label->setText(ref().translation); });

	setLayout(mainLayout);

	//int width = _dataRef->GetColumnCount() > 3 ? 100 : 200;
	for (unsigned int i = 0; i < _dataRef->GetColumnCount(); i++)
	{
		QString labelStr = i >= dataRef.GetColumnCount() ? "-" : dataRef.GetColumnDefinition(i).Label();
		
		int width = GetColumnWidth(dataRef.GetColumnDefinition(i).columnType);

		agxStringFilter_1(labelStr);

		bool empty = labelStr.isEmpty();

		agxStringFilter_2(labelStr);

		QLabel* label = new QLabel(labelStr);

		connect(this, &AgxPropertyBlockWidget::LanguageChanged, label, [dataRef, i, label]() {
			label->setText(dataRef.GetColumnDefinition(i).Label());
				});

		if (labelStr.isEmpty())
		{
			label->setMinimumWidth(50);
			label->setFixedWidth(50);
		}
		else
			label->setMinimumWidth(width);
		
		//label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
		QSpacerItem* hSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
		
		_grid->addItem(hSpacer, 0, i * 2);
		_grid->addWidget(label, 0, i * 2 + 1, 1, 1, Qt::AlignLeft);
		label->setAlignment(Qt::AlignLeft);
	}

	for (unsigned int i = 0; i < _dataRef->GetRowCount(); i++)
	{
		ConstructRow(i);
	}

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &AgxPropertyBlockWidget::customContextMenuRequested, this, &AgxPropertyBlockWidget::ShowContextMenu);
}

void AgxPropertyBlockWidget::ForceRefresh()
{
	updateGeometry();
	parentWidget()->updateGeometry();
	if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
	{
		iagx->RefreshNode();
	}
}

void AgxPropertyBlockWidget::SetUpCustomDropDown(AgxLineEditContainer* line, const QList<TermRef>& list, const QStringList& keyPath)
{
	connect(line, &AgxLineEditContainer::ContentDoubleClicked, line, [this, line, list, keyPath]() {

		QStringList stringList;
		int current = 0;

		for (auto term : list) {
			if (term && term().tag.compare(line->text(), Qt::CaseInsensitive) == 0)
				current = list.indexOf(term);
		}

		if (auto result = AgxSimpleDialog::GetDropDown(this, "Select Entry", "", list, current, false)) {
			if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
			{
				QJsonObject input;
				input["value"] = result().tag;
				iagx->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(keyPath, input));
			}
		}

			});
}

void AgxPropertyBlockWidget::ConstructRow(int index)
{
	auto rowRef = _dataRef->GetRow(index);
	//int width = _dataRef->GetColumnCount() > 3 ? 100 : 200;
	for (unsigned int j = 0; j < _dataRef->GetColumnCount(); j++)
	{
		QStringList keyPath = { "property-blocks",_label->text(),std::to_string(index).c_str(),std::to_string(j).c_str() };

		auto line = AgxWidgetUtil::CreateEntry(_dataRef->GetColumnType(j), keyPath, parent());
		line->setCheckbox(false);

		int width = GetColumnWidth(_dataRef->GetColumnDefinition(j).columnType);

		line->setContentText(rowRef->at(j).Value);
		line->RefreshContentTooltip("[" + GetSFBGSVarTypeFromColumnType(rowRef->at(j).Type).first + "]");

		if (_dataRef->GetColumnDefinition(j).columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(line, _dataRef->GetColumnDefinition(j).CustomDropDownList(), keyPath);
		}


		line->SetContentAlignment(Qt::AlignLeft);
		
		QString labelStr = j >= _dataRef->GetColumnCount() ? "-" : _dataRef->GetColumnDefinition(j).Tag();

		QRegularExpression regex("\\_.*\\_");
		labelStr.replace(regex, "");

		if (labelStr.isEmpty())
		{
			line->setContentMinWidth(50);
			line->setContentFixedWidth(50);
		}
		else
			line->setContentMinWidth(width);

		//line->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
		_grid->addWidget(line, index + 1, j * 2 + 1, Qt::AlignLeft);
		connect(_dataRef, &AgxPropertyBlockData::DataUpdated, line, [this, line, index, j]() {

			line->setContentText(_dataRef->GetRow(index)->at(j).Value);
			line->RefreshContentTooltip("[" + GetSFBGSVarTypeFromColumnType(_dataRef->GetRow(index)->at(j).Type).first + "]");

				});
	}
}

void AgxPropertyBlockWidget::ClearRows(int index)
{
	index++;
	for (int i = index; i < _grid->rowCount(); i++)
	{
		for (int j = 0; j < _grid->columnCount(); j++)
		{
			if (auto item = _grid->itemAtPosition(i, j))
			{
				if (auto widget = item->widget())
				{
					_grid->removeWidget(widget);
					widget->deleteLater();
				}
			}
		}
	}
}

void AgxPropertyBlockWidget::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);

	//QTimer::singleShot(0, this, &AgxPropertyBlockWidget::ForceRefresh);
}

void AgxPropertyBlockWidget::OnRowAdded(int index)
{
	ClearRows(index);

	for (unsigned int i = index; i < _dataRef->GetRowCount(); i++)
	{
		ConstructRow(i);
	}

	Q_EMIT RowsChanged();
}

void AgxPropertyBlockWidget::OnRowRemoved(int index)
{
	ClearRows(index);

	for (unsigned int i = index; i < _dataRef->GetRowCount(); i++)
	{
		ConstructRow(i);
	}

	Q_EMIT RowsChanged();
}

void AgxPropertyBlockWidget::ShowContextMenu(const QPoint& pos)
{
	int index = -1;
	QPoint nPos = pos;
	for (int y = 1; y < _grid->rowCount(); y++)
	{
		auto rect = _grid->cellRect(y, 1);
		nPos.setX(rect.x());
		if (rect.contains(nPos))
		{
			index = y - 1;
		}
	}
	if(index >= 0){
		QMenu* cMenu = new QMenu();
		QAction* removeAction = new QAction("Remove Entry");
		cMenu->addAction(removeAction);
		
		QString title = _label->text();
		connect(removeAction, &QAction::triggered, this, [this, title, index]() {
			if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
			{
				iagx->SendRemoveRowFromPropertyBlockDataCommand(title, index);
			}
				});
		if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
		{
			QPoint scaledPos = iagx->mapToAgxView(mapToParent(pos));

			//if (scaledPos == mapToParent(pos)) 
			{
				scaledPos = mapToGlobal(pos);
			}

			cMenu->exec(scaledPos);
		}
	}


}

void ModifiedPushButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0 || block)
	{
		event->ignore();
		return;
	}
	
	if (!block)
	{
		//QPushButton::mousePressEvent(event);
		QTimer::singleShot(0, this, &ModifiedPushButton::animateClick);
		//animateClick();
		Q_EMIT CustomPressSignal();
		block = true;
		QTimer::singleShot(500, this, &ModifiedPushButton::ResetBlock);
	}
}

void ModifiedPushButton::ResetBlock()
{
	block = false;
}
