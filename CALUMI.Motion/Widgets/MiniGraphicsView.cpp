#include "stdafx.h"
#include "MiniGraphicsView.h"
#include <QDebug>
#include <QTimer>
#include "Painter/AgxStyleCollection.h"

MiniGraphicsView::MiniGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumSize(100, 100);

    MiniViewMouseEventFilter* filter = new MiniViewMouseEventFilter();
    installEventFilter(filter);
}

MiniGraphicsView::MiniGraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(scene, &QGraphicsScene::changed, this, &MiniGraphicsView::OnSceneChange);

    auto const& flowViewStyle = AgxStyleCollection::flowViewStyle();

    setBackgroundBrush(flowViewStyle.BackgroundColor);
    
    setToolTip("Double Click To Open Embedded Graph");

    setFrameStyle(QFrame::Plain);

    MiniViewMouseEventFilter* filter = new MiniViewMouseEventFilter();
    installEventFilter(filter);
}

void MiniGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
    {
        event->ignore();
        return;
    }
    if (event->button() == Qt::LeftButton) {
        mousePressPos = event->pos();
        event->accept();
    }
}

void MiniGraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
    {
        event->ignore();
        return;
    }
    if (event->button() == Qt::LeftButton && event->pos() == mousePressPos) {

        QPointF sceneCoords = mapToScene(event->pos());
        Q_EMIT clicked(sceneCoords);
    }
    event->accept();
}

void MiniGraphicsView::OnSceneChange()
{
    if (scene()) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

void MiniGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    event->ignore();
}

void MiniGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    event->ignore();
}

void MiniGraphicsView::wheelEvent(QWheelEvent* event)
{
    event->ignore();
}

void MiniGraphicsView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);

    if (scene()) {
        fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

bool MiniViewMouseEventFilter::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        event->ignore();
        return true;
        
        /*QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (mouseEvent->button() == Qt::MiddleButton || (mouseEvent->modifiers() & Qt::AltModifier) != 0)
        {
            return true;
        }*/
    }
    return QObject::eventFilter(obj, event);
}
