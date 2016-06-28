#ifndef NOTEGRAPHICSVIEW_H
#define NOTEGRAPHICSVIEW_H

#include <QGraphicsView>

class NoteGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit NoteGraphicsView(QWidget *parent = 0);
public slots:
    void changeBpmCount(QString value);
    void initCurrentTime(qint64 millisec);
private:
    qint64 currentTime;
    qint64 bpm;
};

#endif // NOTEGRAPHICSVIEW_H
