#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QJsonObject>

#include "noteeditscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void songInitSignal(qint64 millisec);
    void changeTimes(qint64 millisec);
    void openJson(QJsonObject json);
public slots:
    void newProject();//新規作成
    void openFile();//ファイル選択
    void dialogResult(QString filepath, int bpm);//新規作成後に呼ばれる
    void playSound();//再生ボタン押下
    void verticalScrolled(int value);//verticalがスクロールしたとき
    void positionChanged(qint64 millisec);//再生音がシークしたとき
    void changeBpm(QString bpm);//
    void openSettingDialog(bool value);
    void playBySimulator();
    void saveJson();
private:
    Ui::MainWindow *ui;
    QString simulatorPath;
    NoteEditScene *_scene;
    QMediaPlayer *player;
    qreal currentValue;
    qint64 currentTime;
    QString openedSongFile;
    QString openedJsonFile;
    int bpm;
};

#endif // MAINWINDOW_H
