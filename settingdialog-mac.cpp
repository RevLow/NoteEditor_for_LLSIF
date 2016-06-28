#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "QSettings"
#include <QFileDialog>
#include <QDir>

void SettingDialog::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::homePath(),
                                                    tr("(*.*)"));
    fileName += "/Contents/MacOS/SifSim-desktop";
    ui->simulatorPath->setText(fileName);
}
