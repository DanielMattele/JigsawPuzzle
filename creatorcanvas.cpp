#include "creatorcanvas.h"

CreatorCanvas::CreatorCanvas(QWidget *parent)
    : QLabel{parent}
{
    setFixedSize(300, 300);
}

CreatorCanvas::~CreatorCanvas()
{
    
}


void CreatorCanvas::mousePressEvent(QMouseEvent *event)
{
    emit clicked(event->pos());
}

void CreatorCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    emit released();
}

void CreatorCanvas::mouseMoveEvent(QMouseEvent *event)
{
    emit dragged(event->pos());
}
