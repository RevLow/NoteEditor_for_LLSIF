#ifndef NOTEBACKGRID_H
#define NOTEBACKGRID_H

#include <QGraphicsItem>
#include <QPainter>

class NoteBackgrid : public QGraphicsItem
{
public:
    NoteBackgrid(QObject *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    QRectF _rect;
};

#endif // NOTEBACKGRID_H
