//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppTooWideScope
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppDFAMemoryLeak
#include "stdafx.h"
#pragma warning(push,0)

#include <QGraphicsScene>
#include <QMenu>
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
// ReSharper disable once CppUnusedIncludeDirective
#include "Utilities/Hash/AgxConnectionIdHash.h"

void AgxGraphicsView::ToggleNodeCollapse(const AgxNodeId nodeId) const
{
	if(const auto agxModel = &agxNodeScene()->agxGraphModel())
		agxModel->ToggleNodeCollapse(nodeId);

}

void AgxGraphicsView::SelectNodeGroup(const AgxNodeId nodeId, const bool additive) const
{
	if (const auto agxModel = &agxNodeScene()->agxGraphModel())
	{
		const auto nodeGroup = agxModel->GetNodeGroup(nodeId);
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

void AgxGraphicsView::SelectNodeGroup(const QString& nodeGroup, const AgxGraphModel* agxModel, const bool additive) const
{
	if (nodeGroup == "")
		return;

	if (!agxModel)
		agxModel = &agxNodeScene()->agxGraphModel();

	bool setting = true;

	if(additive)
	{
		setting = false;
		for (const auto id : agxModel->allNodeIds())
		{
			//is it good to loop twice through to determine if the group is selected? Probably not, but it beats trying to add tons of select/deselect logic for groups elsewhere
			if (agxModel->GetNodeGroup(id) == nodeGroup && !agxNodeScene()->agxNodeGraphicsObject(id)->isSelected())
				setting = true;
		}
	}
	for (const auto id : agxModel->allNodeIds())
	{
		if (agxModel->GetNodeGroup(id) == nodeGroup && agxNodeScene()->agxNodeGraphicsObject(id)->isSelected() != setting)
			agxNodeScene()->agxNodeGraphicsObject(id)->setSelected(setting);
	}
}

void AgxGraphicsView::ShowContextMenu(AgxNodeId const nodeId, QPointF const pos)
{
	const AgxNodeGeometry& geom = agxNodeScene()->agxNodeGeometry();
	const QTransform scT = agxNodeScene()->agxNodeGraphicsObject(nodeId)->sceneTransform();
	const auto gModel = &agxNodeScene()->agxGraphModel();
	
	if (!gModel)
		return;

	QMenu cMenu(this);

	if (!gModel->nodeExists(nodeId))
		return;

	if (const AgxGraphDefinition graphDefinition = AgxGraphRegistry::GetInstance().GetGraphDefinition(gModel->getGraphType()); graphDefinition._hasDefault)
	{
		bool unset = gModel->nodeData(nodeId, AgxNodeRole::AlternateState).toBool();

		const QString defActionTitle = unset ? "Remove As " : "Set As ";
		const QAction* setDefaultAction = cMenu.addAction(defActionTitle + "Default");

		connect(setDefaultAction, &QAction::triggered, this, [this, nodeId, unset] {
					agxNodeScene()->undoStack().push(new AgxNodeAltStateCommand(agxNodeScene(), nodeId, unset));
			});
	}

	const QSize nodeSize = gModel->nodeData(nodeId, AgxNodeRole::Size).toSize();
	const QPointF nodePos = gModel->nodeData(nodeId, AgxNodeRole::Position).toPointF();

	const QAction* removePortAction = nullptr;
	const QAction* addPortAction = nullptr;

	const auto selectedNodes = agxNodeScene()->selectedNodes();

	if (selectedNodes.size() > 0)
	{
		const QString nodeGroupItem = selectedNodes.size() > 1 ? "Selected Nodes" : gModel->GetNodeNameProperty(nodeId);
		const QAction* addToNodeGroup = cMenu.addAction("Add " + nodeGroupItem + " To Node Group");
		connect(addToNodeGroup, &QAction::triggered, this, [this]
		{
			this->ShowNodeGroupMenu(agxNodeScene()->selectedNodes());
		});
	}

	const QAction* selectNodeGroup = cMenu.addAction("Select Node Group");

	if(gModel->GroupExists(gModel->GetNodeGroup(nodeId)))
	{
		const QString str = std::format("Remove From {}", gModel->GetNodeGroup(nodeId).toStdString()).c_str();
		const QAction* removeFromNodeGroup = cMenu.addAction(str);
		connect(removeFromNodeGroup, &QAction::triggered, this, [this, nodeId] { agxNodeScene()->undoStack().push(new RemoveNodeFromGroupCommand(agxNodeScene(), nodeId)); });
	}
	cMenu.addSeparator();
	connect(selectNodeGroup, &QAction::triggered, this, [this, nodeId] {SelectNodeGroup(nodeId); });

	

	if(gModel->CanSetNodeNameProperty(nodeId))
	{
		const QAction* renamePopup = cMenu.addAction("Rename Node");
		connect(renamePopup, &QAction::triggered, this, [this, nodeId] {
			StringEntryPopup popup(this);
			if (const int result = popup.exec(); result == QDialog::Accepted)
			{	
				agxNodeScene()->undoStack().push(new RenameNodeCommand(agxNodeScene(), nodeId, popup.getInputText()));
			}

			});
	}
	cMenu.addSeparator();
	if(const auto portAllowance = gModel->CanModifyPorts(nodeId); portAllowance != AgxPortType::None)
	{
		if (pos.x() < nodePos.x() + nodeSize.width() / 2.0f && (portAllowance == AgxPortType::Both || portAllowance == AgxPortType::In))
		{
			addPortAction = cMenu.addAction("Add Input");

			connect(addPortAction, &QAction::triggered, agxNodeScene(), [this, nodeId]
				{
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
					connect(removePortAction, &QAction::triggered, agxNodeScene(), [this, nodeId, i]
						{
							agxNodeScene()->undoStack().push(new RemovePortCommand(agxNodeScene(), nodeId, AgxPortType::In, i));
						});
				}
			}
		}
		else if(portAllowance == AgxPortType::Both || portAllowance == AgxPortType::Out)
		{
			addPortAction = cMenu.addAction("Add Output");
			connect(addPortAction, &QAction::triggered, agxNodeScene(), [this, nodeId]
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
					connect(removePortAction, &QAction::triggered, agxNodeScene(), [this, nodeId, i]
						{
							agxNodeScene()->undoStack().push(new RemovePortCommand(agxNodeScene(), nodeId, AgxPortType::Out, i));
						});
				}
			}
		}
	}
	
	
	
	
	cMenu.exec(mapToGlobal(mapFromScene(pos)));
}

AgxGraphicsView::AgxGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
	setDragMode(ScrollHandDrag);
	setRenderHint(QPainter::Antialiasing);

	if (!_rubberband)
		_rubberband = new QRubberBand(QRubberBand::Rectangle, this);

	auto const& graphPalette = AgxPalette::GetInstance().graphPalette();

	setBackgroundBrush(graphPalette.BackgroundColor);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setTransformationAnchor(AnchorUnderMouse);

	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);

	setScaleRange(0.05, 8);

	// Sets the scene rect to its maximum possible ranges to avoid auto scene range
	// re-calculation when expanding the all QGraphicsItems common rect.
	constexpr int maxSize = 32767;
	setSceneRect(-maxSize, -maxSize, maxSize * 2, maxSize * 2);

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
	if (scene)
	{
		setScene(scene);

		connect(scene, &AgxGraphicsScene::nodeContextMenu, this, &AgxGraphicsView::ShowContextMenu);
		connect(scene, &AgxGraphicsScene::nodeDoubleClicked, this, &AgxGraphicsView::ToggleNodeCollapse);
		connect(scene, &AgxGraphicsScene::nodePreClicked, this, [this](const AgxNodeId& nodeId, bool additive)
		{
			this->SelectNodeGroup(nodeId, additive);
		});

		connect(&scene->agxGraphModel(),&AgxGraphModel::nodeCreated, this, [this](const AgxNodeId nodeId) {
				const auto sidebar = agxNodeScene()->agxGraphModel().GetNodeSidebarContent(nodeId);
				if (const auto iAgx = dynamic_cast<AgxSidebarContent*>(sidebar))
				{
					iAgx->SetRefData(nodeId, agxNodeScene());
				}

				Q_EMIT broadcastSidebarItem(sidebar);
			});
	}

	if (!_rubberband)
		_rubberband = new QRubberBand(QRubberBand::Rectangle, this);
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

	const auto undoAction = scene->undoStack().createUndoAction(this, tr("&Undo"));
	undoAction->setShortcuts(QKeySequence::Undo);
	addAction(undoAction);
	_undoAction = undoAction;

	const auto redoAction = scene->undoStack().createRedoAction(this, tr("&Redo"));
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

		const QRectF sceneRect = scene()->sceneRect();

		if (sceneRect.width() > this->rect().width() || sceneRect.height() > this->rect().height())
		{
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

void AgxGraphicsView::setScaleRange(const ScaleRange range)
{
	setScaleRange(range.minimum, range.maximum);
}

double AgxGraphicsView::getScale() const
{
	return transform().m11();
}

void AgxGraphicsView::scaleDown()
{
	constexpr double step = 1.2;
	double const factor = std::pow(step, -1.0);

	if (_scaleRange.minimum > 0)
	{
		QTransform t = transform();
		t.scale(factor, factor);

		if (t.m11() <= _scaleRange.minimum)
		{
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

void AgxGraphicsView::hideSelectedObjects() const
{
	agxNodeScene()->undoStack().push(new AgxHideCommand(agxNodeScene(), true));
}
void AgxGraphicsView::unhideSelectedObjects() const
{
	agxNodeScene()->undoStack().push(new AgxHideCommand(agxNodeScene(), false));
}

void AgxGraphicsView::scaleUp()
{
	constexpr double step = 1.2;
	double const factor = std::pow(step, 1.0);

	if (_scaleRange.maximum > 0)
	{
		QTransform t = transform();
		t.scale(factor, factor);
		if (t.m11() >= _scaleRange.maximum)
		{
			setupScale(t.m11());
			return;
		}
	}

	scale(factor, factor);
	Q_EMIT scaleChanged(transform().m11());
}

void AgxGraphicsView::contextMenuEvent(QContextMenuEvent* event)
{
	if (dynamic_cast<AgxArrowGraphicsObject*>(itemAt(event->pos())))
	{
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

	//changes cursor/drag mode if exiting right-clicked context menu while rubberbanding
	if (dragMode() == NoDrag)
		setDragMode(ScrollHandDrag);
}

void AgxGraphicsView::wheelEvent(QWheelEvent* event)
{
	const QPoint delta = event->angleDelta();

	if (delta.y() == 0)
	{
		event->ignore();
		return;
	}

	if (double const d = delta.y() / std::abs(delta.y()); d > 0.0)
		scaleUp();
	else
		scaleDown();
}

void AgxGraphicsView::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Shift && agxNodeScene()->m_lastHoveredNode)
	{
		agxNodeScene()->setGroupHoverState(true, agxNodeScene()->getLastHoveredGroup());
	}
	else
	{
		switch (event->key())
		{
		case Qt::Key_Shift:
			setDragMode(NoDrag);
			viewport()->setCursor(Qt::CrossCursor);
			break;
		case Qt::Key_Control:
			setDragMode(NoDrag);
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
	
	if (event->key() == Qt::Key_Shift && agxNodeScene()->m_lastHoveredNode)
	{
		agxNodeScene()->setGroupHoverState(false, "");
	}

	if ((event->modifiers() & Qt::ShiftModifier) != 0)
	{
		viewport()->setCursor(Qt::CrossCursor);
		setDragMode(NoDrag);
	}
	else if ((event->modifiers() & Qt::ControlModifier) != 0)
	{
		viewport()->setCursor(Qt::PointingHandCursor);
		setDragMode(NoDrag);
	}
	else
	{
		setDragMode(ScrollHandDrag);
		_rubberband->hide();
	}
	
	QGraphicsView::keyReleaseEvent(event);
}

void AgxGraphicsView::mousePressEvent(QMouseEvent* event)
{
	if (dragMode() == NoDrag)
	{
		if((event->modifiers() & Qt::ShiftModifier) == 0 && (event->modifiers() & Qt::ControlModifier) == 0)
			setDragMode(ScrollHandDrag);
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

void AgxGraphicsView::rubberBandAgxItemsSelection(const AgxGraphicsItemsFlags flags) const
{
	const QRect rect = _rubberband->geometry();

	QPainterPath path;
	path.addRect(mapToScene(rect).boundingRect());

	QSet<AgxNodeId> nodesToSelect;
	QSet<AgxConnectionId> connectionsToSelect;

	for (const auto item : scene()->items(path))
	{
		if (const auto node = dynamic_cast<AgxNodeGraphicsObject*>(item))
			nodesToSelect.insert(node->nodeId());
		else if (const auto conn = dynamic_cast<AgxConnectionGraphicsObject*>(item))
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

	if (viewport()->cursor() == Qt::CrossCursor && event->buttons() == Qt::LeftButton)
	{
		_rubberband->setGeometry(QRect(mapFromScene(_clickPos.toPoint()), event->pos()).normalized());
		event->accept();
		return;
	}

	QGraphicsView::mouseMoveEvent(event);

	if (scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton)
	{
		// Make sure control and shift are not pressed
		if (viewport()->cursor() != Qt::CrossCursor && viewport()->cursor() != Qt::PointingHandCursor)
		{
			const QPointF difference = _clickPos - mapToScene(event->pos());
			setSceneRect(sceneRect().translated(difference.x(), difference.y()));
		}
	}
}

void AgxGraphicsView::drawBackground(QPainter* painter, const QRectF& r)
{
	QGraphicsView::drawBackground(painter, r);

	int lod = 0;
	const auto scale = getScale();

	if (scale > 1.0)
		lod = 0;
	else if (scale > 0.5)
		lod = 1;
	else if (scale > 0.1)
		lod = 2;
	else
		lod = 3;

	auto drawGrid = [&](const double gridStep)
	{
		const QRect windowRect = rect();
		const QPointF tl = mapToScene(windowRect.topLeft());
		const QPointF br = mapToScene(windowRect.bottomRight());

		const double left = std::floor(tl.x() / gridStep - 0.5);
		const double right = std::floor(br.x() / gridStep + 1.0);
		const double bottom = std::floor(tl.y() / gridStep - 0.5);
		const double top = std::floor(br.y() / gridStep + 1.0);

		// vertical lines
		for (int xi = static_cast<int>(left); xi <= static_cast<int>(right); ++xi)
		{
			QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);

			painter->drawLine(line);
		}

		// horizontal lines
		for (int yi = static_cast<int>(bottom); yi <= static_cast<int>(top); ++yi)
		{
			QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
			painter->drawLine(line);
		}
	};

	auto const& flowViewStyle = AgxPalette::GetInstance().graphPalette();

	if(lod < 2)
	{
		const QPen pfine(flowViewStyle.FineGridColor, 1.0);

		painter->setPen(pfine);
		drawGrid(15);
	}

	if(lod < 3)
	{
		const QPen p(flowViewStyle.CoarseGridColor, 1.0);

		painter->setPen(p);
		drawGrid(150);
	}

	const QPen pRed(Qt::darkGray, 1.5);
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



void AgxGraphicsView::ShowNodeGroupMenu(const QVector<AgxNodeId>& nodeIds)
{
	const auto gWindow = new QDialog();
	const auto pgrid = new QGridLayout();
	const auto menu = new NodeGroupMenuPopup(nullptr, *agxNodeScene(), false);
	pgrid->addWidget(menu);
	const auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	pgrid->addWidget(buttonBox);
	gWindow->setLayout(pgrid);
	gWindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	gWindow->setContentsMargins(3, 3, 3, 3);

	connect(buttonBox, &QDialogButtonBox::accepted, gWindow, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, gWindow, &QDialog::close);

	const int result = gWindow->exec();

	if (result == QDialog::Accepted)
	{
		const QString group = menu->GetSelectedGroup();
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

	gWindow->deleteLater();
}

void AgxGraphicsView::FilterSelection_Nodes() const
{
	scene()->blockSignals(true);
	for (const auto item : scene()->selectedItems())
	{
		if (dynamic_cast<AgxNodeGraphicsObject*>(item))
		{}
		else
			item->setSelected(false);
	}
	scene()->blockSignals(false);	
}

void AgxGraphicsView::FilterSelection_Connections() const
{
	scene()->blockSignals(true);
	for (const auto item : scene()->selectedItems())
	{
		if (dynamic_cast<AgxConnectionGraphicsObject*>(item))
		{}
		else
			item->setSelected(false);
	}
	scene()->blockSignals(false);
}

QPointF AgxGraphicsView::scenePastePosition() const
{
	QPoint origin = mapFromGlobal(QCursor::pos());

	QRect const viewRect = rect();
	if (!viewRect.contains(origin))
		origin = viewRect.center();

	return mapToScene(origin);
}

AgxGraphicsScene* AgxGraphicsView::agxNodeScene() const
{
	return dynamic_cast<AgxGraphicsScene*>(scene());
}

QUndoStack& AgxGraphicsView::undoStackRef() const
{
	return agxNodeScene()->undoStack();
}

QAction* AgxGraphicsView::undoActionRef() const
{
	return _undoAction;
}

QAction* AgxGraphicsView::redoActionRef() const
{
	return _redoAction;
}

QAction* AgxGraphicsView::cutActionRef() const
{
	return _cutSelectionAction;
}

QAction* AgxGraphicsView::copyActionRef() const
{
	return _copySelectionAction;
}

QAction* AgxGraphicsView::pasteActionRef() const
{
	return _pasteAction;
}

QAction* AgxGraphicsView::duplicateActionRef() const
{
	return _duplicateSelectionAction;
}

QAction* AgxGraphicsView::deleteActionRef() const
{
	return _deleteSelectionAction;
}

QAction* AgxGraphicsView::selectAllNodesActionRef() const
{
	_selectAllNodesAction->setEnabled(agxNodeScene()->nodeGraphicItemCount() != 0);

	return _selectAllNodesAction;
}
QAction* AgxGraphicsView::selectAllConnectionsActionRef() const
{
	_selectAllConnectionsAction->setEnabled(agxNodeScene()->connectionGraphicItemCount() != 0);

	return _selectAllConnectionsAction;
}
QMenu* AgxGraphicsView::selectionFilterMenu() const
{
	_selectionFilterMenu->setEnabled(agxNodeScene()->selectedItems().count() != 0);

	return _selectionFilterMenu;
}
QAction* AgxGraphicsView::selectAllActionRef() const
{
	_selectAllAction->setEnabled(agxNodeScene()->nodeGraphicItemCount() != 0 || agxNodeScene()->connectionGraphicItemCount() != 0);

	return _selectAllAction;
}

QAction* AgxGraphicsView::centerActionRef() const
{
	return _centerAction;
}

QAction* AgxGraphicsView::hideActionRef() const
{
	return _hideAction;
}

QAction* AgxGraphicsView::unhideActionRef() const
{
	return _unhideAction;
}

QHBoxLayout* AgxGraphicsView::getToolBarLayout() const
{
	return _toolbarLayout;
}
