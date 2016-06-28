#include "newfiledialog.h"
#include "ui_newfiledialog.h"
#include <QFileDialog>
#include <QDir>

NewFileDialog::NewFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);
}

NewFileDialog::~NewFileDialog()
{
    delete ui;
}

void NewFileDialog::btnOpenSong_action()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::homePath(),
                                                    tr("MP3 (*.mp3 *.wav)"));
    ui->songLineEdit->setText(fileName);
}

void NewFileDialog::btnOk_action()
{
    QString filepath = ui->songLineEdit->text();
    int bpm = ui->bpmLineEdit->text().toInt();

    emit dialogSignal(filepath, bpm);
}
