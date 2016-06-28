#include "notebackgrid.h"

NoteBackgrid::NoteBackgrid(QObject *parent) :  _rect(QRectF(0,0,1,1))
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

QRectF NoteBackgrid::boundingRect() const
{
    return _rect;
}

void NoteBackgrid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(Qt::red, 0.01));

    for(int note=1;note < 10;note++)
    {
        qreal xPos = static_cast<double>(note) / 9.0;
        QLineF *line = new QLineF(xPos, 0, xPos, 1);
        painter->drawLine(*line);
    }
}
