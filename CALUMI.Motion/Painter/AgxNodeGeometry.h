//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "Utilities/AgxDefinitions.h"

class AgxGraphModel;

class AgxNodeGeometry
{
public:
	AgxNodeGeometry(AgxGraphModel& graphModel);

	QRect groupIconPosition(AgxNodeId const nodeId) const;

	QRectF agxPortTextRect(AgxNodeId const nodeId,
		AgxPortType const portType,
		AgxPortIndex const portIndex) const;
	QRectF titleCaptionRect(const QString& string) const;
	QRectF subtitleCaptionRect(const QString& string) const;

private:
	QRectF portTextRect(AgxNodeId const nodeId,
						AgxPortType const portType,
						AgxPortIndex const portIndex) const;
	/// Finds max number of ports and multiplies by (a port height + interval)
	unsigned int maxVerticalPortsExtent(AgxNodeId const nodeId) const;

	unsigned int maxPortsTextAdvance(AgxNodeId const nodeId, AgxPortType const portType) const;

public:
	// Inherited via AbstractNodeGeometry
	QRectF boundingRect(AgxNodeId const nodeId) const;
	QRectF collapsedBoundingRect(const AgxNodeId& nodeId) const;
	QSize size(AgxNodeId const nodeId) const;
	void recomputeSize(AgxNodeId const nodeId) const;
	QSize collapsedSize(AgxNodeId const nodeId) const;
	QPointF collapsedPortPosition(const AgxNodeId& nodeId, const AgxPortType& portType) const;
	QPointF portPosition(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const;
	QPointF portScenePosition(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const index, QTransform const& t) const;
	QPointF portTextPosition(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const;

	QPointF captionPosition(AgxNodeId const nodeId) const;
	QPair<QPointF, QPointF> dualCaptionPosition(const AgxNodeId& nodeId, const QString& title, const QString& subtitle, bool collapsed = false) const;
	QPair<QPointF, QPointF> dualCaptionPosition(const AgxNodeId& nodeId) const;

	QRectF captionRect(AgxNodeId const nodeId) const;

	QPointF widgetPosition(AgxNodeId const nodeId) const;
	QRect resizeHandleRect(AgxNodeId const nodeId) const;
	AgxPortIndex checkPortHit(AgxNodeId const nodeId, AgxPortType const portType, QPointF const nodePoint) const;

protected:
	mutable unsigned int _portSize;
	unsigned int _portSpacing;
	mutable QFontMetrics _fontMetrics;
	mutable QFontMetrics _boldFontMetrics;

protected:
	AgxGraphModel& _graphModel;

};