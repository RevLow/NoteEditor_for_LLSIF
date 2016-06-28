#include "note.h"

Note::Note(const QRectF &rect) : _rect(rect), _startTime(0), _endTime(0)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemClipsToShape);
}

QRectF Note::boundingRect() const
{
    return _rect;
}

int Note::type() const
{
    return Type;
}

qreal Note::startTime() const
{
    return _startTime;
}

void Note::setStartTime(qreal value)
{
    this->_startTime = value;
}

qreal Note::endTime() const
{
   return _endTime;
}

void Note::setEndTime(qreal value)
{
    this->_endTime = value;
}


bool Note::isStar() const
{
    return _isStar;
}
void Note::setIsStar(bool value)
{
    _isStar = value;
}

bool Note::isLongNotes() const
{
    return _isLongNotes;
}
void Note::setIsLongNotes(bool value)
{
    _isLongNotes = value;
}

int Note::laneNum() const
{
    return _laneNum;
}
void Note::setLaneNum(int value)
{
    _laneNum = value;
}

void Note::calculateInfo(int bpm)
{
    int px = this->x();
    int py = this->y();

    qreal duration = 0;

    double n = BEAT_HEIGHT / NOTE_HEIGHT;
    //1beatあたりにかかる時間
    qreal msecPerBeat = static_cast<double>(60.0 / (n*bpm)) * 1000;

    //もしロングノーツならば差分を保持しておく
    if(_isLongNotes)
        duration = (_rect.height() / NOTE_HEIGHT) * msecPerBeat;

    //今の位置が何ビート目かを計測
    int currentBeat = static_cast<int>(py / NOTE_HEIGHT);

    _startTime = currentBeat * msecPerBeat;
    _endTime = _startTime + duration;

    //レーン番号の計算
    _laneNum = static_cast<int>(px / NOTE_WIDTH);
}

void Note::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::darkGray, 2);
    painter->setPen(pen);

    if(_isStar == false)
    {
        QBrush brush(Qt::lightGray);
        painter->setBrush(brush);
        painter->drawRect(_rect);
    }
    else
    {
        QBrush brush(Qt::red);
        painter->setBrush(brush);
        painter->drawRect(_rect);
    }
}

/**
 * @brief Note::setLongNotes ロングノーツ化
 * @param p
 */
void Note::setLongNotes(qreal top)
{
    _isLongNotes = true;
    int y = top / NOTE_HEIGHT;
    y += 1;
    qreal currentPos = this->y();
    //qDebug(QString::number(y*NOTE_HEIGHT).toStdString().c_str());
    //qDebug(QString::number(currentPos).toStdString().c_str());
    _rect.setHeight((y*NOTE_HEIGHT) - currentPos);

    update(_rect);
}

void Note::changeStar()
{
    if(_isStar==false)
    {
        _isStar = true;
    }
    else
    {
        _isStar = false;
    }
    //設定を反映させる
    update(_rect);
}

/**
 * @brief Note::operator < qSortでソートを行う際に定義する必要がある
 * @param other
 * @return
 */
bool Note::operator<(const Note* &other) const
{
    return this->startTime() < other->startTime();
}


bool Note::compare(const Note* d1, const Note* d2)
{
    return (d1->startTime() < d2->startTime());
}
