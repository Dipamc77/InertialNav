#ifndef GRAPHICSVIEWMOUSE_H
#define GRAPHICSVIEWMOUSE_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class GraphicsviewMouse :public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsviewMouse(QWidget *parent = 0);
    ~GraphicsviewMouse();

   // void mouseDoubleClickEvent(QMouseEvent *ev);
  //  void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void Mouse_Pressed(QPoint);
    void Mouse_Released();
};

#endif // GRAPHICSVIEWMOUSE_H
