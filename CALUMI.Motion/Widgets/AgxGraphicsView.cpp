//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#pragma warning(push,0)

#include "Painter/AgxStyleCollection.h"
#include <QGraphicsScene>
#include <QMenu>

#include <QDebug>
#include <QPointF>
#include <QRectF>
#include <QBrush>
#include <QPen>
#include <QtWidgets>
#pragma warning(pop)
#include "AgxGraphicsView.h"
#include <format>
#include "Widgets/StringEntryPopup.h"
#include "AgxNodes/AgxNode.h"
#include "Widgets/NodeGroupMenuPopup.h"
#include "Models/AgxNodeGraphicsObject.h"
#include "Utilities/UndoRedoCommands.h"
#include <algorithm>


#include <Utilities/AgxGraphRegistry.h>

void AgxGraphicsView::ToggleNodeCollapse(const AgxNodeId nodeId)
{
	if(auto agxModel = &(agxNodeScene()->agxGraphModel()))
		agxModel->ToggleNodeCollapse(nodeId);

}

void AgxGraphicsView::SelectNodeGroup(const AgxNodeId nodeId, bool additive)
{
	
	auto agxModel = &agxNodeScene()->agxGraphModel();
	if (agxModel)
	{
		auto nodeGroup = agxModel->GetNodeGroup(nodeId);
		if (nodeGroup == "") 
		{
			bool setting = true;
			if (additive)
				setting = !agxNodeScene()->agxNodeGraphicsObject(nodeId)->isSelected();
			agxNodeScene()->agxNodeGraphicsObject(nodeId)->setSelected(setting);
			return;
		}
		SelectNodeGroup(nodeGroup, agxModel, additive);
	}
	
}

void AgxGraphicsView::SelectNodeGroup(const QString nodeGroup, AgxGraphModel* agxModel, bool additive)
{
	if (nodeGroup == "") return;

	if (!agxModel)
		agxModel = &agxNodeScene()->agxGraphModel();
	bool setting = true;
	if(additive)
	{
		setting = false;
		for (auto id : agxModel->allNodeIds())
		{
			//is it good to loop twice through to determine if the group is selected? Probably not, but it beats trying to add tons of select/deselect logic for groups elsewhere
			if (agxModel->GetNodeGroup(id) == nodeGroup && !agxNodeScene()->agxNodeGraphicsObject(id)->isSelected())
				setting = true;
		}
	}
	for (auto id : agxModel->allNodeIds())
	{
		if (agxModel->GetNodeGroup(id) == nodeGroup)
			agxNodeScene()->agxNodeGraphicsObject(id)->setSelected(setting);
	}
}

void AgxGraphicsView::ShowContextMenu(AgxNodeId const nodeId, QPointF const pos)
{
	AgxNodeGeometry& geom = agxNodeScene()->agxNodeGeometry();
	QTransform scT = agxNodeScene()->agxNodeGraphicsObject(nodeId)->sceneTransform();
	auto gModel = &(agxNodeScene()->agxGraphModel());
	
	if (!gModel) return;

	QMenu cMenu(this);
	if (!gModel->nodeExists(nodeId))
		return;

	/*QAction* setHidden = new QAction("Toggle Hidden Entries");
	cMenu.addAction(setHidden);
	connect(setHidden, &QAction::triggered, this, [this, nodeId]() { agxNodeScene()->undoStack().push(new ToggleNodeHiddenStateCommand(agxNodeScene(), nodeId)); });*/

	AgxGraphDefinition graphDefinition = AgxGraphRegistry::GetInstance().GetGraphDefinition(gModel->getGraphType());
	if (graphDefinition._hasDefault)
	{
		bool unset = gModel->nodeData(nodeId, AgxNodeRole::AlternateState).toBool();
		QString defActionTitle = unset ? "Remove As " : "Set As ";
		QAction* setDefaultAction = cMenu.addAction(defActionTitle + "Default");
		connect(setDefaultAction, &QAction::triggered, this, [this, nodeId, unset]() { 
					agxNodeScene()->undoStack().push(new AgxNodeAltStateCommand(agxNodeScene(), nodeId, unset));
				});
	}

	QSize nodeSize = gModel->nodeData(nodeId, AgxNodeRole::Size).toSize();
	QPointF nodePos = gModel->nodeData(nodeId, AgxNodeRole::Position).toPointF();

	QAction* removePortAction = nullptr;
	QAction* addPortAction = nullptr;

	if (agxNodeScene()->selectedNodes().size() > 0)
	{
		QString nodeGroupItem = agxNodeScene()->selectedNodes().size() > 1 ? "Selected Nodes" : gModel->GetNodeNameProperty(nodeId);
		QAction* addToNodeGroup = cMenu.addAction("Add " + nodeGroupItem + " To Node Group");
		connect(addToNodeGroup, &QAction::triggered, this, [this, nodeId]() { this->ShowNodeGroupMenu(agxNodeScene()->selectedNodes()); });
	}

	QAction* selectNodeGroup = cMenu.addAction("Select Node Group");

	if(gModel->GroupExists(gModel->GetNodeGroup(nodeId)))
	{
		QString str = std::format("Remove From {}", gModel->GetNodeGroup(nodeId).toStdString()).c_str();
		QAction* removeFromNodeGroup = cMenu.addAction(str);
		connect(removeFromNodeGroup, &QAction::triggered, this, [this, nodeId]() { agxNodeScene()->undoStack().push(new RemoveNodeFromGroupCommand(agxNodeScene(), nodeId)); });
	}
	cMenu.addSeparator();
	connect(selectNodeGroup, &QAction::triggered, this, [this, nodeId]() {SelectNodeGroup(nodeId); });

	

	if(gModel->CanSetNodeNameProperty(nodeId))
	{
		QAction* renamePopup = cMenu.addAction("Rename Node");
		connect(renamePopup, &QAction::triggered, this, [this, nodeId]() {
			StringEntryPopup popup(this);
			int result = popup.exec();
			if (result == QDialog::Accepted)
			{	
				agxNodeScene()->undoStack().push(new RenameNodeCommand(agxNodeScene(), nodeId, popup.getInputText()));
			}

			});
	}
	cMenu.addSeparator();
	auto portAllowance = gModel->CanModifyPorts(nodeId);
	if(portAllowance != AgxPortType::None)
	{
		if (pos.x() < nodePos.x() + nodeSize.width() / 2.0f && (portAllowance == AgxPortType::Both || portAllowance == AgxPortType::In))
		{
			addPortAction = cMenu.addAction("Add Input");
			connect(addPortAction, &QAction::triggered, agxNodeScene(), [this, nodeId]()
				{
					//agxModel->addPort(nodeId, AgxPortType::In, agxModel->nodeData(nodeId, AgxNodeRole::InPortCount).toUInt());
					agxNodeScene()->undoStack().push(new AddPortCommand(agxNodeScene(), nodeId, AgxPortType::In));
				});
			for (int i = 0; i < gModel->nodeData(nodeId, AgxNodeRole::InPortCount).toInt(); i++)
			{
				QPointF scPos = geom.portScenePosition(nodeId, AgxPortType::In, i, scT);
				QRectF txtRect = scT.mapRect(geom.agxPortTextRect(nodeId, AgxPortType::In, i));
				txtRect.translate(geom.portTextPosition(nodeId, AgxPortType::In, i));
				if (abs(pos.x() - scPos.x()) < 10 && abs(pos.y() - scPos.y()) < 10 || txtRect.contains(pos))
				{
					
					removePortAction = cMenu.addAction("Remove Input");
					connect(removePortAction, &QAction::triggered, agxNodeScene(), [this, nodeId, i]()
						{
							agxNodeScene()->undoStack().push(new RemovePortCommand(agxNodeScene(), nodeId, AgxPortType::In, i));
						});
				}
			}
		}
		else if((portAllowance == AgxPortType::Both || portAllowance == AgxPortType::Out))
		{
			addPortAction = cMenu.addAction("Add Output");
			connect(addPortAction, &QAction::triggered, agxNodeScene(), [this, nodeId]()
				{
					agxNodeScene()->undoStack().push(new AddPortCommand(agxNodeScene(), nodeId, AgxPortType::Out));
				});
			for (int i = 0; i < gModel->nodeData(nodeId, AgxNodeRole::OutPortCount).toInt(); i++)
			{
				QPointF scPos = geom.portScenePosition(nodeId, AgxPortType::Out, i, scT);
				QRectF txtRect = scT.mapRect(geom.agxPortTextRect(nodeId, AgxPortType::Out, i));
				txtRect.translate(geom.portTextPosition(nodeId, AgxPortType::Out, i));
				if (abs(pos.x() - scPos.x()) < 10 && abs(pos.y() - scPos.y()) < 10 || txtRect.contains(pos))
				{
					removePortAction = cMenu.addAction("Remove Output");
					connect(removePortAction, &QAction::triggered, agxNodeScene(), [this, nodeId, i]()
						{
							agxNodeScene()->undoStack().push(new RemovePortCommand(agxNodeScene(), nodeId, AgxPortType::Out, i));
						});
				}
			}
		}
	}
	
	
	
	
	cMenu.exec(mapToGlobal(mapFromScene(pos)));
}

AgxGraphicsView::AgxGraphicsView(QWidget* parent) : QGraphicsView(parent), _clearSelectionAction(Q_NULLPTR), 
_deleteSelectionAction(Q_NULLPTR), 
_duplicateSelectionAction(Q_NULLPTR), 
_copySelectionAction(Q_NULLPTR), 
_pasteAction(Q_NULLPTR), 
_cutSelectionAction(Q_NULLPTR), 
_centerAction(Q_NULLPTR)
{
	setDragMode(QGraphicsView::ScrollHandDrag);
	setRenderHint(QPainter::Antialiasing);

	if (!_rubberband)
		_rubberband = new QRubberBand(QRubberBand::Rectangle, this);

	auto const& flowViewStyle = AgxStyleCollection::flowViewStyle();

	setBackgroundBrush(flowViewStyle.BackgroundColor);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	setCacheMode(QGraphicsView::CacheBackground);
	//setViewport(new QOpenGLWidget()); //consider at a later date
	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	/*setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

	QTimer* refresh = new QTimer(this);
	connect(refresh, &QTimer::timeout, this, [this]() { viewport()->update(); });
	refresh->start(1000.0f/30.0f);*/

	setScaleRange(0.05, 8);

	// Sets the scene rect to its maximum possible ranges to avoid autu scene range
	// re-calculation when expanding the all QGraphicsItems common rect.
	int maxSize = 32767;
	setSceneRect(-maxSize, -maxSize, (maxSize * 2), (maxSize * 2));

	_toolbar = new QWidget();
	_toolbarLayout = new QHBoxLayout();
	_toolbar->setLayout(_toolbarLayout);
	_toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	_overlayLayout = new QGridLayout();
	_overlayLayout->setContentsMargins(0, 0, 0, 0);
	_overlayLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Preferred), 0, 0);
	_overlayLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Preferred, QSizePolicy::Expanding), 1, 0);
	_overlayLayout->addWidget(_toolbar, 0, 1);
	setLayout(_overlayLayout);


}

AgxGraphicsView::AgxGraphicsView(AgxGraphicsScene* scene, QWidget* parent) : AgxGraphicsView(parent)
{
	setScene(scene);

	connect(&scene->agxGraphModel(),&AgxGraphModel::nodeCreated, this, [this](AgxNodeId nodeId) {
		auto sidebar = agxNodeScene()->agxGraphModel().GetNodeSidebarContent(nodeId);
		if (auto iagx = dynamic_cast<AgxSidebarContent*>(sidebar)) {
			iagx->SetRefData(nodeId, agxNodeScene());
		}
		Q_EMIT broadcastSidebarItem(sidebar);
			});

	if (!_rubberband)
		_rubberband = new QRubberBand(QRubberBand::Rectangle, this);
}

AgxGraphicsView::~AgxGraphicsView()
{
	
}

QAction* AgxGraphicsView::clearSelectionAction() const
{
	return _clearSelectionAction;
}

QAction* AgxGraphicsView::deleteSelectionAction() const
{
	return _deleteSelectionAction;
}

void AgxGraphicsView::setScene(AgxGraphicsScene* scene)
{
	QGraphicsView::setScene(scene);
	if (!scene)
	{
		// Clear actions.
		delete _clearSelectionAction;
		delete _deleteSelectionAction;
		delete _duplicateSelectionAction;
		delete _copySelectionAction;
		delete _pasteAction;
		_clearSelectionAction = nullptr;
		_deleteSelectionAction = nullptr;
		_duplicateSelectionAction = nullptr;
		_copySelectionAction = nullptr;
		_pasteAction = nullptr;

		_undoAction = nullptr;
		_redoAction = nullptr;

		delete _centerAction;
		_centerAction = nullptr;

		delete _hideAction;
		_hideAction = nullptr;

		delete _unhideAction;
		_unhideAction = nullptr;

		delete _cutSelectionAction;
		_cutSelectionAction = nullptr;

		delete _saveGraphModelAction;
		_saveGraphModelAction = nullptr;

		return;
	}

	{
		// setup actions
		delete _clearSelectionAction;
		_clearSelectionAction = new QAction(QStringLiteral("Clear Selection"), this);
		_clearSelectionAction->setShortcut(Qt::Key_Escape);

		connect(_clearSelectionAction, &QAction::triggered, scene, &QGraphicsScene::clearSelection);

		addAction(_clearSelectionAction);
	}

	{
		delete _deleteSelectionAction;
		_deleteSelectionAction = new QAction(QStringLiteral("Delete"), this);
		_deleteSelectionAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_deleteSelectionAction->setShortcut(QKeySequence(QKeySequence::Delete));
		_deleteSelectionAction->setAutoRepeat(false);
		connect(_deleteSelectionAction,
			&QAction::triggered,
			this,
			&AgxGraphicsView::onDeleteSelectedObjects);

		addAction(_deleteSelectionAction);
	}

	{
		delete _duplicateSelectionAction;
		_duplicateSelectionAction = new QAction(QStringLiteral("Duplicate"), this);
		_duplicateSelectionAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_duplicateSelectionAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
		_duplicateSelectionAction->setAutoRepeat(false);
		connect(_duplicateSelectionAction,
			&QAction::triggered,
			this,
			&AgxGraphicsView::onDuplicateSelectedObjects);

		addAction(_duplicateSelectionAction);
	}

	{
		delete _copySelectionAction;
		_copySelectionAction = new QAction(QStringLiteral("Copy"), this);
		_copySelectionAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_copySelectionAction->setShortcut(QKeySequence(QKeySequence::Copy));
		_copySelectionAction->setAutoRepeat(false);
		connect(_copySelectionAction,
			&QAction::triggered,
			this,
			&AgxGraphicsView::onCopySelectedObjects);

		addAction(_copySelectionAction);
	}

	{
		delete _cutSelectionAction;
		_cutSelectionAction = new QAction(QStringLiteral("Cut"), this);
		_cutSelectionAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_cutSelectionAction->setShortcut(QKeySequence(QKeySequence::Cut));
		_cutSelectionAction->setAutoRepeat(false);
		connect(_cutSelectionAction,
			&QAction::triggered,
			this,
			&AgxGraphicsView::onCutSelectionObjects);

		addAction(_cutSelectionAction);
	}

	{
		delete _pasteAction;
		_pasteAction = new QAction(QStringLiteral("Paste"), this);
		_pasteAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_pasteAction->setShortcut(QKeySequence(QKeySequence::Paste));
		_pasteAction->setAutoRepeat(false);
		connect(_pasteAction, &QAction::triggered, this, &AgxGraphicsView::onPasteObjects);

		addAction(_pasteAction);
	}

	auto undoAction = scene->undoStack().createUndoAction(this, tr("&Undo"));
	undoAction->setShortcuts(QKeySequence::Undo);
	addAction(undoAction);
	_undoAction = undoAction;

	auto redoAction = scene->undoStack().createRedoAction(this, tr("&Redo"));
	redoAction->setShortcuts(QKeySequence::Redo);
	addAction(redoAction);
	_redoAction = redoAction;

	{
		delete _centerAction;
		_centerAction = new QAction("Center View", this);
		_centerAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_centerAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Space));
		_centerAction->setAutoRepeat(false);
		connect(_centerAction, &QAction::triggered, this, &AgxGraphicsView::centerAction);
		addAction(_centerAction);
	}

	{
		delete _hideAction;
		_hideAction = new QAction("Hide Selected", this);
		_hideAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_hideAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key::Key_H));
		_hideAction->setAutoRepeat(false);
		connect(_hideAction, &QAction::triggered, this, &AgxGraphicsView::hideSelectedObjects);
		addAction(_hideAction);
	}

	{
		delete _unhideAction;
		_unhideAction = new QAction("Unhide Selected", this);
		_unhideAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_unhideAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key::Key_U));
		_unhideAction->setAutoRepeat(false);
		connect(_unhideAction, &QAction::triggered, this, &AgxGraphicsView::unhideSelectedObjects);
		addAction(_unhideAction);
	}

	{
		delete _selectAllAction;
		_selectAllAction = new QAction("Select All", this);
		_selectAllAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_selectAllAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key::Key_A));
		_selectAllAction->setAutoRepeat(false);
		connect(_selectAllAction, &QAction::triggered, scene, &AgxGraphicsScene::onSelectAnyAndAllObjects);
		addAction(_selectAllAction);
	}

	{
		delete _selectAllNodesAction;
		_selectAllNodesAction = new QAction("Select All Nodes", this);
		_selectAllNodesAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_selectAllNodesAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key::Key_N));
		_selectAllNodesAction->setAutoRepeat(false);
		connect(_selectAllNodesAction, &QAction::triggered, scene, &AgxGraphicsScene::onSelectAllNodes);
		addAction(_selectAllNodesAction);
	}

	{
		delete _selectAllConnectionsAction;
		_selectAllConnectionsAction = new QAction("Select All Connections ", this);
		_selectAllConnectionsAction->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		_selectAllConnectionsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key::Key_L));
		_selectAllConnectionsAction->setAutoRepeat(false);
		connect(_selectAllConnectionsAction, &QAction::triggered, scene, &AgxGraphicsScene::onSelectAllConnections);
		addAction(_selectAllConnectionsAction);
	}

	{
		delete _selectionFilterMenu;
		delete _selectFilterNodes;
		delete _selectFilterConnections;
		_selectionFilterMenu = new QMenu("Filter Selection", this);

		_selectFilterNodes = _selectionFilterMenu->addAction("Nodes", QKeySequence(Qt::ALT | Qt::Key::Key_N));
		_selectFilterNodes->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		connect(_selectFilterNodes, &QAction::triggered, this, &AgxGraphicsView::FilterSelection_Nodes);
		addAction(_selectFilterNodes);

		_selectFilterConnections = _selectionFilterMenu->addAction("Connections", QKeySequence(Qt::ALT | Qt::Key::Key_L));
		_selectFilterConnections->setShortcutContext(Qt::ShortcutContext::WidgetShortcut);
		connect(_selectFilterConnections, &QAction::triggered, this, &AgxGraphicsView::FilterSelection_Connections);
		addAction(_selectFilterConnections);
	}

}

void AgxGraphicsView::centerSelection()
{
	if (!scene() || scene()->items().isEmpty())
		return;

	QList<QGraphicsItem*> list;

	if (!scene()->selectedItems().isEmpty())
		list = scene()->selectedItems();
	else
		list = scene()->items();

	QRectF boundingRectF = list.first()->sceneBoundingRect();

	for (unsigned int i = 1; i < list.size(); i++)
	{
		boundingRectF = boundingRectF.united(list.at(i)->sceneBoundingRect());
	}

	fitInView(boundingRectF, Qt::KeepAspectRatio);
}

void AgxGraphicsView::centerAction()
{
	if (agxNodeScene()->items().isEmpty()) {
		centerScene();
	}
	else {
		centerSelection();
	}
}

void AgxGraphicsView::centerScene()
{
	if (scene()) {
		scene()->setSceneRect(QRectF());

		QRectF sceneRect = scene()->sceneRect();

		if (sceneRect.width() > this->rect().width() || sceneRect.height() > this->rect().height()) {
			fitInView(sceneRect, Qt::KeepAspectRatio);
		}

		centerOn(sceneRect.center());
	}
}

void AgxGraphicsView::setScaleRange(double minimum, double maximum)
{
	if (maximum < minimum)
		std::swap(minimum, maximum);
	minimum = std::max(0.0, minimum);
	maximum = std::max(0.0, maximum);

	_scaleRange = { minimum, maximum };

	setupScale(transform().m11());
}

void AgxGraphicsView::setScaleRange(ScaleRange range)
{
	setScaleRange(range.minimum, range.maximum);
}

double AgxGraphicsView::getScale() const
{
	return transform().m11();
}

void AgxGraphicsView::scaleDown()
{
	double const step = 1.2;
	double const factor = std::pow(step, -1.0);

	if (_scaleRange.minimum > 0) {
		QTransform t = transform();
		t.scale(factor, factor);
		if (t.m11() <= _scaleRange.minimum) {
			setupScale(t.m11());
			return;
		}
	}

	scale(factor, factor);
	Q_EMIT scaleChanged(transform().m11());
}

void AgxGraphicsView::setupScale(double scale)
{
	scale = std::max(_scaleRange.minimum, std::min(_scaleRange.maximum, scale));

	if (scale <= 0)
		return;

	if (scale == transform().m11())
		return;

	QTransform matrix;
	matrix.scale(scale, scale);
	setTransform(matrix, false);

	Q_EMIT scaleChanged(scale);
}

void AgxGraphicsView::onDeleteSelectedObjects()
{
	if (!agxNodeScene()) return;

	agxNodeScene()->undoStack().push(new DeleteCommand(agxNodeScene()));
}

void AgxGraphicsView::onDuplicateSelectedObjects()
{
	if (!agxNodeScene()) return;

	QPointF const pastePosition = scenePastePosition();

	agxNodeScene()->undoStack().push(new CopyCommand(agxNodeScene()));
	agxNodeScene()->undoStack().push(new PasteCommand(agxNodeScene(), pastePosition));
}

void AgxGraphicsView::onCopySelectedObjects()
{
	if (!agxNodeScene()) return;

	agxNodeScene()->undoStack().push(new CopyCommand(agxNodeScene()));
}

void AgxGraphicsView::onCutSelectionObjects()
{
	if (!agxNodeScene()) return;

	agxNodeScene()->undoStack().push(new CopyCommand(agxNodeScene()));
	agxNodeScene()->undoStack().push(new DeleteCommand(agxNodeScene(),"Cut"));
}

void AgxGraphicsView::onPasteObjects()
{
	if (!agxNodeScene()) return;

	QPointF const pastePosition = scenePastePosition();
	agxNodeScene()->undoStack().push(new PasteCommand(agxNodeScene(), pastePosition));
}

void AgxGraphicsView::hideSelectedObjects()
{
	agxNodeScene()->undoStack().push(new AgxHideCommand(agxNodeScene(), true));
}
void AgxGraphicsView::unhideSelectedObjects()
{
	agxNodeScene()->undoStack().push(new AgxHideCommand(agxNodeScene(), false));
}

void AgxGraphicsView::scaleUp()
{
	double const step = 1.2;
	double const factor = std::pow(step, 1.0);

	if (_scaleRange.maximum > 0) {
		QTransform t = transform();
		t.scale(factor, factor);
		if (t.m11() >= _scaleRange.maximum) {
			setupScale(t.m11());
			return;
		}
	}

	scale(factor, factor);
	Q_EMIT scaleChanged(transform().m11());
}

void AgxGraphicsView::contextMenuEvent(QContextMenuEvent* event)
{
	if (auto agxItem = dynamic_cast<AgxArrowGraphicsObject*>(itemAt(event->pos()))) {
		//
	} 
	else if(itemAt(event->pos()))
	{
		QGraphicsView::contextMenuEvent(event);
		return;
	}

	if (!agxNodeScene()) return;

	auto const scenePos = mapToScene(event->pos());

	QMenu* menu = agxNodeScene()->createSceneMenu(scenePos);

	if (menu) {
		menu->exec(event->globalPos());
	}

	//changes cursor/drag mode if exiting right clicked context menu while rubberbanding
	if (dragMode() == QGraphicsView::NoDrag)
		setDragMode(QGraphicsView::ScrollHandDrag);
}

void AgxGraphicsView::wheelEvent(QWheelEvent* event)
{
	QPoint delta = event->angleDelta();

	if (delta.y() == 0) {
		event->ignore();
		return;
	}

	double const d = delta.y() / std::abs(delta.y());

	if (d > 0.0)
		scaleUp();
	else
		scaleDown();
}

void AgxGraphicsView::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Shift && agxNodeScene()->lastHoveredNode)
	{
		agxNodeScene()->setGroupHoverState(true, agxNodeScene()->getLastHoveredGroup());
	}
	else
	{
		switch (event->key()) {
		case Qt::Key_Shift:
			setDragMode(QGraphicsView::NoDrag);
			viewport()->setCursor(Qt::CrossCursor);
			break;
		case Qt::Key_Control:
			setDragMode(QGraphicsView::NoDrag);
			viewport()->setCursor(Qt::PointingHandCursor);
			break;
		default:			
			break;
		}
	}

	QGraphicsView::keyPressEvent(event); //skip the event on the parent that sets the drag mode.
}

void AgxGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
	
	if (event->key() == Qt::Key_Shift && agxNodeScene()->lastHoveredNode)
	{
		agxNodeScene()->setGroupHoverState(false, "");
	}
	/*switch (event->key()) {
	case Qt::Key_Shift:
	case Qt::Key_Control:
		
		break;
	default:
		break;
	}*/

	if ((event->modifiers() & Qt::ShiftModifier) != 0)
	{
		viewport()->setCursor(Qt::CrossCursor);
		setDragMode(QGraphicsView::NoDrag);
	}
	else if ((event->modifiers() & Qt::ControlModifier) != 0)
	{
		viewport()->setCursor(Qt::PointingHandCursor);
		setDragMode(QGraphicsView::NoDrag);
	}
	else
	{
		setDragMode(QGraphicsView::ScrollHandDrag);
		_rubberband->hide();
	}
	
	QGraphicsView::keyReleaseEvent(event);
}

void AgxGraphicsView::mousePressEvent(QMouseEvent* event)
{
	if (dragMode() == QGraphicsView::NoDrag) 
	{
		if((event->modifiers() & Qt::ShiftModifier) == 0 && (event->modifiers() & Qt::ControlModifier) == 0)
			setDragMode(QGraphicsView::ScrollHandDrag);
	}

	if (event->button() == Qt::MiddleButton) {
		// Create a fake left-click event at the same position
		QMouseEvent fakePress(QEvent::MouseButtonPress, event->pos(), event->globalPosition(),
							  Qt::LeftButton, Qt::LeftButton, event->modifiers() | Qt::AltModifier);
		// Pass the fake event to the base class handler
		QGraphicsView::mousePressEvent(&fakePress);
	} else if (viewport()->cursor() == Qt::CrossCursor && event->button() == Qt::LeftButton && (event->modifiers() & Qt::AltModifier) == 0) {

		_rubberband->setGeometry(QRect(event->pos(), QSize()));
		_rubberband->show();
		event->accept();

	} else {
		QGraphicsView::mousePressEvent(event);
	}
	if (event->button() == Qt::LeftButton || event->button() == Qt::MiddleButton) {
		_clickPos = mapToScene(event->pos());
	}
	
}

void AgxGraphicsView::rubberBandAgxItemsSelection(AgxGraphicsItemsFlags flags)
{
	QRect rect = _rubberband->geometry();
	//scene()->blockSignals(true);
	QPainterPath path;
	path.addRect(mapToScene(rect).boundingRect());
	//scene()->setSelectionArea(path);
	//scene()->blockSignals(false);

	QSet<AgxNodeId> nodesToSelect;
	QSet<AgxConnectionId> connectionsToSelect;

	for (auto item : scene()->items(path))
	{
		if (auto node = dynamic_cast<AgxNodeGraphicsObject*>(item))
			nodesToSelect.insert(node->nodeId());
		else if (auto conn = dynamic_cast<AgxConnectionGraphicsObject*>(item))
			connectionsToSelect.insert(conn->connectionId());
	}

	if (flags.testFlag(AgxGraphicsItemsFlag::Node))
		agxNodeScene()->onSelectNodes(nodesToSelect.values());

	if (flags.testFlag(AgxGraphicsItemsFlag::Connection))
		agxNodeScene()->onSelectedConnections(connectionsToSelect.values());
}

void AgxGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton) {
		// Create a fake left-release event at the same position
		QMouseEvent fakeRelease(QEvent::MouseButtonRelease, event->pos(), event->globalPosition(),
								Qt::LeftButton, Qt::LeftButton, event->modifiers() | Qt::AltModifier);
		// Pass the fake event to the base class handler
		QGraphicsView::mouseReleaseEvent(&fakeRelease);
	} else {
		QGraphicsView::mouseReleaseEvent(event);
	}

	if (_rubberband->isVisible())
	{
		_rubberband->hide();
		rubberBandAgxItemsSelection();
	}
}

void AgxGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
	if (!scene()) return;

	if (viewport()->cursor() == Qt::CrossCursor && event->buttons() == Qt::LeftButton) {
		_rubberband->setGeometry(QRect(mapFromScene(_clickPos.toPoint()), event->pos()).normalized());
		event->accept();
		return;
	}

	QGraphicsView::mouseMoveEvent(event);

	if (scene()->mouseGrabberItem() == nullptr && (event->buttons() == Qt::LeftButton )) {
		// Make sure control and shift are not pressed
		if (viewport()->cursor() != Qt::CrossCursor && viewport()->cursor() != Qt::PointingHandCursor) {
			QPointF difference = _clickPos - mapToScene(event->pos());
			setSceneRect(sceneRect().translated(difference.x(), difference.y()));
		}
	}
}

void AgxGraphicsView::drawBackground(QPainter* painter, const QRectF& r)
{
	QGraphicsView::drawBackground(painter, r);

	int lod = 0; 
	auto scale = getScale();
	if (scale > 1.0)
		lod = 0;
	else if (scale > 0.5)
		lod = 1;
	else if (scale > 0.1)
		lod = 2;
	else
		lod = 3;

	auto drawGrid = [&](double gridStep) {
		QRect windowRect = rect();
		QPointF tl = mapToScene(windowRect.topLeft());
		QPointF br = mapToScene(windowRect.bottomRight());

		double left = std::floor(tl.x() / gridStep - 0.5);
		double right = std::floor(br.x() / gridStep + 1.0);
		double bottom = std::floor(tl.y() / gridStep - 0.5);
		double top = std::floor(br.y() / gridStep + 1.0);

		// vertical lines
		for (int xi = int(left); xi <= int(right); ++xi) {
			QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);

			painter->drawLine(line);
		}

		// horizontal lines
		for (int yi = int(bottom); yi <= int(top); ++yi) {
			QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
			painter->drawLine(line);
		}
		};

	auto const& flowViewStyle = AgxStyleCollection::flowViewStyle();



	if(lod < 2){
		QPen pfine(flowViewStyle.FineGridColor, 1.0);

		painter->setPen(pfine);
		drawGrid(15);
	}

	if(lod < 3){
		QPen p(flowViewStyle.CoarseGridColor, 1.0);

		painter->setPen(p);
		drawGrid(150);
	}

	QPen pRed(Qt::darkGray, 1.5);
	painter->setPen(pRed);
	drawGrid(100000);
}

void AgxGraphicsView::showEvent(QShowEvent* event)
{
	QGraphicsView::showEvent(event);

	if(_firstOpen) //need to add setting to always center screen after first open
	{
		centerScene();
		_firstOpen = false;
	}
}



void AgxGraphicsView::ShowNodeGroupMenu(const std::vector<AgxNodeId>& nodeIds)
{
	QDialog* gWindow = new QDialog();
	QGridLayout* pgrid = new QGridLayout();
	NodeGroupMenuPopup* menu = new NodeGroupMenuPopup(nullptr, *agxNodeScene(), false);
	pgrid->addWidget(menu);
	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	pgrid->addWidget(buttonBox);
	gWindow->setLayout(pgrid);
	gWindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	gWindow->setContentsMargins(3, 3, 3, 3);

	connect(buttonBox, &QDialogButtonBox::accepted, gWindow, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, gWindow, &QDialog::close);

	int result = gWindow->exec();
	if (result == QDialog::Accepted)
	{
		QString group = menu->GetSelectedGroup();
		//if (group != "")
			//agxScene->agxGraphModel().AddToNodeGroup(nodeId, group);
		//agxNodeScene()->undoStack().push(new AddNodeToGroupCommand(agxNodeScene(), group, nodeId));
		std::unordered_map<AgxNodeId, QString> output;
		for (auto& id : nodeIds)
		{
			output.insert({id, agxNodeScene()->agxGraphModel().GetNodeGroup(id)});
		}
		agxNodeScene()->undoStack().push(new AddSelectedToGroupCommand(agxNodeScene(), group, output));
	}

	if (gWindow)
		gWindow->deleteLater();
	//gWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void AgxGraphicsView::OnNodePreClicked(const AgxNodeId nodeId, bool additive)
{
	SelectNodeGroup(nodeId, additive);
}

void AgxGraphicsView::FilterSelection_Nodes()
{
	scene()->blockSignals(true);
	for (auto item : scene()->selectedItems()) {
		if (auto node = dynamic_cast<AgxNodeGraphicsObject*>(item)) {

		} 
		else item->setSelected(false);
	}
	agxNodeScene()->onRightRefreshSideBarVisibility();
	scene()->blockSignals(false);	
}

void AgxGraphicsView::FilterSelection_Connections()
{
	scene()->blockSignals(true);
	for (auto item : scene()->selectedItems()) {
		if (auto node = dynamic_cast<AgxConnectionGraphicsObject*>(item)) {
		
		} 
		else item->setSelected(false);
	}
	agxNodeScene()->onRightRefreshSideBarVisibility();
	scene()->blockSignals(false);
}

AgxGraphicsScene* AgxGraphicsView::agxNodeScene()
{
	return dynamic_cast<AgxGraphicsScene*>(scene());
}

QPointF AgxGraphicsView::scenePastePosition()
{
	QPoint origin = mapFromGlobal(QCursor::pos());

	QRect const viewRect = rect();
	if (!viewRect.contains(origin))
		origin = viewRect.center();

	return mapToScene(origin);;
}

AgxGraphicsScene* AgxGraphicsView::pagxNodeScene()
{
	return agxNodeScene();
}

QUndoStack& AgxGraphicsView::undoStackRef()
{
	return agxNodeScene()->undoStack();
}

QAction* AgxGraphicsView::undoActionRef()
{
	return _undoAction;
}

QAction* AgxGraphicsView::redoActionRef()
{
	return _redoAction;
}

QAction* AgxGraphicsView::cutActionRef()
{
	return _cutSelectionAction;
}

QAction* AgxGraphicsView::copyActionRef()
{
	return _copySelectionAction;
}

QAction* AgxGraphicsView::pasteActionRef()
{
	return _pasteAction;
}

QAction* AgxGraphicsView::duplicateActionRef()
{
	return _duplicateSelectionAction;
}

QAction* AgxGraphicsView::deleteActionRef()
{
	return _deleteSelectionAction;
}

QAction* AgxGraphicsView::selectAllNodesActionRef()
{
	_selectAllNodesAction->setEnabled(agxNodeScene()->nodeGraphicItemCount() != 0);

	return _selectAllNodesAction;
}
QAction* AgxGraphicsView::selectAllConnectionsActionRef()
{
	_selectAllConnectionsAction->setEnabled(agxNodeScene()->connectionGraphicItemCount() != 0);

	return _selectAllConnectionsAction;
}
QMenu* AgxGraphicsView::selectionFilterMenu()
{
	_selectionFilterMenu->setEnabled(agxNodeScene()->selectedItems().count() != 0);

	return _selectionFilterMenu;
}
QAction* AgxGraphicsView::selectAllActionRef()
{
	_selectAllAction->setEnabled(agxNodeScene()->nodeGraphicItemCount() != 0 || agxNodeScene()->connectionGraphicItemCount() != 0);

	return _selectAllAction;
}

QAction* AgxGraphicsView::centerActionRef()
{
	return _centerAction;
}

QAction* AgxGraphicsView::hideActionRef()
{
	return _hideAction;
}

QAction* AgxGraphicsView::unhideActionRef()
{
	return _unhideAction;
}

QHBoxLayout* AgxGraphicsView::getToolBarLayout()
{
	return _toolbarLayout;
}
