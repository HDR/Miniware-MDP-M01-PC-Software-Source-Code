#ifndef SWITCHCOM_H
#define SWITCHCOM_H

#include <QWidget>
#include <QFontDatabase>

namespace Ui {
class switchCom;
}

class switchCom : public QWidget
{
    Q_OBJECT
signals:
    void signalOpenCom(QString,QString);
public:
    explicit switchCom(QWidget *parent = nullptr);


    ~switchCom();

public slots:
    void on_pushButtonScanCom_clicked();

    void on_pushButtonOpenCom_clicked();

private:
    Ui::switchCom *ui;

    int fontId;
    QString fontName;
protected:
    void showEvent(QShowEvent *event);
};

#endif // SWITCHCOM_H
