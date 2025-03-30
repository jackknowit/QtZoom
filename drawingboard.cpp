#include "drawingboard.h"
#include <QPainter>
#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QScrollBar>
#include <QDebug>
#include <QMouseEvent>

DrawingBoard::DrawingBoard(QWidget *parent)
    : QGraphicsView(parent)
    , zoomFactor(1.0)
    , isDragging(false)
{
    // 创建场景并设置固定大小
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, CANVAS_SIZE, CANVAS_SIZE);
    setScene(scene);
    
    // 设置视图属性
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
    
    // 确保可以接收鼠标事件
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    
    // 初始化场景
    setupScene();
    
    // 居中显示
    centerOn(CANVAS_CENTER, CANVAS_CENTER);
}

void DrawingBoard::setupScene()
{
    // 绘制坐标轴
    QPen axisPen(Qt::gray, 1, Qt::DashLine);
    scene->addLine(0, CANVAS_CENTER, CANVAS_SIZE, CANVAS_CENTER, axisPen);  // X轴
    scene->addLine(CANVAS_CENTER, 0, CANVAS_CENTER, CANVAS_SIZE, axisPen);  // Y轴
    
    // 以下所有坐标都是相对于中心点的坐标
    
    // 绘制圆形 (-100, -100, 直径200)
    QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(
        centerToScene(-100, -100).x(),
        centerToScene(-100, -100).y(),
        200, 200);
    circle->setPen(QPen(Qt::blue, 2));
    circle->setBrush(Qt::NoBrush);
    scene->addItem(circle);
    
    // 绘制矩形 (50, -75, 宽200, 高150)
    QGraphicsRectItem *rectangle = new QGraphicsRectItem(
        centerToScene(50, -75).x(),
        centerToScene(50, -75).y(),
        200, 150);
    rectangle->setPen(QPen(Qt::red, 2));
    rectangle->setBrush(Qt::NoBrush);
    scene->addItem(rectangle);
    
    // 绘制三角形
    QPolygonF triangle;
    triangle << centerToScene(-200, 50)
            << centerToScene(-100, 200)
            << centerToScene(-300, 200);
    QGraphicsPolygonItem *triangleItem = new QGraphicsPolygonItem(triangle);
    triangleItem->setPen(QPen(Qt::green, 2));
    triangleItem->setBrush(Qt::NoBrush);
    scene->addItem(triangleItem);
    
    // 绘制梯形
    QPolygonF trapezoid;
    trapezoid << centerToScene(100, 50)
              << centerToScene(300, 50)
              << centerToScene(350, 200)
              << centerToScene(50, 200);
    QGraphicsPolygonItem *trapezoidItem = new QGraphicsPolygonItem(trapezoid);
    trapezoidItem->setPen(QPen(QColor(128, 0, 128), 2));
    trapezoidItem->setBrush(Qt::NoBrush);
    scene->addItem(trapezoidItem);
}

void DrawingBoard::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        // 获取鼠标位置在场景中的坐标
        const QPointF p0scene = mapToScene(event->position().toPoint());

        // 计算缩放因子
        double angle = event->angleDelta().y();
        double factor = qPow(1.0015, angle);

        // 限制缩放范围
        double newZoom = transform().m11() * factor;
        if (newZoom < 0.5 || newZoom > 10.0) {
            return;
        }

        // 设置无锚点
        setTransformationAnchor(QGraphicsView::NoAnchor);

        // 应用缩放
        scale(factor, factor);

        // 计算新的视图坐标
        const QPointF p1mouse = mapFromScene(p0scene);
        const QPointF move = p1mouse - event->position();

        // 调整滚动条位置以保持鼠标位置不变
        horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
        verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());

        event->accept();
        return;
    }

    QGraphicsView::wheelEvent(event);
}

void DrawingBoard::drawBackground(QPainter *painter, const QRectF &rect)
{
    // 绘制灰色背景
    painter->fillRect(rect, QColor(200, 200, 200));
    
    // 绘制白色画布
    painter->fillRect(0, 0, CANVAS_SIZE, CANVAS_SIZE, Qt::white);
}

void DrawingBoard::setZoomFactor(qreal factor)
{
    qreal scaleFactor = factor / zoomFactor;
    scale(scaleFactor, scaleFactor);
    zoomFactor = factor;
}

void DrawingBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        QPoint delta = event->pos() - lastMousePos;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        lastMousePos = event->pos();
        event->accept();
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
} 