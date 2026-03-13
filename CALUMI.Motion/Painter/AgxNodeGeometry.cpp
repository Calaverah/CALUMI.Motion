#include "stdafx.h"
#include "AgxNodeGeometry.h"
#pragma warning(push,0)
#include "Utilities/AgxDefinitions.h"
#include "Models/AgxGraphModel.h"
#include "AgxStyleCollection.h"
#pragma warning(pop)

AgxNodeGeometry::AgxNodeGeometry(AgxGraphModel& graphModel) : _portSize(20), _portSpacing(10), _fontMetrics(QFont()), _boldFontMetrics(QFont()), _graphModel(graphModel)
{
    QFont f;
    f.setBold(true);
    _boldFontMetrics = QFontMetrics(f);

    _portSize = _fontMetrics.height();
}

QRect AgxNodeGeometry::groupIconPosition(AgxNodeId const nodeId) const
{
	QSize size = _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);
	unsigned int rectSize = 7;
	return QRect(size.width() - _portSpacing, _portSpacing, rectSize, rectSize);
}

QRectF AgxNodeGeometry::agxPortTextRect(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const
{
    QString s;
    if (_graphModel.portData<bool>(nodeId, portType, portIndex, AgxPortRole::CaptionVisible)) {
        s = _graphModel.portData<QString>(nodeId, portType, portIndex, AgxPortRole::Caption);
    }
    else {
        auto portData = _graphModel.portData(nodeId, portType, portIndex, AgxPortRole::DataType);

        s = portData.value<AgxNodeDataType>().name;
    }
    QRectF rect = _fontMetrics.boundingRect(s);
    return rect;
}

QRectF AgxNodeGeometry::titleCaptionRect(const QString& string) const
{
    return _boldFontMetrics.boundingRect(string);
}

QRectF AgxNodeGeometry::subtitleCaptionRect(const QString& string) const
{
    return _fontMetrics.boundingRect(string);
}

QRectF AgxNodeGeometry::portTextRect(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const
{
    QString s;
    if (_graphModel.portData<bool>(nodeId, portType, portIndex, AgxPortRole::CaptionVisible)) {
        s = _graphModel.portData<QString>(nodeId, portType, portIndex, AgxPortRole::Caption);
    } else {
        auto portData = _graphModel.portData(nodeId, portType, portIndex, AgxPortRole::DataType);

        s = portData.value<AgxNodeDataType>().name;
    }

    return _fontMetrics.boundingRect(s);
}

unsigned int AgxNodeGeometry::maxVerticalPortsExtent(AgxNodeId const nodeId) const
{
    unsigned int nInPorts = _graphModel.nodeData<unsigned int>(nodeId, AgxNodeRole::InPortCount);

    unsigned int nOutPorts = _graphModel.nodeData<unsigned int>(nodeId, AgxNodeRole::OutPortCount);

    unsigned int maxNumOfEntries = std::max(nInPorts, nOutPorts);
    unsigned int step = _portSize + _portSpacing;

    return step * maxNumOfEntries;
}

unsigned int AgxNodeGeometry::maxPortsTextAdvance(AgxNodeId const nodeId, AgxPortType const portType) const
{
    unsigned int width = 0;

    size_t const n = _graphModel
        .nodeData(nodeId,
                  (portType == AgxPortType::Out) ? AgxNodeRole::OutPortCount
                  : AgxNodeRole::InPortCount)
        .toUInt();

    for (AgxPortIndex portIndex = 0ul; portIndex < n; ++portIndex) {
        QString name;

        if (_graphModel.portData<bool>(nodeId, portType, portIndex, AgxPortRole::CaptionVisible)) {
            name = _graphModel.portData<QString>(nodeId, portType, portIndex, AgxPortRole::Caption);
        } else {
            AgxNodeDataType portData = _graphModel.portData<AgxNodeDataType>(nodeId,
                                                                       portType,
                                                                       portIndex,
                                                                       AgxPortRole::DataType);

            name = portData.name;
        }

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        width = std::max(unsigned(_fontMetrics.horizontalAdvance(name)), width);
#else
        width = std::max(unsigned(_fontMetrics.width(name)), width);
#endif
    }

    return width;
}

QRectF AgxNodeGeometry::boundingRect(AgxNodeId const nodeId) const
{
    QSize s = size(nodeId);

    qreal marginSize = 2.0 * _portSpacing;
    QMargins margins(marginSize, marginSize, marginSize, marginSize);

    QRectF r(QPointF(0, 0), s);

    return r.marginsAdded(margins);
}

QRectF AgxNodeGeometry::collapsedBoundingRect(const AgxNodeId& nodeId) const
{
    QSize s = collapsedSize(nodeId);

    qreal marginSize = 2.0 * _portSpacing;
    QMargins margins(marginSize, marginSize, marginSize, marginSize);

    QRectF r(QPointF(0, 0), s);

    return r.marginsAdded(margins);
}

QSize AgxNodeGeometry::size(AgxNodeId const nodeId) const
{
    return _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);
}

void AgxNodeGeometry::recomputeSize(AgxNodeId const nodeId) const
{
    unsigned int height = maxVerticalPortsExtent(nodeId);

    if (auto w = _graphModel.nodeData<QWidget*>(nodeId, AgxNodeRole::Widget)) {
        height = std::max(height, static_cast<unsigned int>(w->height()));
    }

    QRectF const capRect = captionRect(nodeId);

    height += capRect.height();

    height += _portSpacing; // space above caption
    height += _portSpacing; // space below caption

    unsigned int inPortWidth = maxPortsTextAdvance(nodeId, AgxPortType::In);
    unsigned int outPortWidth = maxPortsTextAdvance(nodeId, AgxPortType::Out);

    unsigned int width = inPortWidth + outPortWidth + 4 * _portSpacing;

    if (auto w = _graphModel.nodeData<QWidget*>(nodeId, AgxNodeRole::Widget)) {
        width += w->width();
    }

    width = std::max(width, static_cast<unsigned int>(capRect.width()) + 2 * _portSpacing);

    QSize size(width, height);

    _graphModel.setNodeData(nodeId, AgxNodeRole::Size, size);
}

QSize AgxNodeGeometry::collapsedSize(AgxNodeId const nodeId) const {

    unsigned int height = 0;// _portSize + _portSpasing;

    QString title = ShortenString(_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString());
    QRectF const capRect = titleCaptionRect(title);

    height += capRect.height();

    height += _portSpacing; // space above caption
    height += _portSpacing; // space below caption

    unsigned int width = 6 * _portSpacing;

    width = std::max(width, static_cast<unsigned int>(capRect.width()) + 3 * _portSpacing);

    return QSize(width, height);
}

QPointF AgxNodeGeometry::collapsedPortPosition(const AgxNodeId& nodeId, const AgxPortType& portType) const
{
    double totalHeight = 0.0;

    totalHeight += captionRect(nodeId).height()/2;
    totalHeight += _portSpacing;

    QSize size = collapsedSize(nodeId);
    QPointF result;

    switch (portType) {
        case AgxPortType::In:
        {
            double x = 0.0;

            result = QPointF(x, totalHeight);
            break;
        }

        case AgxPortType::Out:
        {
            double x = size.width();

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
    bool collapsed = _graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState);

    if (collapsed) return collapsedPortPosition(nodeId, portType);

    unsigned int const step = _portSize + _portSpacing;

    QPointF result;

    double totalHeight = 0.0;

    totalHeight += captionRect(nodeId).height();
    totalHeight += _portSpacing;

    totalHeight += step * portIndex;
    totalHeight += step / 2.0;


    QSize size = _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    switch (portType) {
        case AgxPortType::In:
        {
            double x = 0.0;

            result = QPointF(x, totalHeight);
            break;
        }

        case AgxPortType::Out:
        {
            double x = size.width();

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
    QPointF result = portPosition(nodeId, portType, index);
    
    return t.map(result);
}

QPointF AgxNodeGeometry::portTextPosition(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const portIndex) const
{
    QPointF p = portPosition(nodeId, portType, portIndex);

    QRectF rect = portTextRect(nodeId, portType, portIndex);

    p.setY(p.y() + rect.height() / 4.0);

    QSize size = _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    switch (portType) {
        case AgxPortType::In:
            p.setX(_portSpacing);
            break;

        case AgxPortType::Out:
            p.setX(size.width() - _portSpacing - rect.width());
            break;

        default:
            break;
    }

    return p;
}

QPointF AgxNodeGeometry::captionPosition(AgxNodeId const nodeId) const
{
    QSize size = _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);
    return QPointF(0.5 * (size.width() - captionRect(nodeId).width()),
                   0.5 * _portSpacing + captionRect(nodeId).height());
}

QPair<QPointF, QPointF> AgxNodeGeometry::dualCaptionPosition(const AgxNodeId& nodeId, const QString& title, const QString& subtitle, bool collapsed) const
{
    QSize size = collapsed ? collapsedSize(nodeId) : _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    auto tRect = titleCaptionRect(title);
    auto stRect = subtitleCaptionRect(subtitle);


    QPointF titlePos(0.5 * (size.width() - tRect.width()),
                     0.5 * _portSpacing + tRect.height());

    QPointF subtitlePos(0.5 * (size.width() - stRect.width()),
                        0.5 * _portSpacing + tRect.height()+stRect.height());

    return { titlePos, subtitlePos };
}

QPair<QPointF, QPointF> AgxNodeGeometry::dualCaptionPosition(const AgxNodeId& nodeId) const
{
    bool collapsed = _graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState);
    QString title = collapsed ? ShortenString(_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString()) : _graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString();
    QString subtitle = _graphModel.nodeData<QString>(nodeId, AgxNodeRole::SubCaption);

    return dualCaptionPosition(nodeId, title, subtitle, collapsed);
}

QRectF AgxNodeGeometry::captionRect(AgxNodeId const nodeId) const
{
    if (!_graphModel.nodeData<bool>(nodeId, AgxNodeRole::CaptionVisible))
        return QRectF();

    bool collapsed = _graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState);
    QString title = collapsed ? ShortenString(_graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString()) : _graphModel.nodeData(nodeId, AgxNodeRole::Caption).toString();

    QRectF nameRect = _boldFontMetrics.boundingRect(title);

    if (!collapsed) {

        QString subName = ShortenString(_graphModel.nodeData<QString>(nodeId, AgxNodeRole::SubCaption),30);
        if (!subName.isEmpty()) {

            QRectF output = _fontMetrics.boundingRect(subName);
            output.setHeight(output.height() + nameRect.height());
            output.setWidth(qMax(output.width(), nameRect.width()));
            return output;
        }
    }
    return nameRect;
}

QPointF AgxNodeGeometry::widgetPosition(AgxNodeId const nodeId) const
{
    QSize size = _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    unsigned int captionHeight = captionRect(nodeId).height();

    if (auto w = _graphModel.nodeData<QWidget*>(nodeId, AgxNodeRole::Widget)) {
        // If the widget wants to use as much vertical space as possible,
        // place it immediately after the caption.
        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag) {
            return QPointF(2.0 * _portSpacing + maxPortsTextAdvance(nodeId, AgxPortType::In),
                           _portSpacing + captionHeight);
        } else {
            return QPointF(2.0 * _portSpacing + maxPortsTextAdvance(nodeId, AgxPortType::In),
                           (captionHeight + size.height() - w->height()) / 2.0);
        }
    }
    return QPointF();
}

QRect AgxNodeGeometry::resizeHandleRect(AgxNodeId const nodeId) const
{
    QSize size = _graphModel.nodeData<QSize>(nodeId, AgxNodeRole::Size);

    unsigned int rectSize = 7;

    return QRect(size.width() - _portSpacing, size.height() - _portSpacing, rectSize, rectSize);
}

AgxPortIndex AgxNodeGeometry::checkPortHit(AgxNodeId const nodeId, AgxPortType const portType, QPointF const nodePoint) const
{

    auto const& nodeStyle = AgxStyleCollection::nodeStyle();

    AgxPortIndex result = InvalidPortIndex;

    if (_graphModel.nodeData<bool>(nodeId, AgxNodeRole::CollapseState)) return result;

    if (portType == AgxPortType::None)
        return result;

    double const tolerance = 2.0 * nodeStyle.ConnectionPointDiameter;

    size_t const n = _graphModel.nodeData<unsigned int>(nodeId,
                                                        (portType == AgxPortType::Out)
                                                        ? AgxNodeRole::OutPortCount
                                                        : AgxNodeRole::InPortCount);

    for (unsigned int portIndex = 0; portIndex < n; ++portIndex) {
        auto pp = portPosition(nodeId, portType, portIndex);

        QPointF p = pp - nodePoint;
        auto distance = std::sqrt(QPointF::dotProduct(p, p));

        if (distance < tolerance) {
            result = portIndex;
            break;
        }
    }

    return result;
}