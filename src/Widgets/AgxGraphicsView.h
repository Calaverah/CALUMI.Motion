//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QGraphicsView>
#include <Utilities/AgxDefinitions.h>
#include <QRubberBand>
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

	explicit AgxGraphicsView(QWidget* parent = Q_NULLPTR);
	explicit AgxGraphicsView(AgxGraphicsScene* scene, QWidget* parent = Q_NULLPTR);

	~AgxGraphicsView() override = default;

	AgxGraphicsView(const AgxGraphicsView&) = delete;
	AgxGraphicsView operator=(const AgxGraphicsView&) = delete;

	[[nodiscard]] QAction* clearSelectionAction() const;

	[[nodiscard]] QAction* deleteSelectionAction() const;

	void setScene(AgxGraphicsScene* scene);

public Q_SLOTS:
	void centerScene();
	void centerSelection();
	void centerAction();

public:
	/// @brief max=0/min=0 indicates infinite zoom in/out
	void setScaleRange(double minimum = 0, double maximum = 0);

	void setScaleRange(ScaleRange range);

	[[nodiscard]] double getScale() const;

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
	void hideSelectedObjects() const;
	void unhideSelectedObjects() const;

Q_SIGNALS:
	void scaleChanged(double scale);
	void broadcastSidebarItem(QWidget* input);

protected:
	void rubberBandAgxItemsSelection(AgxGraphicsItemsFlags flags = AgxGraphicsItemsFlag::All) const;

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
	void ShowContextMenu(AgxNodeId nodeId, QPointF pos);
	void ToggleNodeCollapse(AgxNodeId nodeId) const;
	void SelectNodeGroup(AgxNodeId nodeId, bool additive = false) const;
	void SelectNodeGroup(const QString& nodeGroup, const AgxGraphModel* agxModel = nullptr, bool additive = false) const;
	void ShowNodeGroupMenu(const QVector<AgxNodeId>& nodeIds);
	void FilterSelection_Nodes() const;
	void FilterSelection_Connections() const;

protected:
	[[nodiscard]] QPointF scenePastePosition() const;

public:
	[[nodiscard]] AgxGraphicsScene* agxNodeScene() const;

	[[nodiscard]] QUndoStack& undoStackRef() const;
	[[nodiscard]] QAction* undoActionRef() const;
	[[nodiscard]] QAction* redoActionRef() const;
	[[nodiscard]] QAction* cutActionRef() const;
	[[nodiscard]] QAction* copyActionRef() const;
	[[nodiscard]] QAction* pasteActionRef() const;
	[[nodiscard]] QAction* duplicateActionRef() const;
	[[nodiscard]] QAction* deleteActionRef() const;

	[[nodiscard]] QAction* selectAllActionRef() const;
	[[nodiscard]] QAction* selectAllNodesActionRef() const;
	[[nodiscard]] QAction* selectAllConnectionsActionRef() const;
	[[nodiscard]] QMenu* selectionFilterMenu() const;

	[[nodiscard]] QAction* centerActionRef() const;
	[[nodiscard]] QAction* hideActionRef() const;
	[[nodiscard]] QAction* unhideActionRef() const;

	[[nodiscard]] QHBoxLayout* getToolBarLayout() const;

private:
	QAction* _clearSelectionAction = Q_NULLPTR;
	QAction* _deleteSelectionAction = Q_NULLPTR;
	QAction* _duplicateSelectionAction = Q_NULLPTR;
	QAction* _copySelectionAction = Q_NULLPTR;
	QAction* _pasteAction = Q_NULLPTR;
	QAction* _cutSelectionAction = Q_NULLPTR;

	QAction* _selectAllAction = Q_NULLPTR;
	QAction* _selectAllNodesAction = Q_NULLPTR;
	QAction* _selectAllConnectionsAction = Q_NULLPTR;

	QMenu* _selectionFilterMenu = Q_NULLPTR;
	QAction* _selectFilterNodes = Q_NULLPTR;
	QAction* _selectFilterConnections = Q_NULLPTR;
	
	QAction* _saveGraphModelAction = Q_NULLPTR;

	QAction* _centerAction = Q_NULLPTR;
	QAction* _hideAction = Q_NULLPTR;
	QAction* _unhideAction = Q_NULLPTR;

	QAction* _undoAction = Q_NULLPTR;
	QAction* _redoAction = Q_NULLPTR;

	QPointF _clickPos;
	ScaleRange _scaleRange;

	QWidget* _toolbar = Q_NULLPTR;
	QHBoxLayout* _toolbarLayout = Q_NULLPTR;
	QGridLayout* _overlayLayout = Q_NULLPTR;

	QRubberBand* _rubberband = Q_NULLPTR;

	bool _firstOpen = true;
};