#ifndef THREADCOM_H
#define THREADCOM_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>
#include <QSlider>
#include <QString>
#include <QTimer>

class threadCom :public QObject
{
    Q_OBJECT

signals:
    //信号打开，端口成功。
    void singalComOpenOk(bool);
    void singalSendProcess(QByteArray);
    //信号没有数据到达
    void singalNoPack(bool);
public slots:
    //打开端口
    void sloteOpenCom(QString comName,QString comId);      //打开端口

    void slotCloseCom();                    //关闭端口
    void slotComReadyRead();

    void slotComTimerTimeOut();
    //发送数据
    void slotComSendData(QByteArray buf);
    //端口打开错误
    void slotComerror(QSerialPort::SerialPortError err);

    void cheeckPackOutQtimer();
public:
    threadCom();

    bool comisOpen(void)const{return Com.isOpen();}

    QString getComName(void)const{return _comName;}
    QString getComID(void)const{return _comID;}
private:
    //实质端口
    QSerialPort Com;

    QString _comName;
    QString _comID;
    //串口接收到数据缓冲
    QByteArray comDatBuff;

    QTimer comTimer;        //串口获取数据延时处理
    int32_t comTimerTime = 30;

    //检查是否有数据到定时器
    QTimer cheeckPackQtimer;

    bool havePackFlag = false;
};

#endif // THREADCOM_H
