#ifndef NOTEEDITSCENE_H
#define NOTEEDITSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "note.h"
#include <QMediaPlayer>
#include <QSound>

#include <QJsonObject>

class NoteEditScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode{
        ADD,
        MOVE,
        ERASE,
        LONGNOTE
    };

    explicit NoteEditScene(QObject *parent = 0);
    //void fitToContentSize(QRectF qRect);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void changeBpmCount(QString value);

    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void saveJson(QString path);
public slots:
    //void toggleMode(bool isCheck);
    void triggerAdd();
    void triggerMove();
    void triggerErase();
    void triggerLN();
    void changeTimes(qint64 millisec);
    void openJson(QJsonObject json);
private:
    QVector<QGraphicsLineItem*> _lineVec;//横線を保持するためのベクター
    QList<Note*> noteList;
    QRectF viewRect;//このシーンを見るビューの大きさを格納する
    //void DrawTimeLineGrid();//タイムラインカーソルを作るためのメソッド
    void drawHGrid();//横の線を描画
    void drawVGrid();
    QGraphicsItem* _grabItem;//クリックした対象を保持
    QPointF m_clickPoint;
    unsigned int bpm;
    unsigned int msec;
    const int LANE_NUM = 9;
    Mode currentMode = MOVE;
    qint64 previousTime = 0;
    //QMediaPlayer *player;

};

#endif // NOTEEDITSCENE_H
