#ifndef NOTE_H
#define NOTE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

class Note : public QGraphicsItem
{
public://クラスメソッド、フィールド
    enum {
        Type = UserType + 1
    };
    static const int NOTE_WIDTH = 77;
    static const int NOTE_HEIGHT = 11;
    static const int BEAT_HEIGHT = 44;
    static bool compare(const Note* d1, const Note* d2);
public://インスタンスメソッド
    Note(const Note &obj){}
    Note(const QRectF &rect);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;
    void changeStar();
    void calculateInfo(int bpm);
    void setLongNotes(qreal p);
    bool operator<(const Note* &other) const;

public://アクセサメソッド
    qreal startTime() const;
    void setStartTime(qreal value);
    qreal endTime() const;
    void setEndTime(qreal value);
    bool isStar() const;
    void setIsStar(bool value);
    bool isLongNotes() const;
    void setIsLongNotes(bool value);
    int laneNum() const;
    void setLaneNum(int value);
private:
    QRectF _rect;
    bool _isStar = false;//星付きかどうか
    qreal _startTime;//ノートの開始時間
    qreal _endTime;//ノートの終端時間
    int _laneNum = 0;//このノートのレーン番号
    bool _isLongNotes = false;
    bool _isDrag = false;
};

#endif // NOTE_H
