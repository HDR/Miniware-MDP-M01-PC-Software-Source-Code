#include "threadcom.h"
#include <QDebug>
#include <QObject>

//打开端口
void threadCom::sloteOpenCom(QString comName,QString comId)
{
    _comName = comName;
    _comID = comId;
    if(Com.isOpen())
    {
        Com.clear();
        Com.close();
    }
    Com.setPortName(comName);

    if(Com.open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        emit singalComOpenOk(true);
    }
    else
    {
        qDebug()<<"open Com error";
        emit singalComOpenOk(false);
    }


    Com.clear();

    //开启检测是否有包过来
    cheeckPackQtimer.start(3000);
}

//关闭端口
void threadCom::slotCloseCom()
{
    cheeckPackQtimer.stop();
    Com.clear();
    Com.close();
    _comName = "";
    _comID = "";
}

void threadCom::slotComReadyRead()
{
    //说明有数据到
    havePackFlag = true;
    comTimer.start(comTimerTime);

    //comDatBuff = Com.readAll();
    //qDebug() << comDatBuff.toHex();
    return;
}

void threadCom::slotComTimerTimeOut()
{
    comTimer.stop();
    comDatBuff.clear();

    comDatBuff = Com.readAll();

    //qDebug() << comDatBuff.toHex();

    emit singalSendProcess(comDatBuff);
    //qDebug() << comDatBuff.toHex();
}

void threadCom::slotComSendData(QByteArray buf)
{
    //qDebug() << buf.toHex();
    Com.write(buf);
}

void threadCom::slotComerror(QSerialPort::SerialPortError err)
{
    qDebug() << err;
}

void threadCom::cheeckPackOutQtimer()
{
    static int count = 0;
    if(havePackFlag == false)
    {
        count++;
        if(count >= 2)
        {
            emit singalNoPack(true);
        }
    }

    havePackFlag = false;

}

threadCom::threadCom()
{
    Com.clear();
    Com.close();
    Com.setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);//设置波特率和读写方向
    Com.setDataBits(QSerialPort::Data8);		//数据位为8位
    Com.setFlowControl(QSerialPort::NoFlowControl);//无流控制
    Com.setParity(QSerialPort::NoParity);	//无校验位
    Com.setStopBits(QSerialPort::OneStop); //一位停止位


    connect(&Com,SIGNAL(readyRead()),this,SLOT(slotComReadyRead()));

    connect(&Com,SIGNAL(error(QSerialPort::SerialPortError)),this,SLOT(slotComerror(QSerialPort::SerialPortError)));

    connect(&comTimer,SIGNAL(timeout()),this,SLOT(slotComTimerTimeOut()));

    connect(&cheeckPackQtimer,SIGNAL(timeout()),this,SLOT(cheeckPackOutQtimer()));
}




