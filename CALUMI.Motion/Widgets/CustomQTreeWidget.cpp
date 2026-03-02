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
