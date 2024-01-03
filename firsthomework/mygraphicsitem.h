#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H

#include <QWidget>
#include <QGraphicsItem>
#include <QPainter>
#include <QRect>
#include <QPainterPath>

//模式枚举
enum MODE_PAINTER{
    MODE_SATRT=0,

    MODE_LINE,
    MODE_RECT,
    MODE_ELLIPSE,
    MODE_POLYGON,
    MODE_FREELINE,

    MODE_END=0xff,
};

class MyGraphicsItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit MyGraphicsItem(bool state=true,qint8 m_mode=MODE_SATRT,QGraphicsItem *parent = nullptr);

    //继承QGraphicsItem，须重写相应类
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;//重写形状

    void reset();                       //清除所有点
    void setDrawingState(bool state);   //设置绘画状态
    void setMode(qint8 m_mode);         //设置绘画模式
    bool getDrawingState();             //返回会话状态
    void setLastPoint(QPointF point);

    void addPoint(QPointF point);       //加点
    QPolygonF getAllPoint();            //返回所有点

private:
    QPolygonF pst;                      //点集合
    QPointF lastPoint;                  //最后一个点
    QPointF minPoint;
    QPointF maxPoint;
    bool DrawingState=true;             //绘画状态
    qint8 mode;                         //绘画模式

    //不同图形的绘画
    void m_drawLine(QPainter *painter);
    void m_drawRect(QPainter *painter);
    void m_drawEllipse(QPainter *painter);
    void m_drawPolygon(QPainter *painter);
    void m_drawFreeLine(QPainter *painter);

signals:

};

#endif // MYGRAPHICSITEM_H
