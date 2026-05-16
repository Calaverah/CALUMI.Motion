//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "Utilities/AgxDefinitions.h"

class AgxGraphModel;

class AgxNodeGeometry
{
public:
	explicit AgxNodeGeometry(AgxGraphModel& graphModel);

	QRect groupIconPosition(AgxNodeId nodeId) const;

	QRectF agxPortTextRect(AgxNodeId nodeId,
						   AgxPortType portType,
						   AgxPortIndex portIndex) const;
	QRectF titleCaptionRect(const QString& string) const;
	QRectF subtitleCaptionRect(const QString& string) const;

private:
	QRectF portTextRect(AgxNodeId nodeId,
						AgxPortType portType,
						AgxPortIndex portIndex) const;
	/// Finds max number of ports and multiplies by (a port height + interval)
	unsigned int maxVerticalPortsExtent(AgxNodeId nodeId) const;

	unsigned int maxPortsTextAdvance(AgxNodeId nodeId, AgxPortType portType) const;

public:
	// Inherited via AbstractNodeGeometry
	QRectF boundingRect(AgxNodeId nodeId) const;
	QRectF collapsedBoundingRect(const AgxNodeId& nodeId) const;
	QSize size(AgxNodeId nodeId) const;
	void recomputeSize(AgxNodeId nodeId) const;
	QSize collapsedSize(AgxNodeId nodeId) const;
	QPointF collapsedPortPosition(const AgxNodeId& nodeId, const AgxPortType& portType) const;
	QPointF portPosition(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex) const;
	QPointF portScenePosition(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex index, QTransform const& t) const;
	QPointF portTextPosition(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex) const;

	QPointF captionPosition(AgxNodeId nodeId) const;
	QPair<QPointF, QPointF> dualCaptionPosition(const AgxNodeId& nodeId, const QString& title, const QString& subtitle, bool collapsed = false) const;
	QPair<QPointF, QPointF> dualCaptionPosition(const AgxNodeId& nodeId) const;

	QRectF captionRect(AgxNodeId nodeId) const;

	QPointF widgetPosition(AgxNodeId nodeId) const;
	QRect resizeHandleRect(AgxNodeId nodeId) const;
	AgxPortIndex checkPortHit(AgxNodeId nodeId, AgxPortType portType, QPointF nodePoint) const;

protected:
	mutable unsigned int m_portSize;
	unsigned int m_portSpacing;
	mutable QFontMetrics m_fontMetrics;
	mutable QFontMetrics m_boldFontMetrics;

AgxGraphModel& m_graphModel;

};