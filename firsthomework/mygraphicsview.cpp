#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        QPoint point=event->pos();
        emit mouseLeftPress(point);
    }
    else if(event->button()==Qt::RightButton)
    {
        QPoint point=event->pos();
        emit mouseRightPress(point);
    }
    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

    QPoint point=event->pos();
    emit mouseMove(point);
    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton){
        QPoint point=event->pos();
        emit mouseLeftRelease(point);
    }
    QGraphicsView::mouseReleaseEvent(event);
}
