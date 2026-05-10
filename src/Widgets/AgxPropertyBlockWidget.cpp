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
#include "AgxWrappedRowEntry/AgxWrappedRowEntry.h"

AgxPropertyBlockWidget::AgxPropertyBlockWidget(TermRef ref, AgxPropertyBlockData& dataRef, uint8_t wrappedRowItemCount, QWidget* parent) : QWidget(parent), _vLayout(new QVBoxLayout()), _dataRef(&dataRef), _labelRef(ref), _wrappedRowItemCount(wrappedRowItemCount)
{
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	QVBoxLayout* mainLayout = new QVBoxLayout();

	QLabel* blockLabel = new QLabel(_labelRef().translation);
	QFont boldFont = blockLabel->font();
	boldFont.setBold(true);
	blockLabel->setFont(boldFont);

	QFrame* hLine = new QFrame();
	hLine->setFrameShape(QFrame::HLine);
	hLine->setFrameShadow(QFrame::Sunken);
	hLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	mainLayout->addWidget(blockLabel,1, Qt::AlignCenter);
	mainLayout->addWidget(hLine, 1, Qt::AlignCenter);

	mainLayout->addLayout(_vLayout);
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);
	mainLayout->setHorizontalSizeConstraint(QLayout::SetMinimumSize);
	
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->setVerticalSizeConstraint(QLayout::SetFixedSize);

	ModifiedPushButton* addButton = new ModifiedPushButton(tr("Add Entry"));

	addButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	buttonLayout->addWidget(addButton);

	mainLayout->addLayout(buttonLayout);

	connect(_dataRef, &AgxPropertyBlockData::RowAdded, this, &AgxPropertyBlockWidget::OnRowAdded);
	connect(_dataRef, &AgxPropertyBlockData::RowRemoved, this, &AgxPropertyBlockWidget::OnRowRemoved);

	connect(addButton, &ModifiedPushButton::CustomPressSignal, _dataRef, [this, ref]() 
	{
		if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
		{
			iagx->SendAddRowToPropertyBlockDataCommand(ref().tag, _dataRef->GetRowCount());
		}
	});

	connect(this, &AgxPropertyBlockWidget::LanguageChanged, blockLabel, [this, ref, blockLabel, addButton]()
	{ 
		blockLabel->setText(ref().translation); 
		addButton->setText(tr("Add Entry"));
	});

	setLayout(mainLayout);

	QList<QWidget*> labels = {};
	for (unsigned int i = 0; i < _dataRef->GetColumnCount(); i++)
	{
		QString labelStr = i >= dataRef.GetColumnCount() ? "-" : dataRef.GetColumnDefinition(i).Label();
		
		agxStringFilter_1(labelStr);
		agxStringFilter_2(labelStr);

		QLabel* label = new QLabel(labelStr);

		connect(this, &AgxPropertyBlockWidget::LanguageChanged, label, [dataRef, i, label]()
		{
			label->setText(dataRef.GetColumnDefinition(i).Label());
		});

		label->setFixedWidth(200);

		QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(label);
		effect->setOpacity(1.0);
		label->setGraphicsEffect(effect);

		QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
		animation->setDuration(1500);
		animation->setStartValue(1.0);
		animation->setEasingCurve(QEasingCurve::InOutCubic);
		animation->setKeyValueAt(0.5, 0.0);
		animation->setEndValue(1.0);
		animation->setLoopCount(-1);

		label->setAlignment(Qt::AlignLeft);

		labels.append(label);
		_columnHeaders.append(label);
		_columnAnimation.append(animation);
	}

	AgxWrappedRowEntry* labelRow = new AgxWrappedRowEntry(labels,this, _wrappedRowItemCount);
	_vLayout->addWidget(labelRow, Qt::AlignLeft);

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
	connect(line, &AgxLineEditContainer::ContentDoubleClicked, line, [this, line, list, keyPath]() 
	{

		QStringList stringList;
		int current = 0;

		for (auto term : list) {
			if (term && term().tag.compare(line->text(), Qt::CaseInsensitive) == 0)
				current = list.indexOf(term);
		}

		if (auto result = AgxSimpleDialog::GetDropDown(this, tr("Select Entry"), "", list, current, false)) {
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
	
	QList<QWidget*> rowEntry = {};

	for (unsigned int j = 0; j < _dataRef->GetColumnCount(); j++)
	{
		QStringList keyPath = { "property-blocks",_labelRef().tag,std::to_string(index).c_str(),std::to_string(j).c_str() };

		auto line = AgxWidgetUtil::CreateEntry(_dataRef->GetColumnType(j), keyPath, parent());
		line->setCheckbox(false);

		line->setContentText(rowRef->at(j).Value);
		line->RefreshContentTooltip("[" + GetSFBGSVarTypeFromColumnType(rowRef->at(j).Type).first + "]");

		if (_dataRef->GetColumnDefinition(j).columnType == AgxColumnTypes::CustomDropDown)
		{
			SetUpCustomDropDown(line, _dataRef->GetColumnDefinition(j).CustomDropDownList(), keyPath);
		}

		line->SetContentAlignment(Qt::AlignLeft);

		rowEntry.append(line);
		
		connect(_dataRef, &AgxPropertyBlockData::DataUpdated, line, [this, line, index, j]()
		{
			line->setContentText(_dataRef->GetRow(index)->at(j).Value);
			line->RefreshContentTooltip("[" + GetSFBGSVarTypeFromColumnType(_dataRef->GetRow(index)->at(j).Type).first + "]");
		});
		
		
		if (auto colLabel = _columnHeaders.at(j))
		{
			if (auto animation = _columnAnimation.at(j))
			{
				connect(line, &AgxLineEditContainer::hovered, colLabel, [this, colLabel, animation]()
				{
					colLabel->graphicsEffect()->setEnabled(true);
					animation->start();
				});

				connect(line, &AgxLineEditContainer::unhovered, colLabel, [this, colLabel, animation]()
				{
					animation->stop();
					colLabel->graphicsEffect()->setEnabled(false);

				});
			}
		}

	}

	if (!rowEntry.isEmpty())
	{
		bool toSep = rowEntry.count() > _wrappedRowItemCount;
		AgxWrappedRowEntry* wrappedRow = new AgxWrappedRowEntry(rowEntry, this, _wrappedRowItemCount, toSep);
		_vLayout->addWidget(wrappedRow, Qt::AlignLeft);
	}
}

void AgxPropertyBlockWidget::ClearRows(int index)
{
	index++;

	for (int i = _vLayout->count() - 1; i >= index; i--)
	{
		if (auto item = _vLayout->itemAt(i))
		{
			if (auto widget = item->widget())
			{
				_vLayout->removeWidget(widget);
				widget->deleteLater();
			}
		}
	}
}

void AgxPropertyBlockWidget::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
}

void AgxPropertyBlockWidget::OnRowAdded(int index)
{
	ClearRows(index);

	for (unsigned int i = index; i < _dataRef->GetRowCount(); i++)
	{
		ConstructRow(i);
	}

	QTimer::singleShot(1, this, [this]() 
	{
		adjustSize();
		Q_EMIT RowsChanged();
	});
}

void AgxPropertyBlockWidget::OnRowRemoved(int index)
{
	ClearRows(index);

	for (unsigned int i = index; i < _dataRef->GetRowCount(); i++)
	{
		ConstructRow(i);
	}

	QTimer::singleShot(1, this, [this]()
	{
	   adjustSize();
	   Q_EMIT RowsChanged();
	});
}

void AgxPropertyBlockWidget::ShowContextMenu(const QPoint& pos)
{
	int index = -1;
	QPoint nPos = pos;
	for (int y = 1; y < _vLayout->count(); y++)
	{
		if (_vLayout->itemAt(y) && _vLayout->itemAt(y)->geometry().contains(nPos))
		{
			index = y - 1;
		}
	}
	if(index >= 0){
		QMenu* cMenu = new QMenu();
		QAction* removeAction = new QAction(tr("Remove Entry"));
		cMenu->addAction(removeAction);
		
		connect(removeAction, &QAction::triggered, this, [this, index]() 
		{
			if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
			{
				iagx->SendRemoveRowFromPropertyBlockDataCommand(_labelRef().tag, index);
			}
		});

		if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(this->parent()))
		{
			QPoint scaledPos = iagx->mapToAgxView(mapToParent(pos));

			scaledPos = mapToGlobal(pos);

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
		QTimer::singleShot(0, this, &ModifiedPushButton::animateClick);

		Q_EMIT CustomPressSignal();
		block = true;
		QTimer::singleShot(500, this, &ModifiedPushButton::ResetBlock);
	}
}

void ModifiedPushButton::ResetBlock()
{
	block = false;
}
