//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "CustomQTreeWidget.h"

CustomQTreeWidget::CustomQTreeWidget(QWidget* parent) : QTreeWidget(parent) {}

void CustomQTreeWidget::mousePressEvent(QMouseEvent* event)
{
	Q_EMIT mousePressEventCalled();
	QTreeWidgetItem* item = itemAt(event->pos());

	if (!item)
	{
		clearSelection();
		Q_EMIT deselectCalled();
	}

	QTreeWidget::mousePressEvent(event);
}

void CustomQTreeWidget::onDeselectCalled()
{
	clearSelection();
}
