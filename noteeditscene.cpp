#include "noteeditscene.h"
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QDrag>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QSoundEffect>
#include <QFileDialog>
NoteEditScene::NoteEditScene(QObject *parent) : QGraphicsScene(parent),
    m_clickPoint(QPointF(0,0)),
    bpm(120),
    msec(300000),
    _grabItem(0)
{
    clear();
    drawVGrid();
    drawHGrid();
}

/**
 * @brief NoteEditScene::DrawHGrid エディッタの横線を描画するメソッド
 */
void NoteEditScene::drawHGrid()
{
    int allBeats = ((static_cast<double>(bpm) / 60.0) / 1000.0) * msec;
    int width = Note::NOTE_WIDTH;
    int height = Note::NOTE_HEIGHT;

    allBeats *= Note::BEAT_HEIGHT / Note::NOTE_HEIGHT;

    for(int n=0;n < allBeats;n++)
    {
        QPen pen;

        if(n%16 == 0)
        {
            pen = QPen(Qt::green);
        }
        else if(n%4 == 0)
        {
            pen=QPen(Qt::white);

        }
        else
        {
            pen = QPen(Qt::darkGray);
        }

       QGraphicsLineItem *line = addLine(0, height*n, width*9, height*n, pen);
       _lineVec.push_back(line);
    }

}

void NoteEditScene::drawVGrid()
{
    int allBeats = ((static_cast<double>(bpm) / 60.0) / 1000.0) * msec;
    int width = Note::NOTE_WIDTH;
    int height = Note::BEAT_HEIGHT;

    for(int n=0;n < 10;n++)
    {
        QPen pen = QPen(Qt::white);
        QGraphicsLineItem *line = addLine(n * width, 0, n * width, height * allBeats, pen);
    }
}

/**
 * @brief NoteEditScene::changeBpmCount ボックスに入力されたbpmを適用するスロット
 * @param value テキストボックスに入力された文字列
 */
void NoteEditScene::changeBpmCount(QString value)
{
    qDebug("CALLED FROM NoteEditScene");
    unsigned int inputValue = value.toUInt();
    if(inputValue > 0)
    {
        bpm = inputValue;
        //bpmの変更に伴い、現在の位置の時間を再計測する
        if(noteList.size() != 0)
        {
            for(Note *n : noteList)
            {
                n->calculateInfo(bpm);
            }
        }
    }
}

/**
 * @brief NoteEditScene::triggerAdd モードを切り替えたときに実行される処理
 */
void NoteEditScene::triggerAdd()
{
    currentMode = ADD;
}

void NoteEditScene::triggerMove()
{
    currentMode = MOVE;
}

void NoteEditScene::triggerErase()
{
    currentMode = ERASE;
}

void NoteEditScene::triggerLN()
{
    currentMode = LONGNOTE;
}

void NoteEditScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
       QPointF mousePoint;
       mousePoint = event->scenePos();
       m_clickPoint = mousePoint;

       if(currentMode == MOVE
        ||currentMode == LONGNOTE)
       {
           QGraphicsItem *gItem = itemAt(mousePoint.x(), mousePoint.y(), QTransform());

           if(gItem != nullptr && gItem->type() == Note::Type)
           {
               //もしもオブジェクトをつかんだら
               QGraphicsView *view = this->views().at(0);
               view->setDragMode(QGraphicsView::DragMode::NoDrag);

               _grabItem = gItem;
               return;
           }
       }
       else if(currentMode == ERASE)
       {
           QGraphicsItem *gItem = itemAt(mousePoint.x(), mousePoint.y(), QTransform());
           if(gItem != nullptr && gItem->type() == Note::Type)
           {
               noteList.removeOne(dynamic_cast<Note*>(gItem));
               removeItem(gItem);
           }
       }
       else
       {
           qreal widthRatio  = Note::NOTE_WIDTH;
           qreal heightRatio = Note::NOTE_HEIGHT;
           auto note = new Note(QRect(0,0, widthRatio, heightRatio));
           //QRectF rect = this->sceneRect();
           int px = static_cast<int>(mousePoint.x() / widthRatio) * widthRatio;
           int py = static_cast<int>(mousePoint.y() / heightRatio) * heightRatio;
           note->setPos(px, py);
           addItem(note);

           note->calculateInfo(bpm);
           noteList.append(note);

           qDebug("X: %lf, Y: %lf", mousePoint.x(), mousePoint.y());
       }
      return;
}

void NoteEditScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(_grabItem == 0) return;
     auto p = event->scenePos();
     if(p.y() < 0) return;
     qreal dY = p.y() - m_clickPoint.y();

     qreal height = Note::NOTE_HEIGHT;
     qreal resultY = static_cast<int>(dY/height);
     if(currentMode == LONGNOTE)
     {
         dynamic_cast<Note*>(_grabItem)->setLongNotes(static_cast<int>(m_clickPoint.y() / height) * height + (resultY * height));
         dynamic_cast<Note*>(_grabItem)->calculateInfo(bpm);
     }
     else
     {
         _grabItem->setY(static_cast<int>(m_clickPoint.y() / height) * height + (resultY * height));
     }

}

void NoteEditScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(_grabItem != 0)
    {
        QGraphicsView *view = this->views().at(0);
        view->setDragMode(QGraphicsView::DragMode::RubberBandDrag);
        dynamic_cast<Note*>(_grabItem)->calculateInfo(bpm);

        _grabItem = 0;
    }
}


/**
 * @brief NoteEditScene::changeTimes 再生時にスクロールが行われたときに呼ばれる
 * @param millisec
 */
void NoteEditScene::changeTimes(qint64 millisec)
{

   for(QList<Note*>::iterator it = noteList.begin();it!=noteList.end();it++)
   {
       Note* n = *it;
       qint64 s = n->startTime();
       qint64 e = n->endTime();
       if((s >= previousTime && s < millisec) ||
               (e >= previousTime && e < millisec))
       {
            QSoundEffect *snd = new QSoundEffect();
            snd->setSource(QUrl::fromLocalFile(":/new/sound/perfect.wav"));
            snd->play();
            QObject::connect(snd, &QSoundEffect::playingChanged, [this](){
                delete sender();
            });



//            QMediaPlayer *player = new QMediaPlayer;
//            player->setMedia(QUrl::fromLocalFile("/Users/tetsushi2/Desktop/perfect.wav"));
//            QObject::connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(destoryPlayer(QMediaPlayer::State)));
//            playerList.append(player);
//            player->play();
//            qDebug()<<player->state();
//            if(player->state() == QMediaPlayer::State::StoppedState)
//            {
//                player->play();
//            }
//            else if(player->state() == QMediaPlayer::PlayingState)
//            {

//                player->stop();
//                player->setPosition(0);
//                player->play();
//            }
           break;
       }
   }

    previousTime = millisec;
}

void NoteEditScene::openJson(QJsonObject json)
{
    for(Note* n : noteList)
    {
        noteList.removeOne(n);
        removeItem(n);
    }

    QJsonArray array = json["lane"].toArray();
    for(int i=0;i<array.size();i++)
    {
        QJsonArray notes = array.at(i).toArray();
        for(int j=0;j<notes.size();j++)
        {
            QJsonObject json_note = notes.at(j).toObject();
            //noteの情報を取得
            qreal startTime = json_note["starttime"].toDouble();
            qreal endTime = json_note["endtime"].toDouble();
            bool isLongNote = json_note["longnote"].toBool();
            bool isHold = json_note["hold"].toBool();
            bool isParallel = json_note["parallel"].isBool();
            int lane = i;

            double n = Note::BEAT_HEIGHT / Note::NOTE_HEIGHT;
            //位置を計算
            qreal startPosY = (startTime / 1000.0) * (static_cast<qreal>(n*bpm) / 60.0);
            qreal endPosY = (endTime / 1000.0) * (static_cast<qreal>(n*bpm) / 60.0);
            //heightを計算
            qreal height = ( endPosY - startPosY + 1 ) * Note::NOTE_HEIGHT;
            int posX = lane * Note::NOTE_WIDTH;

            QRectF rect(0, 0, Note::NOTE_WIDTH, height);
            Note *note = new Note(rect);
            note->setPos(posX, startPosY * Note::NOTE_HEIGHT);
            addItem(note);
            note->setStartTime(startTime);
            note->setEndTime(endTime);
            note->setIsLongNotes(isLongNote);
            note->setIsStar(isHold);
            note->setLaneNum(lane);

            noteList.append(note);
        }
    }
}

/**
 * @brief NoteEditScene::saveJson 指定したパスにjson情報を保存する
 * @param path
 */
void NoteEditScene::saveJson(QString path)
{
    //QString saveName = QFileDialog::getSaveFileName(this->views()[0], tr("Save File"), QDir::homePath(), tr("Json (*.json)"));

    QFile f(path);

    if(!f.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save.json");
        return;
    }
    //ソート
    qSort(noteList.begin(), noteList.end(), Note::compare);

    QVector< QVector< QSharedPointer<QJsonObject> > > lanes;
    for(int i=0;i < 9;i++)
    {
        QVector< QSharedPointer<QJsonObject> > lane;
        lanes.append(lane);
    }

    QHash<int, QSharedPointer<QJsonObject> > timeMap;

    for(Note* n : noteList)
    {
        qreal startTime = n->startTime();
        qreal endTime = n->endTime();
        int lane =n->laneNum();
        bool isLongNote = n->isLongNotes();
        bool isHold = n->isStar();

        QSharedPointer<QJsonObject> noteInfo(new QJsonObject);
        noteInfo->insert("starttime", startTime);
        noteInfo->insert("endtime", endTime);
        noteInfo->insert("longnote", isLongNote);
        noteInfo->insert("hold", isHold);
        noteInfo->insert("parallel", false);
        noteInfo->insert("lane", lane);


        //もしもハッシュの中に同じ時間のデータが存在する場合
        QHash<int, QSharedPointer<QJsonObject> >::iterator it = timeMap.find(startTime);
        if(it != timeMap.end() && it.key() == startTime)
        {
            noteInfo->insert("parallel", true);
            //発見したものも直す
            it.value()->insert("parallel",true);
        }
        else
        {
            timeMap.insert(startTime,noteInfo);
        }

        //lanesに追加
        lanes[lane].append(noteInfo);
    }


    QJsonObject saveObject;
    QJsonArray mainArray;
    for(QVector<QSharedPointer<QJsonObject> > v : lanes)
    {
        QSharedPointer<QJsonArray> array(new QJsonArray);
        for(QSharedPointer<QJsonObject> json : v)
        {
            array->append(*json);
        }
        mainArray.append(*array);
    }

    saveObject.insert("lane", mainArray);
    // QJsonオブジェクトをテキストの形にして、保存する
    QJsonDocument saveDoc(saveObject);
    f.write(saveDoc.toJson());
    f.close();
}
