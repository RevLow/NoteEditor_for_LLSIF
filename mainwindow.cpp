#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QAction>
#include <QActionGroup>
#include <newfiledialog.h>
#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QInputDialog>
#include "settingdialog.h"
#include <QSettings>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player(new QMediaPlayer),
    bpm(120),
    openedJsonFile(""),
    openedSongFile("")
{
    ui->setupUi(this);

    _scene = new NoteEditScene(this);
    QObject::connect(ui->bpmBox, SIGNAL(textChanged(QString)), ui->graphicsView, SLOT(changeBpmCount(QString)));
    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->addaction);
    group->addAction(ui->moveaction);
    group->addAction(ui->actionEraser);
    group->addAction(ui->actionLN);

    group->setExclusive(true);
    ui->moveaction->setChecked(true);

    QObject::connect(ui->actionSinkisakusei, SIGNAL(triggered(bool)), this, SLOT(newProject()));
    QObject::connect(ui->actionOpen,SIGNAL(triggered(bool)), this, SLOT(openFile()));
    QObject::connect(ui->actionJsonSave, SIGNAL(triggered(bool)), this, SLOT(saveJson()));

    QObject::connect(ui->addaction, SIGNAL(triggered(bool)), _scene, SLOT(triggerAdd()));
    QObject::connect(ui->moveaction, SIGNAL(triggered(bool)), _scene, SLOT(triggerMove()));
    QObject::connect(ui->actionEraser, SIGNAL(triggered(bool)), _scene, SLOT(triggerErase()));
    QObject::connect(ui->actionLN, SIGNAL(triggered(bool)), _scene, SLOT(triggerLN()));
    QObject::connect(ui->environment_action, SIGNAL(triggered(bool)), this,SLOT(openSettingDialog(bool)));
    player->setNotifyInterval(1);

    QObject::connect(ui->graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(verticalScrolled(int)));
    QObject::connect(this, SIGNAL(songInitSignal(qint64)), ui->graphicsView, SLOT(initCurrentTime(qint64)));

    QObject::connect(this, SIGNAL(changeTimes(qint64)), _scene, SLOT(changeTimes(qint64)));
    QObject::connect(this, SIGNAL(openJson(QJsonObject)), _scene, SLOT(openJson(QJsonObject)));
    ui->graphicsView->setScene(_scene);
    ui->graphicsView->scale(-1, 1);
    ui->graphicsView->rotate(180);

    QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));

    QSettings settings("./setting.ini",QSettings::IniFormat, this);
    this->simulatorPath = settings.value("SIMULATOR_PATH", "").toString();

    QObject::connect(ui->actionPlayBySimulator, SIGNAL(triggered(bool)), this, SLOT(playBySimulator()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWindow::newProject ファイルの新規作成アクション
 */
void MainWindow::newProject()
{
    NewFileDialog *nfd = new NewFileDialog(this);
    QObject::connect(nfd, SIGNAL(dialogSignal(QString,int)), this, SLOT(dialogResult(QString,int)));
    nfd->show();
}

/**
 * @brief MainWindow::openSettingDialog
 * @param value
 */
void MainWindow::openSettingDialog(bool value)
{
    SettingDialog *dialog = new SettingDialog(this);
    QObject::connect(dialog, &SettingDialog::dialogCloseSignal, [this](){
        QSettings settings("./setting.ini",QSettings::IniFormat, this);
        this->simulatorPath = settings.value("SIMULATOR_PATH", "").toString();
    });
    
    dialog->show();
}

/**
 * @brief MainWindow::playBySimulator シミュレータ起動アクションの実行
 */
void MainWindow::playBySimulator()
{
    //シミュレータの設定がされてない場合
    if(simulatorPath == "")
    {
        QMessageBox msgBox;
        msgBox.setText("シミュレータの設定が行われてません");
        msgBox.exec();
        return;
    }

    //楽曲ファイルの指定がない場合
    if(openedSongFile == "")
    {
        QMessageBox msgBox;
        msgBox.setText("楽曲ファイルの指定が行われていません");
        msgBox.exec();
        return;
    }
    //saveダイアログを開く
    if(openedJsonFile == "")
    {
        openedJsonFile = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("Json (*.json)"));
        if(openedJsonFile == "")
        {
            QMessageBox msgBox;
            msgBox.setText("jsonファイルが保存されていません");
            msgBox.exec();
            return;
        }
        dynamic_cast<NoteEditScene*>(ui->graphicsView->scene())->saveJson(openedJsonFile);
    }

    //シミュレータの起動
    QProcess::execute(simulatorPath + " -s " + openedSongFile + " -j " + openedJsonFile);
}
/**
 * @brief MainWindow::saveJson jsonファイルを保存する
 */
void MainWindow::saveJson()
{
    QString saveName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), tr("Json (*.json)"));
    dynamic_cast<NoteEditScene*>(ui->graphicsView->scene())->saveJson(saveName);
    openedJsonFile = saveName;
}

/**
 * @brief MainWindow::openFile ファイルを開く
 */
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::homePath(),
                                                    tr("Json (*.json)"));

   QFile loadFile(fileName);
   if(!loadFile.open(QIODevice::ReadOnly))
   {
       qWarning() << "Couldn't open " << fileName;
       return;
   }

   // save.jsonの中身を取得
   QByteArray data = loadFile.readAll();

   // 取得したテキストからJsonを扱うためのオブジェクト生成
   auto jsonDoc = QJsonDocument::fromJson(data);

   bool ok;
   QString input_value = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                               tr("BPM:"), QLineEdit::Normal,
                                               "120", &ok);
   ui->bpmBox->setText(input_value);
   emit openJson(jsonDoc.object());
   openedJsonFile = fileName;
}

/**
 * @brief MainWindow::playSound 再生ボタンを押したときの処理
 */
void MainWindow::playSound()
{
    if(player->mediaStatus() != QMediaPlayer::MediaStatus::NoMedia)
    {
        if(player->state() == QMediaPlayer::State::PlayingState)
        {
            player->pause();
            ui->playButton->setIcon(QIcon(":/new/prefix1/Play-icon.png"));
        }
        else
        {
            player->play();
            ui->playButton->setIcon(QIcon(":/new/prefix1/Pause-icon.png"));
        }
    }
}

/**
 * @brief MainWindow::verticalScrolled 垂直スクロールバーが変更されたときに呼ばれる
 * @param value 変更後の位置 (上に行くほどマイナスになっていく)
 */
void MainWindow::verticalScrolled(int value)
{
    //QTransform t = ui->graphicsView->viewportTransform();
    //QRect r = ui->graphicsView->frameRect();

    //qDebug("%lf %lf", t.dx(), t.dy() - r.height());

    //qDebug() << player->duration();
    QRect r = ui->graphicsView->frameRect();
    QTransform t = ui->graphicsView->viewportTransform();
    qreal bottom = t.dy() - r.height();

    if(player->state() != QMediaPlayer::State::PlayingState)
    {
        currentTime = (bottom / static_cast<qreal>(Note::BEAT_HEIGHT) ) * (60.0 / static_cast<qreal>(bpm)) * 1000.0;
        player->setPosition(currentTime);
        emit changeTimes(currentTime);
    }

    currentValue = bottom;
}



/**
 * @brief MainWindow::positionChanged 曲の位置が移動したときに呼ばれる
 * @param millisec 現在の時間(millisecond)
 */
void MainWindow::positionChanged(qint64 millisec)
{
    ui->nowTime->setText(QString::number(millisec));

    if(player->state() == QMediaPlayer::State::PlayingState)
    {
        qint64 dt = millisec - currentTime;

        double x = static_cast<double>(bpm) / 60000.0;
        qreal nBeat = x * dt;

        qreal dy = nBeat * (qreal)Note::BEAT_HEIGHT;
        QTransform t = ui->graphicsView->transform();
        t.translate(0, -dy);
        ui->graphicsView->setTransform(t);

        emit changeTimes(millisec);
        currentTime = millisec;
        ui->nowTime->setText(QString::number(currentTime));
    }

}


/**
 * @brief MainWindow::dialogResult ダイアログを閉じたときの処理
 * @param filepath　曲のファイルパス
 * @param bpm　bpm情報
 */
void MainWindow::dialogResult(QString filepath, int bpm)
{
    //qDebug("SONG_PATH: %s, BPM: %d", filepath.toStdString().c_str(), bpm);
    if(filepath != "") openedSongFile = filepath;
    player->setMedia(QUrl::fromLocalFile(filepath));
    currentTime = player->position();
    ui->nowTime->setText(QString::number(currentTime));
    ui->graphicsView->verticalScrollBar()->setValue(ui->graphicsView->verticalScrollBar()->maximum());

    ui->bpmBox->setText(QString::number(bpm));
    setWindowTitle(filepath);

    QRect r = ui->graphicsView->frameRect();
    QTransform t = ui->graphicsView->viewportTransform();
    currentValue = t.dy() - r.height();

    emit songInitSignal(player->position());
}

void MainWindow::changeBpm(QString bpm)
{
    qDebug(bpm.toStdString().c_str());
    this->bpm = bpm.toInt();
}
