#include "machine.h"
#include "string.h"
#include <QStringList>
#include <QDebug>
machine::machine()
{

}

void machine::setNewAddr(const uint8_t *addr, uint32_t freq)
{
   for (int i = 0;i < 5;i++)
   {
       upDatAddress[4 - i] = addr[i];
   }
   upDatFreq = freq;
    //qDebug() << upDatFreq;
   updatAddressFlag = true;
}

void machine::setNewAddr(QString addr, QString freq)
{
    uint8_t tmpAddr[5] = {0};
    uint32_t tmpFreq = 0;

    QStringList tmpList = addr.split(":");
    if(tmpList.count() != 5)
    {
        qDebug() << "tmpAddr to split error";
        return;
    }

    bool ok = true;
    for(int i = 0;i < 5;i++)
    {
       tmpAddr[i] = static_cast<uint8_t>(tmpList[i].toUInt(&ok,16));

       if(!ok)
       {
           qDebug() << "tmpAddr to int error";
           return;
       }
    }

    freq.remove('.');

    tmpFreq = freq.toUInt(&ok);
    if(!ok)qDebug() << "tmpFreq to int error";

    if(0 == tmpAddr[0] && 0 == tmpAddr[1] && 0 == tmpAddr[2] && 0 == tmpAddr[3] && 0 == tmpAddr[4])
    {
       addrEmpty = true;
       tmpFreq = 2400;
    }
    else
    {
       addrEmpty = false;
    }



    setNewAddr(tmpAddr, tmpFreq);
}

QString machine::getFreqStr() const
{
    QString tmp = QString("%1").arg(freq);
    tmp.insert(1,".");
    return tmp;
}

QString machine::getUpdatFreqStr() const
{
    QString tmp = QString("%1").arg(upDatFreq);
    tmp.insert(1,".");
    return tmp;

}

QString machine::powerU16ToQSting(uint16_t num)
{
    double tmp_num = num;
    tmp_num = tmp_num / 1000.0;
    return QString().sprintf("%.3f",tmp_num);
}

QString machine::powerU16ToQSting(uint32_t num)
{
    double tmp_num = num;
    tmp_num = tmp_num / 1000.0;
    return QString().sprintf("%.3f",tmp_num);
}
