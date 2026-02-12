#ifndef MACHINE_H
#define MACHINE_H

#include "stdint.h"
#include "QString"
#include "QStringList"
#include "QColor"

class machine
{
public:
    //机器类型
    enum machineTypeEnum
    {
        nodeType = 0,
        P905,
        P906,
        L1060        //电子负载
    };
    //输入输出类型
    enum outInModeEnum
    {
        OFF = 0,
        CC = 1,
        CV,
        CR,
        CP,
        ON,
        nodeMode,
    };
    //输出状态
    enum outputStatus
    {
        OUTPUT_OFF = 0,
        OUTPUT_ON = 1,
    };

    const QStringList machineTypeQString = {"","P905","P906","L1060"};
    const QStringList outInModeEnumQString = {"OFF","CC","CV","CR","CP","ON","ERR"};

    machine();


public:


    //设定电压电流
    uint16_t setPutVoltage = 0;
    uint16_t setPutCurrent = 0;
    uint32_t setPutPower   = 0;

    QString setPutVoltageStr(){return powerU16ToQSting(setPutVoltage);}
    QString setPutCurrentStr(){return powerU16ToQSting(setPutCurrent);}
    QString setPutPowerStr()  {return powerU16ToQSting(setPutPower  );}

    bool updatSetPutFlag = false;
    uint16_t updatSetPutVoltage = 0;
    uint16_t updatSetPutCurrent = 0;
    uint32_t updatSetPutPower   = 0;
//===================================
    //输出电压电流
    uint16_t outPutVoltage = 0;
    uint16_t outPutCurrent = 0;
    uint32_t outPutPower   = 0;

    uint16_t waveOutPutVoltage = 0;
    uint16_t waveOutPutCurrent = 0;
    uint32_t waveOutPutPower   = 0;

    QString outPutVoltageStr(){return powerU16ToQSting(outPutVoltage);}
    QString outPutCurrentStr(){return powerU16ToQSting(outPutCurrent);}
    QString outPutPowerStr()  {return powerU16ToQSting(outPutPower  );}

//===================================
    //输入电压电流
    uint16_t inPutVoltage  = 0;
    uint16_t inPutCurrent  = 0;
    uint32_t inPutPower    = 0;

    QString inPutVoltageStr(){return powerU16ToQSting(inPutVoltage);}
    QString inPutCurrentStr(){return powerU16ToQSting(inPutCurrent);}
    QString inPutPowerStr()  {return powerU16ToQSting(inPutPower  );}

//===================================
    //波形电压电流
    uint16_t waveVoltage    = 0;
    uint16_t wavePutCurrent = 0;
    uint32_t wavePutPower   = 0;
//===================================
    //类型
    machineTypeEnum machineType = nodeType;
    QString machineTypeString = machineTypeQString.at(machineType);
    QString machineTypeStr()const{return machineTypeQString.at(machineType);}
    bool machineTypeUpdatFlag = false;
//===================================
    //输入输出模式
    outInModeEnum outInMode = nodeMode;
    QString outInModeString = outInModeEnumQString.at(outInMode);
    QString outInModeSrt()const{return outInModeEnumQString.at(outInMode);}
    bool outInModeUpdatFlag = false;
//===================================
    //是否输入输出 false为关
    bool outPutState = false;

    bool updatoutPutState = false;

    bool updatoutPutStateFlag = false;
//===================================
    //温度
    uint32_t temp = 0;

    QString tempStr() {double tmp_temp = temp;tmp_temp /= 10.0;return QString().sprintf("%.1f",tmp_temp);}
//===================================
    //地址频率
    bool addrEmpty = true;

    //如果为真,表示地址变化了,需要进行刷新
    bool addressFlag = false;
    uint8_t address[5] = {0};
    uint32_t freq = 0;

    //手动更新的时候标志位会为true 表示地址更新了
    uint8_t upDatAddress[5] = {0};
    uint32_t upDatFreq = 0;
    bool updatAddressFlag = true;

    void setNewAddr(const uint8_t *addr,uint32_t freq);
    void setNewAddr(QString addr,QString freq);

    QString getAddrStr()const{return QString().sprintf("%X:%X:%X:%X:%X",address[0],address[1],address[2],address[3],address[4]);}
    QString getFreqStr()const;

    QString getUpdatAddrStr()const{return QString().sprintf("%X:%X:%X:%X:%X",upDatAddress[0],upDatAddress[1],upDatAddress[2],upDatAddress[3],upDatAddress[4]);}
    QString getUpdatFreqStr()const;

//===================================
    //颜色
    QColor color = QColor(128,128,128);
    bool colorUpdatFlag = false;
//===================================
    //锁
    bool lock = false;
    bool lockUpdatFlag = false;
//===================================
    //在线
    bool onLine = false;
    bool onLineUpdatFlag = false;
//===================================
    //机械编号
    uint8_t NO;


private:
    QString powerU16ToQSting(uint16_t num);
    QString powerU16ToQSting(uint32_t num);
};

#endif // MACHINE_H


