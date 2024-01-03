#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsItem>
#include <mygraphicsitem.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //下方标签，显示scene的坐标
    QLabel *labCord;
    QPointF temp_point;

    //当前模式
    qint8 mode;

    //场景
    QGraphicsScene m_graphicsScene;
    //图元
    MyGraphicsItem *m_GraphicsItem;
    //场景中的图元项
    QVector<QGraphicsItem*> my_v;


private slots:
    //各行为槽函数
    void on_action_line_triggered();
    void on_action_rect_triggered();
    void on_action_circle_triggered();
    void on_action_back_triggered();
    void on_action_polygon_triggered();
    void on_action_freeline_triggered();

    //槽函数，由于接受mygraphicsview传出的点击和移动信号
    void on_mouseMovePoint(QPoint point);
    void on_mouseClicked(QPoint point);
    void on_mouseRelease(QPoint point);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
