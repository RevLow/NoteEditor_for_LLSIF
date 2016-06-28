#include "notegraphicsview.h"
#include "noteeditscene.h"
#include "QScrollBar"
#include "note.h"
NoteGraphicsView::NoteGraphicsView(QWidget *parent) : QGraphicsView(parent), currentTime(0)
{

}

void NoteGraphicsView::changeBpmCount(QString value)
{
    qDebug("CALLED FROM NoteGraphicsView");
    bpm = value.toLong();
    dynamic_cast<NoteEditScene*>(scene())->changeBpmCount(value);

}

void NoteGraphicsView::initCurrentTime(qint64 millisec)
{
    currentTime = millisec;
    qDebug("CurrentTime: %ld", currentTime);
}

