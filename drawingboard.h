#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTransform>

class DrawingBoard : public QGraphicsView
{
    Q_OBJECT

public:
    explicit DrawingBoard(QWidget *parent = nullptr);
    void setZoomFactor(qreal factor);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    static const int CANVAS_SIZE = 1024;  // 画布大小为1024x1024
    static const int CANVAS_CENTER = CANVAS_SIZE / 2;  // 画布中心点
    
    QGraphicsScene *scene;
    qreal zoomFactor;
    void setupScene();
    
    // 辅助方法：将相对于中心点的坐标转换为场景坐标
    QPointF centerToScene(qreal x, qreal y) const {
        return QPointF(x + CANVAS_CENTER, y + CANVAS_CENTER);
    }
    
    // 辅助方法：将场景坐标转换为相对于中心点的坐标
    QPointF sceneToCenter(const QPointF &scenePos) const {
        return QPointF(scenePos.x() - CANVAS_CENTER, scenePos.y() - CANVAS_CENTER);
    }

    // 鼠标拖动相关
    bool isDragging;
    QPoint lastMousePos;
};

#endif // DRAWINGBOARD_H 