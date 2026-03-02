#include "stdafx.h"
#include "NodeGroupMenuPopup.h"
#include "StringEntryPopup.h"
#include "Utilities/UndoRedoCommands.h"
#include <windows.h>

void NodeGroupMenuPopup::BuildTreeView()
{
	if (!&_scene) return;
	ui.NodeGroupTree->clear();

	if (!_showNodeTree) {
		QTreeWidgetItem* noneItem = new QTreeWidgetItem(ui.NodeGroupTree);
		noneItem->setText(0, tr("NONE"));
	}

	for (auto& entry : _scene.agxGraphModel().GetNodeGroupList())
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(ui.NodeGroupTree);
		item->setText(0, entry);
		item->setBackground(1, QBrush(_scene.agxGraphModel().GetGroupColor(entry)));
	}
	
	ui.NodeGroupTree->header()->setStretchLastSection(false);
	ui.NodeGroupTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	ui.NodeGroupTree->header()->resizeSection(1, 10);

	ui.NodeTree->clear();
	
	if (!_showNodeTree) 
	{
		ui.NodeTree->hide();
		return;
	}
	for (auto& entry : _scene.agxGraphModel().GetNodeGroupAssignmentList())
	{
		QTreeWidgetItem* parent = new QTreeWidgetItem(ui.NodeTree);
		parent->setText(0, entry.first);
		for (auto& subentry : entry.second)
		{
			QTreeWidgetItem* child = new QTreeWidgetItem(parent);
			child->setText(0, QString(std::to_string(subentry).c_str()));
			QString nodeName = _scene.agxGraphModel().nodeData(subentry, AgxNodeRole::Caption).toString();
			child->setText(1, nodeName);
		}
	}
}

NodeGroupMenuPopup::NodeGroupMenuPopup(QWidget* parent, AgxGraphicsScene& scene, bool showNodeTree) : NodeGroupMenuPopup(parent, scene, nullptr, nullptr, showNodeTree) {}
NodeGroupMenuPopup::NodeGroupMenuPopup(QWidget* parent, AgxGraphicsScene& scene, AgxGraphicsView* view, QTabWidget* tab, bool showNodeTree)
	: QWidget(parent), _scene(scene), _showNodeTree(showNodeTree), _view(view), _tab(tab)
{
	ui.setupUi(this);
	BuildTreeView();
	connect(ui.NodeGroupTree, &QTreeWidget::customContextMenuRequested, this, &NodeGroupMenuPopup::prepareNodeGroupTreeMenu);
	connect(ui.NodeTree, &CustomQTreeWidget::mousePressEventCalled, ui.NodeGroupTree, &CustomQTreeWidget::onDeselectCalled);
}

NodeGroupMenuPopup::~NodeGroupMenuPopup(){}

QString NodeGroupMenuPopup::GetSelectedGroup()
{
	auto item = ui.NodeGroupTree->selectedItems();
	if(item.size()>0 && item.at(0)->text(0) != tr("NONE"))
		return item.at(0)->text(0);

	return "";
}

void NodeGroupMenuPopup::SetActiveNodeGroupTreeItem(const QString& group)
{
	ui.NodeGroupTree->onDeselectCalled();
	QTreeWidgetItem* item;
	for (int i = 0; i < ui.NodeGroupTree->topLevelItemCount(); i++)
	{
		item = ui.NodeGroupTree->topLevelItem(i);
		if (group == item->text(0))
			ui.NodeGroupTree->setCurrentItem(item);
	}
}

QColor NodeGroupMenuPopup::GetNewColorDialog(QColor initial)
{
	return QColorDialog::getColor(initial, this, tr("Select Color"));
}

void NodeGroupMenuPopup::mousePressEvent(QMouseEvent* event)
{
	ui.NodeGroupTree->onDeselectCalled();
}

void NodeGroupMenuPopup::prepareNodeGroupTreeMenu(const QPoint &pos)
{
	QMenu cMenu(this);
	QAction* addGroup = cMenu.addAction(tr("Add Group"));
	

	connect(addGroup, &QAction::triggered, this, [this]() {
		StringEntryPopup popup(this);
		int result = popup.exec();
		if (result == QDialog::Accepted)
		{
			//this->_scene.agxGraphModel().CreateNodeGroup(popup.getInputText());
			this->_scene.undoStack().push(new CreateGroupCommand(&(this->_scene), popup.getInputText()));
			this->BuildTreeView();
			SetActiveNodeGroupTreeItem(popup.getInputText());
		}

		});

	QTreeWidgetItem* clickedItem = ui.NodeGroupTree->itemAt(pos);
	if(clickedItem)
	{
		QAction* removeGroup = cMenu.addAction(tr("Remove Group"));
		connect(removeGroup, &QAction::triggered, this, [this, clickedItem]()
			{
				QString str = clickedItem->text(0);
				_scene.undoStack().push(new EraseGroupCommand(&_scene, str));
				this->BuildTreeView();
			});

		if(_view && _tab){
			QAction* selectGroup = cMenu.addAction(tr("Select Group"));
			connect(selectGroup, &QAction::triggered, this, [this, clickedItem]() {
					this->_view->SelectNodeGroup(clickedItem->text(0));
					this->_tab->setCurrentWidget(_view);
					auto gWindow = dynamic_cast<QDialog*>(this->parent()->parent()->parent());
					gWindow->reject();
				});
		}
		QAction* chooseColor = cMenu.addAction(tr("Change Color"));
		connect(chooseColor, &QAction::triggered, this, [this, clickedItem]() {
					QColor iniColor = this->_scene.agxGraphModel().GetGroupColor(clickedItem->text(0));
					QColor newColor = this->GetNewColorDialog(iniColor);
					_scene.undoStack().push(new ChangeGroupColorCommand(&_scene, clickedItem->text(0), newColor));
					BuildTreeView();

					

			});
	}

	cMenu.exec(ui.NodeGroupTree->cursor().pos());
}

void NodeGroupMenuPopup::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui.retranslateUi(this);
	}

	QWidget::changeEvent(event);
}

