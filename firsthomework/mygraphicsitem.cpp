#include "mygraphicsitem.h"
#include <QDebug>

MyGraphicsItem::MyGraphicsItem(bool state,qint8 m_mode,QGraphicsItem *parent) :
   QGraphicsItem(parent),DrawingState(state),mode(m_mode)
{

}

void MyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::blue);
    painter->setBrush(Qt::NoBrush);
    painter->setOpacity(1.0);
    switch(mode){
    case MODE_LINE:
        m_drawLine(painter);
        break;
    case MODE_RECT:
        m_drawRect(painter);
        break;
    case MODE_ELLIPSE:
        m_drawEllipse(painter);
        break;
    case MODE_POLYGON:
        m_drawPolygon(painter);
        break;
    case MODE_FREELINE:
        m_drawFreeLine(painter);
        break;
    default:break;
    }
}

QRectF MyGraphicsItem::boundingRect() const
{
    if(mode==MODE_POLYGON||mode==MODE_FREELINE)return QRectF(minPoint,maxPoint);
    if(pst.count()>0&&pst[0]!=lastPoint)return QRectF(pst[0],lastPoint);
}

QPainterPath MyGraphicsItem::shape() const
{
    QPainterPath p;
    p.addEllipse(boundingRect());
    return p;
}

void MyGraphicsItem::addPoint(QPointF point)
{
    if(pst.contains(point))return;
    if(pst.isEmpty()||DrawingState){
        pst.push_back(point);
        minPoint=point;
        maxPoint=point;
    }
    else if(mode==MODE_POLYGON||mode==MODE_FREELINE)pst.push_back(point);
}
//画线
void MyGraphicsItem::m_drawLine(QPainter *painter)
{
    if(pst.count()>0&&pst[0]!=lastPoint){
        QLineF line(pst[0],lastPoint);
        painter->drawLine(line);
    }
}
//画矩形
void MyGraphicsItem::m_drawRect(QPainter *painter)
{
    if(pst.count()>0&&pst[0]!=lastPoint){
        QRectF rect(pst[0],lastPoint);
        painter->drawRect(rect);
    }
}
//画椭圆
void MyGraphicsItem::m_drawEllipse(QPainter *painter)
{
    if(pst.count()>0&&pst[0]!=lastPoint){
        QRectF rect(pst[0],lastPoint);
        painter->drawEllipse(rect);
    }
}
//画线多边形
void MyGraphicsItem::m_drawPolygon(QPainter *painter)
{
    if(pst.count()>0&&pst[0]!=lastPoint){
        for(int i=1;i<pst.size();i++){
            QLineF line(pst[i-1],pst[i]);
            painter->drawLine(line);
        }
        QLineF line(pst[pst.size()-1],lastPoint);
        painter->drawLine(line);
    }
}
//画自由线条
void MyGraphicsItem::m_drawFreeLine(QPainter *painter)
{
    if(pst.count()>=2){
            QPainterPath ppath;
            ppath.moveTo(pst[0]);
            for(int i=1;i<pst.size();i++)ppath.lineTo(pst[i]);
            painter->drawPath(ppath);
    }
}

void MyGraphicsItem::reset()
{
    pst.clear();
}

void MyGraphicsItem::setDrawingState(bool state)
{
    DrawingState=state;
}

void MyGraphicsItem::setMode(qint8 m_mode)
{
    mode=m_mode;
}

bool MyGraphicsItem::getDrawingState()
{
    return DrawingState;
}

void MyGraphicsItem::setLastPoint(QPointF point)
{
    minPoint.setX(qMin(minPoint.x(),point.x()));
    minPoint.setY(qMin(minPoint.y(),point.y()));
    maxPoint.setX(qMax(maxPoint.x(),point.x()));
    maxPoint.setY(qMax(maxPoint.y(),point.y()));
    lastPoint=point;
}

QPolygonF MyGraphicsItem::getAllPoint()
{
    return pst;
}
