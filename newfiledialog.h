#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>

namespace Ui {
class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(QWidget *parent = 0);
    ~NewFileDialog();
signals:
    void dialogSignal(QString filepath, int bpm);
public slots:
    void btnOpenSong_action();
    void btnOk_action();
private:
    Ui::NewFileDialog *ui;
};

#endif // NEWFILEDIALOG_H
