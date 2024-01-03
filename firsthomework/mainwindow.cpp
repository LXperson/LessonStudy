#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&m_graphicsScene);
    ui->graphicsView->setSceneRect(0,0,ui->graphicsView->frameSize().width(),ui->graphicsView->frameSize().height());

    //相关参数设置
    ui->graphicsView->setBackgroundBrush(QColor(255, 255, 255));
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsScene.setBackgroundBrush(Qt::darkGray);

    //下方坐标
    labCord=new QLabel("坐标：");
    labCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labCord);

    ui->graphicsView->setCursor(Qt::CrossCursor);
    ui->graphicsView->setMouseTracking(true);

    mode=MODE_SATRT;
    m_GraphicsItem=new MyGraphicsItem(true,mode);

    //将信号与槽函数连接起来
    connect(ui->graphicsView,SIGNAL(mouseMove(QPoint)),this,SLOT(on_mouseMovePoint(QPoint)));
    connect(ui->graphicsView,SIGNAL(mouseLeftPress(QPoint)),this,SLOT(on_mouseClicked(QPoint)));
    connect(ui->graphicsView,SIGNAL(mouseLeftRelease(QPoint)),this,SLOT(on_mouseRelease(QPoint)));
    connect(ui->graphicsView,SIGNAL(mouseRightPress(QPoint)),this,SLOT(on_mouseClicked(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//直线
void MainWindow::on_action_line_triggered()
{
    mode=MODE_LINE;
    m_GraphicsItem->setMode(mode);
}

//矩形
void MainWindow::on_action_rect_triggered()
{
    mode=MODE_RECT;
    m_GraphicsItem->setMode(mode);
}

//椭圆
void MainWindow::on_action_circle_triggered()
{
    mode=MODE_ELLIPSE;
    m_GraphicsItem->setMode(mode);
}

//多边形
void MainWindow::on_action_polygon_triggered()
{
    mode=MODE_POLYGON;
    m_GraphicsItem->setMode(mode);
}

//自由线条
void MainWindow::on_action_freeline_triggered()
{
    mode=MODE_FREELINE;
    m_GraphicsItem->setMode(mode);
}

//后退
void MainWindow::on_action_back_triggered()
{
    if(!my_v.isEmpty()){
        m_graphicsScene.removeItem(my_v[my_v.size()-1]);
        my_v.pop_back();
        ui->graphicsView->viewport()->update();
    }
}
//鼠标移动槽函数
void MainWindow::on_mouseMovePoint(QPoint point)
{
    //坐标转换
    temp_point=ui->graphicsView->mapToScene(point);
    labCord->setText(QString::asprintf("坐标：%.0f,%.0f",temp_point.x(),temp_point.y()));

    //绘画未完成，则进入并加点
    if(mode!=MODE_SATRT&&!m_GraphicsItem->getDrawingState()){
        if(mode==MODE_FREELINE)m_GraphicsItem->addPoint(temp_point);
        else m_GraphicsItem->setLastPoint(temp_point);
        ui->graphicsView->viewport()->update();
    }
}
//鼠标点击槽函数
void MainWindow::on_mouseClicked(QPoint point)
{
    //坐标转换，加点，设置状态为false，表示绘画未完成
    if(mode!=MODE_SATRT){
        temp_point=ui->graphicsView->mapToScene(point);
        m_GraphicsItem->addPoint(temp_point);
        m_GraphicsItem->setLastPoint(temp_point);
        m_GraphicsItem->setDrawingState(false);
        m_graphicsScene.addItem(m_GraphicsItem);
    }
}
//鼠标释放槽函数
void MainWindow::on_mouseRelease(QPoint point)
{
    if(mode!=MODE_SATRT){
        //坐标转换，加点，设置状态为true，表示绘画完成
        temp_point=ui->graphicsView->mapToScene(point);
        m_GraphicsItem->setDrawingState(true);
        m_GraphicsItem->addPoint(temp_point);

        QPolygonF allPoint=m_GraphicsItem->getAllPoint();
        switch(mode){
        case MODE_LINE:{
            if(allPoint.size()==2){
                QGraphicsLineItem *line=new QGraphicsLineItem(allPoint[0].x(),allPoint[0].y(),allPoint[1].x(),allPoint[1].y());
                m_graphicsScene.addItem(line);
                my_v.push_back(line);
            }
            break;
        }
        case MODE_RECT:{
            if(allPoint.size()==2){
                QRectF area=m_GraphicsItem->boundingRect();
                QGraphicsRectItem *rect=new QGraphicsRectItem(area);
                m_graphicsScene.addItem(rect);
                my_v.push_back(rect);
            }
            break;
        }
        case MODE_ELLIPSE:{
            if(allPoint.size()==2){
                QRectF area=m_GraphicsItem->boundingRect();
                QGraphicsEllipseItem *ellipse=new QGraphicsEllipseItem(area);
                m_graphicsScene.addItem(ellipse);
                my_v.push_back(ellipse);
            }
            break;
        }
        case MODE_POLYGON:{
            if(allPoint.size()>2){
                QGraphicsPolygonItem *polygon=new QGraphicsPolygonItem(allPoint);
                m_graphicsScene.addItem(polygon);
                my_v.push_back(polygon);
            }
            break;
        }
        case MODE_FREELINE:{
            if(allPoint.size()>=2){
                QPainterPath ppath(allPoint[0]);
                for(int i=1;i<allPoint.size();i++)ppath.lineTo(allPoint[i]);
                QGraphicsPathItem *path=new QGraphicsPathItem(ppath);
                m_graphicsScene.addItem(path);
                my_v.push_back(path);
            }
            break;
        }
        default:
            break;
        }
        //将临时项移出场景
        m_graphicsScene.removeItem(m_GraphicsItem);
        m_GraphicsItem->reset();
        ui->graphicsView->viewport()->update();
    }
}
