#pragma once
#pragma warning(push,0)
#include <QGraphicsView>
#include <QMouseEvent>
#pragma warning(pop)

class MiniViewMouseEventFilter : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};


class MiniGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MiniGraphicsView(QWidget* parent = nullptr);
    MiniGraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);

signals:
    void clicked(QPointF scenePos);

public slots:
    void OnSceneChange();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    QPoint mousePressPos;
};