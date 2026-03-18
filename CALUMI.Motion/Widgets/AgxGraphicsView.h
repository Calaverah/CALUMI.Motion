//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QGraphicsView>
#include <Utilities/AgxDefinitions.h>
#pragma warning(pop)

class AgxGraphicsScene;
class AgxGraphModel;

class AgxGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	struct ScaleRange
	{
		double minimum = 0;
		double maximum = 0;
	};
public:
	AgxGraphicsView(QWidget* parent = Q_NULLPTR);
	AgxGraphicsView(AgxGraphicsScene* scene, QWidget* parent = Q_NULLPTR);

	~AgxGraphicsView();

	AgxGraphicsView(const AgxGraphicsView&) = delete;
	AgxGraphicsView operator=(const AgxGraphicsView&) = delete;

	QAction* clearSelectionAction() const;

	QAction* deleteSelectionAction() const;

	void setScene(AgxGraphicsScene* scene);

public Q_SLOTS:
	void centerScene();
	void centerSelection();
	void centerAction();

public:
	/// @brief max=0/min=0 indicates infinite zoom in/out
	void setScaleRange(double minimum = 0, double maximum = 0);

	void setScaleRange(ScaleRange range);

	double getScale() const;

public Q_SLOTS:
	void scaleUp();
	void scaleDown();
	void setupScale(double scale);
	virtual void onDeleteSelectedObjects();
	virtual void onDuplicateSelectedObjects();
	virtual void onCopySelectedObjects();
	virtual void onCutSelectionObjects();
	virtual void onPasteObjects();

public Q_SLOTS:
	void hideSelectedObjects();
	void unhideSelectedObjects();

Q_SIGNALS:
	void scaleChanged(double scale);
	void broadcastSidebarItem(QWidget* input);

protected:
	void rubberBandAgxItemsSelection(AgxGraphicsItemsFlags flags = AgxGraphicsItemsFlag::All);

	void contextMenuEvent(QContextMenuEvent* event) override;

	void wheelEvent(QWheelEvent* event) override;

	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

	void drawBackground(QPainter* painter, const QRectF& r) override;

	void showEvent(QShowEvent* event) override;

public Q_SLOTS:
	void ShowContextMenu(AgxNodeId const nodeId, QPointF const pos);
	void ToggleNodeCollapse(const AgxNodeId nodeId);
	void SelectNodeGroup(const AgxNodeId nodeId, bool additive = false);
	void SelectNodeGroup(const QString nodeGroup, AgxGraphModel* agxModel = nullptr, bool additive = false);
	void ShowNodeGroupMenu(const std::vector<AgxNodeId>& nodeIds);
	void OnNodePreClicked(const AgxNodeId nodeId, bool additive);
	void FilterSelection_Nodes();
	void FilterSelection_Connections();

protected:
	AgxGraphicsScene* agxNodeScene();
	QPointF scenePastePosition();

public:
	AgxGraphicsScene* pagxNodeScene();

	QUndoStack& undoStackRef();
	QAction* undoActionRef();
	QAction* redoActionRef();
	QAction* cutActionRef();
	QAction* copyActionRef();
	QAction* pasteActionRef();
	QAction* duplicateActionRef();
	QAction* deleteActionRef();

	QAction* selectAllActionRef();
	QAction* selectAllNodesActionRef();
	QAction* selectAllConnectionsActionRef();
	QMenu* selectionFilterMenu();

	QAction* centerActionRef();
	QAction* hideActionRef();
	QAction* unhideActionRef();

	QHBoxLayout* getToolBarLayout();

private:
	QAction* _clearSelectionAction = nullptr;
	QAction* _deleteSelectionAction = nullptr;
	QAction* _duplicateSelectionAction = nullptr;
	QAction* _copySelectionAction = nullptr;
	QAction* _pasteAction = nullptr;
	QAction* _cutSelectionAction = nullptr;

	QAction* _selectAllAction = nullptr;
	QAction* _selectAllNodesAction = nullptr;
	QAction* _selectAllConnectionsAction = nullptr;

	QMenu* _selectionFilterMenu = nullptr;
	QAction* _selectFilterNodes = nullptr;
	QAction* _selectFilterConnections = nullptr;
	
	QAction* _saveGraphModelAction = nullptr;

	QAction* _centerAction = nullptr;
	QAction* _hideAction = nullptr;
	QAction* _unhideAction = nullptr;

	QAction* _undoAction = nullptr;
	QAction* _redoAction = nullptr;

	QPointF _clickPos;
	ScaleRange _scaleRange;

	QWidget* _toolbar;
	QHBoxLayout* _toolbarLayout;
	QGridLayout* _overlayLayout;

	QRubberBand* _rubberband = nullptr;

	bool _firstOpen = true;
};