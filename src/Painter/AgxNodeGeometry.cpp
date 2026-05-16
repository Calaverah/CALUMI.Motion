//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNodeGeometry.h"
#pragma warning(push,0)
#include "AgxPalette.h"
#include "Utilities/AgxDefinitions.h"
#include "Models/AgxGraphModel.h"
#pragma warning(pop)

AgxNodeGeometry::AgxNodeGeometry(AgxGraphModel& graphModel) : m_portSize(20), m_portSpacing(10), m_fontMetrics(QFont()), m_boldFontMetrics(QFont()), m_graphModel(graphModel)
{
    QFont f;
    f.setBold(true);
    m_boldFontMetrics = QFontMetrics(f);

    m_portSize = m_fontMetrics.height();
}

QRect AgxNodeGeometry::groupIconPosition(AgxNodeId const nodeId) const
{
	const auto size = m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);
    constexpr unsigned int rectSize = 7;
	return QRect(size.width() - m_portSpacing, m_portSpacing, rectSize, rectSize);
}

QRectF AgxNodeGeometry::agxPortTextRect(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const
{
    QString s;
    if (m_graphModel.portData<bool>(nodeId, portType, portIndex, AgxPortRole::CaptionVisible)) {
        s = m_graphModel.portData<QString>(nodeId, portType, portIndex, AgxPortRole::Caption);
    }
    else {
        const auto portData = m_graphModel.portData(nodeId, portType, portIndex, AgxPortRole::DataType);

        s = portData.value<AgxNodeDataType>().name;
    }
    const QRectF rect = m_fontMetrics.boundingRect(s);
    return rect;
}

QRectF AgxNodeGeometry::titleCaptionRect(const QString& string) const
{
    return m_boldFontMetrics.boundingRect(string);
}

QRectF AgxNodeGeometry::subtitleCaptionRect(const QString& string) const
{
    return m_fontMetrics.boundingRect(string);
}

QRectF AgxNodeGeometry::portTextRect(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const
{
    QString s;
    if (m_graphModel.portData<bool>(nodeId, portType, portIndex, AgxPortRole::CaptionVisible)) {
        s = m_graphModel.portData<QString>(nodeId, portType, portIndex, AgxPortRole::Caption);
    } else {
        const auto portData = m_graphModel.portData(nodeId, portType, portIndex, AgxPortRole::DataType);

        s = portData.value<AgxNodeDataType>().name;
    }

    return m_fontMetrics.boundingRect(s);
}

unsigned int AgxNodeGeometry::maxVerticalPortsExtent(AgxNodeId const nodeId) const
{
    const unsigned int nInPorts = m_graphModel.nodeData<unsigned int>(nodeId, AgxNodeRole::InPortCount);

    const unsigned int nOutPorts = m_graphModel.nodeData<unsigned int>(nodeId, AgxNodeRole::OutPortCount);

    const unsigned int maxNumOfEntries = std::max(nInPorts, nOutPorts);
    const unsigned int step = m_portSize + m_portSpacing;

    return step * maxNumOfEntries;
}

unsigned int AgxNodeGeometry::maxPortsTextAdvance(const AgxNodeId nodeId, const AgxPortType portType) const
{
    unsigned int width = 0;

    size_t const n = m_graphModel
        .nodeData(nodeId,
                  portType == AgxPortType::Out ? AgxNodeRole::OutPortCount
                  : AgxNodeRole::InPortCount)
        .toUInt();

    for (AgxPortIndex portIndex = 0ul; portIndex < n; ++portIndex)
    {
        QString name;

        if (m_graphModel.portData<bool>(nodeId, portType, portIndex, AgxPortRole::CaptionVisible)) {
            name = m_graphModel.portData<QString>(nodeId, portType, portIndex, AgxPortRole::Caption);
        } else {
            const auto [portId, portName] = m_graphModel.portData<AgxNodeDataType>(nodeId,
                                                                                   portType,
                                                                                   portIndex,
                                                                                   AgxPortRole::DataType);

            name = portName;
        }

        width = std::max(static_cast<unsigned>(m_fontMetrics.horizontalAdvance(name)), width);
    }

    return width;
}

QRectF AgxNodeGeometry::boundingRect(AgxNodeId const nodeId) const
{
    const QSize s = size(nodeId);

    const qreal marginSize = 2.0 * m_portSpacing;
    const QMargins margins(marginSize, marginSize, marginSize, marginSize);

    const QRectF r(QPointF(0, 0), s);

    return r.marginsAdded(margins);
}

QRectF AgxNodeGeometry::collapsedBoundingRect(const AgxNodeId& nodeId) const
{
    const QSize s = collapsedSize(nodeId);

    const qreal marginSize = 2.0 * m_portSpacing;
    const QMargins margins(marginSize, marginSize, marginSize, marginSize);

    const QRectF r(QPointF(0, 0), s);

    return r.marginsAdded(margins);
}

QSize AgxNodeGeometry::size(AgxNodeId const nodeId) const
{
    return m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);
}

void AgxNodeGeometry::recomputeSize(AgxNodeId const nodeId) const
{
    unsigned int height = maxVerticalPortsExtent(nodeId);

    if (const auto w = m_graphModel.nodeData<QWidget*>(nodeId, AgxNodeRole::Widget)) {
        height = std::max(height, static_cast<unsigned int>(w->height()));
    }

    QRectF const capRect = captionRect(nodeId);

    height += capRect.height();

    height += m_portSpacing; // space above caption
    height += m_portSpacing; // space below caption

    const unsigned int inPortWidth = maxPortsTextAdvance(nodeId, AgxPortType::In);
    const unsigned int outPortWidth = maxPortsTextAdvance(nodeId, AgxPortType::Out);

    unsigned int width = inPortWidth + outPortWidth + 4 * m_portSpacing;

    if (const auto w = m_graphModel.nodeData<QWidget*>(nodeId, AgxNodeRole::Widget)) {
        width += w->width();
    }

    width = std::max(width, static_cast<unsigned int>(capRect.width()) + 2 * m_portSpacing);

    const QSize size(width, height);

    m_graphModel.setNodeData(nodeId, AgxNodeRole::Size, size);
}

QSize AgxNodeGeometry::collapsedSize(AgxNodeId const nodeId) const {

    unsigned int height = 0;

    const QString title = ShortenString(m_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString());
    QRectF const capRect = titleCaptionRect(title);

    height += capRect.height();

    height += m_portSpacing; // space above caption
    height += m_portSpacing; // space below caption

    unsigned int width = 6 * m_portSpacing;

    width = std::max(width, static_cast<unsigned int>(capRect.width()) + 3 * m_portSpacing);

    return QSize(width, height);
}

QPointF AgxNodeGeometry::collapsedPortPosition(const AgxNodeId& nodeId, const AgxPortType& portType) const
{
    double totalHeight = 0.0;

    totalHeight += captionRect(nodeId).height()/2;
    totalHeight += m_portSpacing;

    const QSize size = collapsedSize(nodeId);
    QPointF result;

    switch (portType) {
        case AgxPortType::In:
        {
            constexpr double x = 0.0;

            result = QPointF(x, totalHeight);
            break;
        }

        case AgxPortType::Out:
        {
            const double x = size.width();

            result = QPointF(x, totalHeight);
            break;
        }

        default:
            break;
    }

    return result;
}

QPointF AgxNodeGeometry::portPosition(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const
{
    // ReSharper disable once CppTooWideScope
    const bool collapsed = m_graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState);

    if (collapsed)
        return collapsedPortPosition(nodeId, portType);

    unsigned int const step = m_portSize + m_portSpacing;

    QPointF result;

    double totalHeight = 0.0;

    totalHeight += captionRect(nodeId).height();
    totalHeight += m_portSpacing;

    totalHeight += step * portIndex;
    totalHeight += step / 2.0;


    const auto size = m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    switch (portType) {
        case AgxPortType::In:
        {
            constexpr double x = 0.0;

            result = QPointF(x, totalHeight);
            break;
        }

        case AgxPortType::Out:
        {
            const double x = size.width();

            result = QPointF(x, totalHeight);
            break;
        }

        default:
            break;
    }

    return result;
}

QPointF AgxNodeGeometry::portScenePosition(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const index, QTransform const& t) const
{
    const QPointF result = portPosition(nodeId, portType, index);
    
    return t.map(result);
}

QPointF AgxNodeGeometry::portTextPosition(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const
{
    QPointF p = portPosition(nodeId, portType, portIndex);

    const QRectF rect = portTextRect(nodeId, portType, portIndex);

    p.setY(p.y() + rect.height() / 4.0);

    const auto size = m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    switch (portType) {
        case AgxPortType::In:
            p.setX(m_portSpacing);
            break;

        case AgxPortType::Out:
            p.setX(size.width() - m_portSpacing - rect.width());
            break;

        default:
            break;
    }

    return p;
}

QPointF AgxNodeGeometry::captionPosition(AgxNodeId const nodeId) const
{
    const auto size = m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);
    return QPointF(0.5 * (size.width() - captionRect(nodeId).width()),
                   0.5 * m_portSpacing + captionRect(nodeId).height());
}

QPair<QPointF, QPointF> AgxNodeGeometry::dualCaptionPosition(const AgxNodeId& nodeId, const QString& title, const QString& subtitle, const bool collapsed) const
{
    const QSize size = collapsed ? collapsedSize(nodeId) : m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    const auto tRect = titleCaptionRect(title);
    const auto stRect = subtitleCaptionRect(subtitle);


    QPointF titlePos(0.5 * (size.width() - tRect.width()),
                     0.5 * m_portSpacing + tRect.height());

    QPointF subtitlePos(0.5 * (size.width() - stRect.width()),
                        0.5 * m_portSpacing + tRect.height()+stRect.height());

    return { titlePos, subtitlePos };
}

QPair<QPointF, QPointF> AgxNodeGeometry::dualCaptionPosition(const AgxNodeId& nodeId) const
{
    const bool collapsed = m_graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState);
    const QString title = collapsed ? ShortenString(m_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString()) : m_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString();
    const QString subtitle = m_graphModel.nodeData<QString>(nodeId, AgxNodeRole::SubCaption);

    return dualCaptionPosition(nodeId, title, subtitle, collapsed);
}

QRectF AgxNodeGeometry::captionRect(AgxNodeId const nodeId) const
{
    if (!m_graphModel.nodeData<bool>(nodeId, AgxNodeRole::CaptionVisible))
        return QRectF();

    const bool collapsed = m_graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState);
    const QString title = collapsed ? ShortenString(m_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString()) : m_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString();

    const QRectF nameRect = m_boldFontMetrics.boundingRect(title);

    if (!collapsed) {

        // ReSharper disable once CppTooWideScopeInitStatement
        const QString subName = ShortenString(m_graphModel.nodeData<QString>(nodeId, AgxNodeRole::SubCaption),30);

        if (!subName.isEmpty())
        {
            QRectF output = m_fontMetrics.boundingRect(subName);
            output.setHeight(output.height() + nameRect.height());
            output.setWidth(qMax(output.width(), nameRect.width()));
            return output;
        }
    }
    return nameRect;
}

QPointF AgxNodeGeometry::widgetPosition(AgxNodeId const nodeId) const
{
    const QSize size = m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    const unsigned int captionHeight = captionRect(nodeId).height();

    if (const auto w = m_graphModel.nodeData<QWidget*>(nodeId, AgxNodeRole::Widget))
    {
        // If the widget wants to use as much vertical space as possible,
        // place it immediately after the caption.
        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag)
        {
            return QPointF(2.0 * m_portSpacing + maxPortsTextAdvance(nodeId, AgxPortType::In),
                           m_portSpacing + captionHeight);
        }

        return QPointF(2.0 * m_portSpacing + maxPortsTextAdvance(nodeId, AgxPortType::In),
                           (captionHeight + size.height() - w->height()) / 2.0);
    }
    return QPointF();
}

QRect AgxNodeGeometry::resizeHandleRect(AgxNodeId const nodeId) const
{
    const QSize size = m_graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    constexpr unsigned int rectSize = 7;

    return QRect(size.width() - m_portSpacing, size.height() - m_portSpacing, rectSize, rectSize);
}

AgxPortIndex AgxNodeGeometry::checkPortHit(AgxNodeId const nodeId, AgxPortType const portType, QPointF const nodePoint) const
{

    auto const& nodeStyle = AgxPalette::GetInstance().nodePalette();

    AgxPortIndex result = InvalidPortIndex;

    if (m_graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState))
        return result;

    if (portType == AgxPortType::None)
        return result;

    double const tolerance = 2.0 * nodeStyle.ConnectionPointDiameter;

    size_t const n = m_graphModel.nodeData<unsigned int>(nodeId,
                                                        portType == AgxPortType::Out
                                                        ? AgxNodeRole::OutPortCount
                                                        : AgxNodeRole::InPortCount);

    for (unsigned int portIndex = 0; portIndex < n; ++portIndex)
    {
        auto pp = portPosition(nodeId, portType, portIndex);

        QPointF p = pp - nodePoint;

        // ReSharper disable once CppTooWideScopeInitStatement
        const auto distance = std::sqrt(QPointF::dotProduct(p, p));

        if (distance < tolerance)
        {
            result = portIndex;
            break;
        }
    }

    return result;
}