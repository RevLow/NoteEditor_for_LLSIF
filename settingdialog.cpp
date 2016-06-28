#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "QSettings"
#include <QFileDialog>
#include <QDir>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    QSettings settings("./setting.ini",QSettings::IniFormat, this);
    ui->simulatorPath->setText(settings.value("SIMULATOR_PATH", "").toString());

}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::btnOk_action()
{
    QString simPath = ui->simulatorPath->text();
    QSettings settings("./setting.ini",QSettings::IniFormat, this);
    settings.setValue("SIMULATOR_PATH", simPath);
    settings.sync();
    emit dialogCloseSignal();
}

