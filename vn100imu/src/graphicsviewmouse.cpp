#include "graphicsviewmouse.h"
#include "tuning_ui.h"

//explicit bool mousereleased;


GraphicsviewMouse::GraphicsviewMouse(QWidget *parent) : QGraphicsView(parent)
{

}

GraphicsviewMouse::~GraphicsviewMouse()
{

}

/*void GraphicsviewMouse::mouseDoubleClickEvent(QMouseEvent *ev)
{

}
void GraphicsviewMouse::mouseMoveEvent(QMouseEvent *ev)
{

}*/
void GraphicsviewMouse::mousePressEvent(QMouseEvent *ev)
{
    QPoint pt = QPoint(ev->x(),ev->y());
    emit Mouse_Pressed(pt);
}
void GraphicsviewMouse::mouseReleaseEvent(QMouseEvent *ev)
{
    emit Mouse_Released();
}

