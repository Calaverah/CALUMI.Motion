//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QWidget>
#include "ui_NodeGroupMenuPopup.h"
#pragma warning(pop)
#include "Models/AgxGraphicsScene.h"
#include "Widgets/AgxGraphicsView.h"
#include "Painter/QColorHelper.h"

class NodeGroupMenuPopup : public QWidget
{
	Q_OBJECT

public:
	NodeGroupMenuPopup(QWidget *parent, AgxGraphicsScene& scene, bool showNodeTree = true);
	NodeGroupMenuPopup(QWidget *parent, AgxGraphicsScene& scene, AgxGraphicsView* view, QTabWidget* tab,  bool showNodeTree = true);
	~NodeGroupMenuPopup();

	QString GetSelectedGroup();
	void SetActiveNodeGroupTreeItem(const QString& group);

	QColor GetNewColorDialog(QColor initial = generateRandomQColor());

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void changeEvent(QEvent* event) override;

public Q_SLOTS:
	void prepareNodeGroupTreeMenu(const QPoint& pos);

private:
	void BuildTreeView();

	bool _showNodeTree : 1 = true;
	Ui::NodeGroupMenuPopupClass ui;
	AgxGraphicsScene& _scene;
	AgxGraphicsView* _view;
	QTabWidget* _tab;
	
};

