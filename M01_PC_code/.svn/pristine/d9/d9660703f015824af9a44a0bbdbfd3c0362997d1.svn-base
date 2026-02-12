#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "threadcom.h"
#include <windows.h>
#include <dbt.h>
#include <devguid.h>
#include <setupapi.h>
#include <initguid.h>
#include <QDebug>
#include <QThread>
#include <QSettings>
#include <QFileDialog>
#include <QStyleFactory>
#include <QMessageBox>
#include <QSplashScreen>
#include <QScreen>
#include <QTextCodec>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setFixedSize(this->width(),this->height());

    //界面的一些初始化设置
    UiInit();
    //全部的连接函数
    connectAll();
    //设置语言
    setLanguage(cn);
    //设置标题
    setWindowTitle(tr("MDP_M01上位机软件") + softwareVersion);

    //将串口和数据处理各放在一条线程里面
    QThread *tmpThread_1;
    tmpThread_1 = new QThread();
    baseCom.moveToThread(tmpThread_1);
    tmpThread_1->start();

    //数据处理函数
    QThread *tmpThread_2;
    tmpThread_2 = new QThread();
    proData.moveToThread(tmpThread_2);
    tmpThread_2->start();


    //从文件读取地址
    readAddrFormFile();
    //设置当前通道号
    proData.now_ch = 0;
    //扫描串口
    scanComList();

    //用于窗口旋转,接触注释即可使用,这里没有使用
    //runWindwoQtime.start(4);
}




MainWindow::~MainWindow()
{
    on_pushButtonDelAllAddr_clicked();
    on_pushButtonDelAllAddr_clicked();
    delete ui;
}

void MainWindow::setLanguage(MainWindow::language la)
{
    static QTranslator m_translator;
    static language old_language = nola;
    //this->hide();
    //第一次设置语言,读文件,忽略参数
    if(nola == old_language)
    {
        m_translator.load(":/pic/ui.qm","");
        la = readLanguageFormFile();
    }

    if(old_language == la)
    {
        if(cn == la)
        {
            ui->actionchina->setChecked(true);
            ui->actionEnglish->setChecked(false);
        }
        else
        {
            ui->actionchina->setChecked(false);
            ui->actionEnglish->setChecked(true);
        }

        return;
    }
    else
    {
        old_language = la;
    }

    switch (la)
    {
    case en:

        qApp->installTranslator(&m_translator);
        ui->actionEnglish->setChecked(true);
        ui->actionchina->setChecked(false);
        axisX->setTitleText(tr("time/ms"));//标题
        axisY_V->setTitleText(tr("Voltage(V)"));
        axisY_I->setTitleText(tr("Current(A)"));
        proData.series_V->setName(tr("(unit:V)"));
        proData.series_I->setName(tr("(unit:A)"));

        break;
    case cn:
        qApp->removeTranslator(&m_translator);
        ui->actionEnglish->setChecked(false);
        ui->actionchina->setChecked(true);

        axisX->setTitleText(tr("时间/ms"));//标题
        axisY_V->setTitleText(tr("电压(V)"));
        axisY_I->setTitleText(tr("电流(A)"));

        proData.series_V->setName(tr("(单位:V)"));
        proData.series_I->setName(tr("(单位:A)"));

        break;
    default:break;
    }

    old_language = la;

    this->ui->retranslateUi(this);
    slotUpdatDataToUi();
    writeLanguageToFile(la);

    slotUpdatUiaddr();
    slotUpdatUiaddr();

}

MainWindow::language MainWindow::readLanguageFormFile()
{
    QFileInfo file("文件路径");
    if(file.exists() == false)
    {
        QLocale ql;
        //语言不是中文就返回英文
        if(ql.language() == QLocale::Chinese)
        {
            return cn;
        }
        else
        {
            return en;
        }
    }

    language ret = nola;
    QSettings *configIni = new QSettings(QCoreApplication::applicationDirPath() + "/address.ini", QSettings::IniFormat);

    configIni->beginGroup("sofeConfig");

    QString tmpStr = configIni->value("Language").toString();

    if(tmpStr == "CN")
    {
        ret = cn;
    }
    else if(tmpStr == "EN")
    {
        ret = en;
    }
    else
    {
        ret = cn;
    }

    configIni->endGroup();

    delete configIni;
    return ret;
}

void MainWindow::writeLanguageToFile(MainWindow::language la)
{
    QSettings *configIni = new QSettings(QCoreApplication::applicationDirPath() + "/address.ini", QSettings::IniFormat);
    configIni->beginGroup("sofeConfig");

    QString tmpStr;

    switch (la)
    {
        case en:
            tmpStr = "EN";
        break;
        case cn:
            tmpStr = "CN";
        break;
    default:;
    }

    configIni->setValue("Language",tmpStr);
    configIni->endGroup();
    delete configIni;
}

void MainWindow::slotheartbeatTime()
{
    if(autoMachFlag == true)return;
    //qDebug() << proData.machineType;

    emit signalSendHeartBeat();
    if(proData.machineType == processingData::noLcd)
    {
        emit signalSendAllAddr();
    }
    else
    if(proData.machineType == processingData::haveLcd)
    {
        emit signalSendHeartBeat();
    }

}

void MainWindow::slotRunWindowTime()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect mm = screen->availableGeometry();
    int screen_width = mm.width();
    int screen_height = mm.height();
    int x,y;
    int a = 50;
    static double i = 20;
    if(i > 0)
    {
        x = static_cast<int>(a * (cos(i) + i * sin(i)) + screen_width / 2 - a - width() / 2);
        y = static_cast<int>(a * (sin(i) - i * cos(i)) + screen_height / 2 - a - height() / 2);
        move(x,y);
        i -= 0.05;
    }
    else
    {
        runWindwoQtime.stop();
    }
}

bool MainWindow::regUSBEvent()
{
//====================注册USB插拔事件===========================
    static const GUID GUID_DEVINTERFACE_LIST[] =
    {
    // GUID_DEVINTERFACE_USB_DEVICE
    { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } }

    };
    //注册插拔事件
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;
    ZeroMemory(&NotifacationFiler,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    for(uint32_t i=0;i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID);i++)
    {
        NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];//GetCurrentUSBGUID();
        hDevNotify = RegisterDeviceNotification(HANDLE(this->winId()),&NotifacationFiler,DEVICE_NOTIFY_WINDOW_HANDLE);
        if(!hDevNotify)
        {
            GetLastError();
            qDebug() << "注册失败";
            return false;
        }
    }

    return true;
}



void MainWindow::readAddrFormFile()
{
    QSettings *configIni = new QSettings(QCoreApplication::applicationDirPath() + "/address.ini", QSettings::IniFormat);
    QString tmp_path = "";

    for(int i = 0;i < 6;i++)
    {
        tmp_path = QString().sprintf("NO%d",i);

        configIni->beginGroup(tmp_path);


        QString tmp_addr = configIni->value("addr").toString();
        QString tmp_freq = configIni->value("freq").toString();



        if(tmp_addr == "" || tmp_freq == "")
        {
            tmp_addr = QString("0:0:0:0:0");
            tmp_freq = "2.400";
            configIni->setValue("addr",tmp_addr);
            configIni->setValue("freq",tmp_freq);


        }

        QStringList addrList = tmp_addr.split(":");

        if(5 == addrList.count())
        {
            proData.MDP[i].upDatAddress[0] = static_cast<uint8_t>(addrList[0].toUInt());
            proData.MDP[i].upDatAddress[1] = static_cast<uint8_t>(addrList[1].toUInt());
            proData.MDP[i].upDatAddress[2] = static_cast<uint8_t>(addrList[2].toUInt());
            proData.MDP[i].upDatAddress[3] = static_cast<uint8_t>(addrList[3].toUInt());
            proData.MDP[i].upDatAddress[4] = static_cast<uint8_t>(addrList[4].toUInt());

            //到界面
            addrQLineEditArry[i]->setText(tmp_addr);
            freqQLineEditArry[i]->setText(tmp_freq);

            proData.MDP[i].freq = static_cast<uint32_t>(tmp_freq.remove(".").toInt());
        }

        configIni->endGroup();

        proData.MDP->updatAddressFlag = true;
    }
    delete configIni;

}


/*pushButtonDelAllAddr*/
void MainWindow::on_pushButtonDelAllAddr_clicked()
{
    for(int i = 0;i < 6;i++)
    {
        addrQLineEditArry[i]->setText("0:0:0:0:0");
        freqQLineEditArry[i]->setText("2.400");
        proData.MDP[i].setNewAddr(addrQLineEditArry[i]->text(),freqQLineEditArry[i]->text());
    }

    emit signalSendAllAddr();
}

void MainWindow::writeAddrtoFile()
{
    //如果是有屏幕的,就不用保存地址了
    if(proData.machineType == processingData::haveLcd ||
       proData.machineType == processingData::noType)return;

    QSettings *configIni = new QSettings(QCoreApplication::applicationDirPath() + "/address.ini", QSettings::IniFormat);
    QString tmp_path = "";
    for(int i = 0;i < 6;i++)
    {
        tmp_path = QString().sprintf("NO%d",i);

        configIni->beginGroup(tmp_path);

        QString tmp_addr;
        QString tmp_freq;

        //到界面
        tmp_addr = addrQLineEditArry[i]->text();
        tmp_freq = freqQLineEditArry[i]->text();

        if(tmp_addr != "" && tmp_freq != "")
        {
            configIni->setValue("addr",tmp_addr);
            configIni->setValue("freq",tmp_freq);
        }

        configIni->endGroup();
    }
    delete configIni;
}

bool MainWindow::connectAll()
{
    //数据处理发送数据到传输线程发送数据
    connect(&proData,SIGNAL(signalsSendPack(QByteArray)),&thrCom,SLOT(slotComSendData(QByteArray)));

    //数据处理通知更新地址数据
    connect(&proData,SIGNAL(signalsUpdatUiAddr()),this,SLOT(slotUpdatUiaddr()));

    //通知没有收到包
    connect(&thrCom,SIGNAL(singalNoPack(bool)),this,SLOT(slotNoPack(bool)));

    //串口接收到数据后发送到数据处理线程
    connect(&thrCom,SIGNAL(singalSendProcess(QByteArray)),&proData,SLOT(slotDisposeRawPack(QByteArray)));

    //判断是否打开串口成功
    connect(&thrCom,SIGNAL(singalComOpenOk(bool)),this,SLOT(slotOpenComisOk(bool)));

    //更新界面定时器
    connect(&updatDataToUiQTimer,SIGNAL(timeout()),this,SLOT(slotUpdatDataToUi()));

    connect(&autoMatchGetAddr,SIGNAL(timeout()),this,SLOT(on_pushButtonReadAllAddrToPc_clicked()));

    connect(&batchSetQTimer,SIGNAL(timeout()),this,SLOT(slotbatchSetQTimer()));

    connect(&runWindwoQtime,SIGNAL(timeout()),this,SLOT(slotRunWindowTime()));

    connect(&heartbeatQTimer,SIGNAL(timeout()),this,SLOT(slotheartbeatTime()));

    connect(&delayOutQtimer,SIGNAL(timeout()),this,SLOT(slotDelayOutQtimer()));
//==============================================================================================================
    //打开串口
    connect(this,SIGNAL(signalOpenCom(QString,QString)),&thrCom,SLOT(sloteOpenCom(QString,QString)));
    connect(&swComForm,SIGNAL(signalOpenCom(QString,QString)),&thrCom,SLOT(sloteOpenCom(QString,QString)));
    //关闭串口
    connect(this,SIGNAL(signalCloseCom()),&thrCom,SLOT(slotCloseCom()));
    //确定更新地址数据单个
    connect(this,SIGNAL(signalSendAddr(int)),&proData,SLOT(slotSendAddrToLower(int)));
    //更新地址全部
    connect(this,SIGNAL(signalSendAllAddr()),&proData,SLOT(slotSendAllAddrToLower()));
    //更新电压/电流
    connect(this,SIGNAL(signalSendDefualtVolta(int)),&proData,SLOT(slotSendVoltaToLower(int)));
    connect(this,SIGNAL(signalSendDefualtElect(int)),&proData,SLOT(slotSendElectToLower(int)));
    //开/关自动匹配
    connect(this,SIGNAL(signalSendStartAutoMatch()),&proData,SLOT(slotSendStartAutoMatch()));
    connect(this,SIGNAL(signalSendStopAutoMatch()),&proData,SLOT(slotSendStopAutoMatch()));
    //开关RGB灯闪
    connect(this,SIGNAL(signalSendStartRGB()),&proData,SLOT(slotSendStartRGB()));
    connect(this,SIGNAL(signalSendStopRGB()),&proData,SLOT(slotSendStopRGB()));
    //开关输出
    connect(this,SIGNAL(signalSendSetoutPutState(int)),&proData,SLOT(slotSendSetOutputState(int)));
    //清空波形
    connect(this,SIGNAL(signalSendCleanWave()),&proData,SLOT(slotCleanWave()));
    //暂停波形
    connect(this,SIGNAL(signalSendWaitWave(bool)),&proData,SLOT(slotWaitWave(bool)));
    //获取地址
    connect(this,SIGNAL(signalSendReadAllAddrToPc()),&proData,SLOT(slotSendReadAllAddrToPc()));
    //设置当前通道
    connect(this,SIGNAL(signalSendSetNowCh(char)),&proData,SLOT(slotSendNowCh(char)));
    //重启到DFU
    connect(this,SIGNAL(signalSendToDfu()),&proData,SLOT(slotSendToDfu()));
    //获取机械类型
    connect(this,SIGNAL(signalGetMachineType()),&proData,SLOT(slotGetMachineType()));

    //心跳包
    connect(this,SIGNAL(signalSendHeartBeat()),&proData,SLOT(slotHeartBeat()));

    //axisX->rangeChanged(qreal,qreal); slotWaveRangeChanged

    //下位机更新地址同步到上位机
    connect(&proData,SIGNAL(signalSetUiCh(int)),ui->comboBoxSelectCh,SLOT(setCurrentIndex(int)));

    //下位机告诉上位机切换通道
    connect(&proData,SIGNAL(signalSetChToUi(int)),this,SLOT(slotSetChToUi(int)));

    //下位机告诉上位机机械类型
    connect(&proData,SIGNAL(signalSetMachine()),this,SLOT(slotSetMachineType()));

    //240模块错误
    connect(&proData,SIGNAL(signalErr240ToUi()),this,SLOT(slotErr240()));

    //鼠标在波形上显示数据
    connect(proData.series_V,SIGNAL(hovered(const QPointF &, bool)),this,SLOT(slotShowWaveLableV(const QPointF &, bool)));

    //鼠标在波形上显示数据
    connect(proData.series_I,SIGNAL(hovered(const QPointF &, bool)),this,SLOT(slotShowWaveLableI(const QPointF &, bool)));



    //波形图范围变化
    connect(axisX,SIGNAL(rangeChanged(qreal,qreal)),&proData,SLOT(slotWaveRangeChanged(qreal,qreal)));

    connect(&QButtonGroupDelAddr,SIGNAL(buttonClicked(int)),this,SLOT(slotQButtonGroupDelAddr(int)));
    connect(&QButtonGroupSetAddr,SIGNAL(buttonClicked(int)),this,SLOT(slotQButtonGroupSetAddr(int)));

    connect(&QButtonGroupsetDefaultVolta,SIGNAL(buttonClicked(int)),this,SLOT(slotQButtonGroupSetDefaultVolta(int)));
    connect(&QButtonGroupsetDefaultElect,SIGNAL(buttonClicked(int)),this,SLOT(slotQButtonGroupSetDefaultElect(int)));

    //控制输出输入
    connect(&QButtonGroupSetoutPutState,SIGNAL(buttonClicked(int)),this,SLOT(slotQButtonGroupSetOutputState(int)));


    //菜单栏相关
    //actionUptoDFU
    connect(ui->actionUptoDFU,SIGNAL(triggered(bool)),this,SLOT(slotMenuToDfu(bool)));
    connect(ui->actionScanCom,SIGNAL(triggered(bool)),this,SLOT(slotScanCom(bool)));
    return true;
}

bool MainWindow::scanComList()
{
    const auto infos = QSerialPortInfo::availablePorts();

    QString description, manufacturer, serialNumber;

    for(const QSerialPortInfo &info : infos)
    {
        static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");
        QStringList list;

        description = info.description();

        manufacturer = info.manufacturer();

        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             <<(info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        qDebug() << list;
        //"STMicroel"
        //找到了复合条件的串口
        if(manufacturer.indexOf("STMicroel") != -1)
        {
            //发送信号打开串口
            emit signalOpenCom(info.portName(),info.serialNumber());
            return true;
        }
    }

    return false;
}

void MainWindow::slotDelayOutQtimer()
{
    static uint32_t count = 0;
    static bool lastFlag = false;


    //这个判断是为了判断串口打开之后再进来一次进来的时候才清空标志位,
    //避免有些操作还没完成标志位就已经清空了
    if(lastFlag)
    {
        lastFlag = false;
        delayOutFlag = false;
        count = 0;
        //不用再去检测,关闭定时器
        delayOutQtimer.stop();
        return;
    }

    //如果串口已经打开了,就说明不用检测了 退出
    if(thrCom.comisOpen())
    {
        lastFlag = true;
        return;
    }

    count++;
    //超过了计数值,说明已经到达了最大检测时间
    if(count > delayOutCount)
    {
        //设置为false 说明等不到连接
        count = 0;
        lastFlag = false;
        delayOutFlag = false;
        devIsPull(true);
        delayOutQtimer.stop();
        return;
    }
}

void MainWindow::cleanAllUi()
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, QColor(128,128,128));
    //showQLabelArry[i][0]->setAutoFillBackground(true);
    for(int i = 0;i < 6;i++)
    {
        showQLabelArry[i][1]->setText("0.000V");
        showQLabelArry[i][2]->setText("0.000A");
        showQLabelArry[i][3]->setText("0.000W");
        showQLabelArry[i][4]->setText("0.000V");
        showQLabelArry[i][5]->setText("0.000A");
        showQLabelArry[i][6]->setText("0.0 C");
        showQLabelArry[i][7]->setText("0.000V  0.000A");
        showQLabelArry[i][8]->setText("关");
        showQLabelArry[i][0]->setText(QString(tr("%1# 未设地址")).arg(i + 1));

        for(int j = 0;j < 8;j++)
        {
            showQLabelArry[i][j]->setPalette(pal);
            //showQLabelArry[i][j]->setEnabled(false);
        }

    }

    pal.setColor(QPalette::WindowText, QColor(128,128,128));
    for(int i = 0;i < 6;i++)
    {
        showQLabelArry[i][7]->setPalette(pal);
    }

    //ui->labelNowVolta->setText(tr("电压:0.000V"));
    //ui->labelNowElect->setText(tr("电流:0.000A"));
    ui->labelNowVolta->setText("0.000V");
    ui->labelNowElect->setText("0.000A");
    ui->labelNowTitle->setText(tr("未连接"));


}

void MainWindow::devIsPull(bool sw)
{
    if(sw)
    {
        ui->groupBoxControl->setEnabled(false);
        ui->actionUptoDFU->setEnabled(false);
        ui->actionScanCom->setEnabled(true);
        updatDataToUiQTimer.stop();
        autoMatchGetAddr.stop();
        on_pushButtonCleanWave_clicked();
        proData.now_ch = 0;
        cleanAllUi();
        if(autoMachFlag == true)
        {
            on_pushButtonStartAutoMach_clicked();
        }

       QPalette pal;
       pal.setColor(QPalette::ButtonText, QColor(128,128,128));
       ui->pushButtonStartAutoMach->setPalette(pal);
       proData.machineType = processingData::noType;
       heartbeatQTimer.stop();
       ui->statusBar->showMessage(tr("提示信息:设备已经被拔出"),3000);
    }
    else
    {
        emit signalSendHeartBeat();
        heartbeatQTimer.start(1500);
        QThread::msleep(200);  //不可去掉的延时先心跳激活M02起来
        //readAddrFormFile();
        if(processingData::noLcd == proData.machineType)
        {
            on_pushButtonSetAllAddr_clicked();
        }
        else if(processingData::haveLcd == proData.machineType)
        {
            on_pushButtonReadAllAddrToPc_clicked();
        }
        else if(processingData::noType == proData.machineType)
        {
            slotGetMachineType();
        }


        updatDataToUiQTimer.start(updatDataToUiQTimertim);

        ui->groupBoxControl->setEnabled(true);
        ui->actionScanCom->setEnabled(false);

        ui->actionUptoDFU->setEnabled(true);
        QPalette pal;
        pal.setColor(QPalette::ButtonText, QColor(0,0,0));
        ui->pushButtonStartAutoMach->setPalette(pal);

        pal.setColor(QPalette::WindowText, QColor(0,0,0));

        for(int i = 0;i < 6;i++)
        {
            showQLabelArry[i][7]->setPalette(pal);
        }
        if(!delayOutFlag)
        {
            ui->statusBar->showMessage(tr("提示信息:打开端口成功!成功连接"));
        }

        //on_comboBoxSelectCh_currentIndexChanged(1);
        //on_comboBoxSelectCh_currentIndexChanged(0);
        proData.now_ch = 0;
        QThread::msleep(500);
        emit signalSendSetNowCh(0);

        ui->tabWidgetSetVI->setCurrentIndex(proData.now_ch);
        switch(proData.now_ch)
        {
        case 0:
            ui->widget_ch1->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch1{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 1:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch2{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 2:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch3{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 3:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch4{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 4:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch5{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 5:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch6{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            break;
        default:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
        break;
        }
        ui->comboBoxSelectCh->setCurrentIndex(proData.now_ch);
        showNowCh.setGeometry(139,470,64,64);
        if(!delayOutFlag)
        {
            emit signalSendCleanWave();
        }

    }
}

void MainWindow::UiInit()
{
    uiToArry();

    //====隐藏切换到通道控件======================
    ui->label_6->hide();
    ui->comboBoxSelectCh->hide();
    ui->label_18->hide();
    //====隐藏RGB按钮===========================
    ui->pushButtonRGB->hide();
    //====设置菜单栏=============================
    /*
    ui->menuBar->setStyleSheet("QMenuBar{background-color:rgba(50,50,50,0.7);}"
                               "QMenuBar::selected{background-color:rgba(150,150,150,0.7);}"
                               "QMenuBar::item{font-size:12px;font-family:Microsoft YaHei;color:rgba(255,255,255,0.7);}"
                               "QMenu {background-color:rgb(30,24,47);border:1px solid rgb(82,130,164);}"
                               "QMenu::item {min-width:50px;font-size: 12px;color: rgb(225,225,225);background:rgb(75,120,154);"
                               "border:1px solid rgba(82,130,164,1);padding:1px 1px;margin:1px 1px;}"
                               "QMenu::item:selected {background:rgba(82,130,164,1);border:1px solid rgba(82,130,164,1);}"
                               "QMenu::item:pressed {background:rgba(82,130,164,0.4);border:1px solid rgba(82,130,164,1);}"
                               );
    */
    ui->menuBar->setStyleSheet("QMenu{background-color:rgba(50,50,50,0.7);}"

    "QMenuBar{background:rgba(50,50,50,0.7);color:rgb(200,200,200);}"

    "QMenuBar::item {spacing: 3px;padding: 1px 4px;background: transparent;}"

    "QMenuBar::item:selected {border-style: solid;border-top-color: transparent;border-right-color: transparent;"
    "border-left-color: transparent;border-bottom-color: #e67e22;border-bottom-width: 1px;border-bottom-radius: 6px;"
    "border-style: solid;color: rgb(255,255,255);padding-bottom: 0px;background-color: rgb(30,30,30);}"

    "QMenu::item:selected {border-style: solid;border-top-color: transparent;border-right-color: transparent;"
    "border-left-color: #e67e22;border-bottom-color: transparent;border-left-width: 2px;color: #FFFFFF;"
    "padding-left:15px;padding-top:4px;padding-bottom:4px;padding-right:7px;background-color:#000000;}"

    "QMenu::item {border-style: solid;border-top-color: transparent;border-right-color: transparent;"
    "border-left-color: transparent;border-bottom-color: transparent;border-bottom-width: 1px;border-style: solid;"
    "color: #a9b7c6;padding-left:17px;padding-top:4px;padding-bottom:4px;padding-right:7px;background-color:#000000;}"
    );
    //=========================================

    for(int i = 0;i < 6;i++)
    {
        QButtonGroupDelAddr.addButton(addrDelQPushButtonArry[i],i);
        QButtonGroupSetAddr.addButton(addrSetQPushButtonArry[i],i);

        QButtonGroupsetDefaultVolta.addButton(defaultVoltageQPushButtonArry[i],i);
        QButtonGroupsetDefaultElect.addButton(defaultElectQPushButtonArry[i],i);

        QButtonGroupSetoutPutState.addButton(setOutputStateArry[i],i);
        addrQLineEditArry[i]->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]{1,2}(:[0-9a-fA-F]{1,2}){4}"), this));
        freqQLineEditArry[i]->setValidator(new QRegExpValidator(QRegExp("^2\\.\\d{0,3}$"), this));
    }

    devIsPull(true);

    ui->groupBoxControl->setEnabled(false);
    ui->statusBar->setStyleSheet(QString("color:red;font-size:20px;"));

    ui->statusBar->showMessage(tr("提示信息:软件初始化成功,等待设备插入"));

    //QPalette pal;
    //pal.setColor(QPalette::WindowText, QColor(Qt::red));

    //自动匹配说明窗口
    autoMatchExplain.hide();

    //showNowCh.setParent(this);
    //showNowCh.setAttribute(Qt::WA_TranslucentBackground); //设置为透明

    //254为ASCII表里面的方块
    //showNowCh.setText(QString(static_cast<char>(254)));
    //showNowCh.setPalette(pal);
    //proData.now_ch = 0;
    //showNowCh.setGeometry(139 + proData.now_ch * 150,470,64,64);


    //波形界面初始化
    waveWidgetInit();


    showWaveLabelV.setParent(chartview);

    showWaveLabelV.setStyleSheet("color:rgb(255, 162, 0);background:transparent;font: bold;font-size:19px; ");
    showWaveLabelV.hide();
    showWaveLabelI.setParent(chartview);
    showWaveLabelI.setStyleSheet("color: rgb(85, 85, 255);background:transparent;font: bold;font-size:19px;");
    showWaveLabelI.hide();

    ui->mainToolBar->hide();
    //注册USB插拔事件
    if(!regUSBEvent())
    {
        //MessageBox(reinterpret_cast<HWND>(this->winId()), TEXT("程序注册失败!"), TEXT("程序注册失败!"), MB_OK);
        QMessageBox::critical(this, tr("错误"), tr("软件注册USB失败!"));
        exit(0);
    }


}



void MainWindow::waveWidgetInit()
{
    chart = new QChart();//创建图表

    chart->addSeries(proData.series_V);
    chart->addSeries(proData.series_I);

    chart->legend()->setLabelBrush(QBrush(QColor(210,210,210)));
    chart->legend()->setFont(QFont("思源黑体 CN Normal"));

    axisX   = new QValueAxis;
    axisY_V = new QValueAxis;
    axisY_I = new QValueAxis;

//****************************************************
    QFont waveFont = QFont("思源黑体 CN Medium",12);
    axisX->setRange(0, 4000);
    axisX->setTitleText(tr("时间/ms"));//标题
    axisX->setTitleFont(waveFont);
    axisX->setLabelFormat("%4d"); //标签格式：每个单位保留几位小数
    axisX->setTickCount(5); //主分隔个数
    axisX->setLabelsFont(waveFont);
    axisX->setMinorTickCount(2); //每个单位之间绘制了多少虚网线
    axisX->setLabelsColor(QColor(210,210,210));
    axisX->setTitleBrush(QBrush(QColor(210,210,210)));
//==================================================
    axisY_V->setRange(0, 30);
    axisY_V->setTitleText(tr("电压(V)"));
    axisY_V->setTitleFont(waveFont);
    axisY_V->setLabelFormat("%5.2f"); //标签格式
    axisY_V->setTickCount(3);
    axisY_V->setLabelsFont(waveFont);
    axisY_V->setMinorTickCount(2); //每个单位之间绘制了多少虚网线
//    axisY_V->setGridLinePen(QPen(QColor(85, 85, 255)));
    axisY_V->setLabelsColor(QColor(255, 162,0));
    axisY_V->setTitleBrush(QBrush(QColor(255, 162,0)));

//==================================================
    axisY_I->setRange(0,5);
    axisY_I->setTitleText(tr("电流(A)"));
    axisY_I->setLabelsFont(waveFont);
    axisY_I->setTitleFont(waveFont);
    axisY_I->setLabelFormat("%5.2f"); //标签格式
    axisY_I->setTickCount(3);
    axisY_I->setMinorTickCount(2); //每个单位之间绘制了多少虚网线
    axisY_I->setLabelsFont(QFont("思源黑体 CN Medium",12));

    //axisY_I->setGridLinePen(QPen(QColor(255, 162,0)));

    axisY_I->setTitleBrush(QBrush(QColor(85, 85, 255)));
    axisY_I->setLabelsColor(QColor(85, 85, 255));
//****************************************************

    chart->setAxisX(axisX,proData.series_V);
    chart->setAxisY(axisY_V,proData.series_V);

    chart->addAxis(axisY_I,Qt::AlignRight);
    proData.series_I->attachAxis(axisY_I);
    proData.series_I->attachAxis(axisX);

    chart->setMargins(QMargins(0, 0, 0, 0));//设置内边界全部为0
    chart->setBackgroundRoundness(0);//设置背景区域无圆角
    chart->setBackgroundBrush(QBrush(QColor(30,30,30)));

    chartview = new MyChartView(chart,this);//创建图表显示

    chartview->set_I_axix(axisY_I);

    ui->horizontalLayoutWave->addWidget(chartview);
    ui->widget->setLayout(ui->horizontalLayoutWave);

    chartview->saveGeometry();
}


void MainWindow::slotUpdatDataToUi()
{
    machine *p = proData.MDP;

    if(processingData::noType == proData.machineType)
    {
        slotGetMachineType();
        return ;
    }

    QLabel *lockGroup[6]={ui->label_ch1lock,ui->label_ch2lock,ui->label_ch3lock,ui->label_ch4lock,ui->label_ch5lock,ui->label_ch6lock};

    for(int i = 0;i < 6; i++)
    {
        QPalette pal;
        QString tmpTitle;
        if(p->addrEmpty)
        {

            pal.setColor(QPalette::WindowText, QColor(128,128,128));

            tmpTitle = QString(tr("%1# 未设地址")).arg(i + 1);

            lockGroup[i]->setStyleSheet("");

            for(int j = 0 ;j < 8;j++)
            {
                showQLabelArry[i][j]->setEnabled(false);
            }

            ui->tabWidgetSetVI->setTabEnabled(i,false);

        }
        else
        {
            if(p->onLine)
            {
                tmpTitle = QString("%1# %2 %3").arg(i + 1).arg(proData.MDP[i].machineTypeStr()).arg(proData.MDP[i].outInModeSrt());

                //在线则判断锁
                if(proData.MDP[i].lock == true)
                {
                    //tmpTitle.append(" 🔒");
                    lockGroup[i]->setStyleSheet("image: url(:/ico/ico/lock.png);");
                    defaultVoltageQPushButtonArry[i]->setEnabled(false);
                    defaultElectQPushButtonArry[i]->setEnabled(false);
                    defaultVoltageQDoubleSpinBox[i]->setEnabled(false);
                    defaultElectQDoubleSpinBox[i]->setEnabled(false);
                }
                else
                {
                    //tmpTitle.append(" 🔓");
                    lockGroup[i]->setStyleSheet("image: url(:/ico/ico/unlock.png);");
                    defaultVoltageQPushButtonArry[i]->setEnabled(true);
                    defaultElectQPushButtonArry[i]->setEnabled(true);
                    defaultVoltageQDoubleSpinBox[i]->setEnabled(true);
                    defaultElectQDoubleSpinBox[i]->setEnabled(true);
                }
            }
            else
            {
                //tmpTitle.sprintf("%d# offline",i + 1);
                tmpTitle = QString(tr("%1# 离线")).arg(i + 1);
                lockGroup[i]->setStyleSheet("");
                pal.setColor(QPalette::WindowText, QColor(0,0,0));
                for(int j = 0 ;j < 8;j++)
                {
                    //showQLabelArry[i][j]->setPalette(pal);;
                    showQLabelArry[i][j]->setEnabled(true);
                }
            }

            ui->tabWidgetSetVI->setTabEnabled(i,true);
        }

        showQLabelArry[i][0]->setText(tmpTitle);
        //showQLabelArry[i][1]->setText(QString(tr("输出电压:%1V")).arg(p->outPutVoltageStr()));
        //showQLabelArry[i][2]->setText(QString(tr("输出电流:%1A")).arg(p->outPutCurrentStr()));
        //showQLabelArry[i][3]->setText(QString(tr("输出功率:%1W")).arg(p->outPutPowerStr()));
        //showQLabelArry[i][4]->setText(QString(tr("输入电压:%1V")).arg(p->inPutVoltageStr()));
        //showQLabelArry[i][5]->setText(QString(tr("输入电流:%1A")).arg(p->inPutCurrentStr()));
        //showQLabelArry[i][6]->setText(QString(tr("温度:%1 C")).arg(p->tempStr()));
        //showQLabelArry[i][7]->setText(QString(tr("当前预设: 电压(V):%1V 电流(A):%2A")).arg(p->setPutVoltageStr())   \
                                                                                .arg(p->setPutCurrentStr()));
        //showQLabelArry[i][8]->setText(QString(tr("当前输出状态:%1")).arg((p->outPutState)?tr("开"):tr("关")));

        showQLabelArry[i][1]->setText(QString("%1V").arg(p->outPutVoltageStr()));
        showQLabelArry[i][2]->setText(QString("%1A").arg(p->outPutCurrentStr()));
        showQLabelArry[i][3]->setText(QString("%1W").arg(p->outPutPowerStr()));
        showQLabelArry[i][4]->setText(QString("%1V").arg(p->inPutVoltageStr()));
        showQLabelArry[i][5]->setText(QString("%1A").arg(p->inPutCurrentStr()));
        showQLabelArry[i][6]->setText(QString("%1 C").arg(p->tempStr()));
        showQLabelArry[i][7]->setText(QString("%1V     %2A").arg(p->setPutVoltageStr()).arg(p->setPutCurrentStr()));
        showQLabelArry[i][8]->setText(QString("%1").arg((p->outPutState)?tr("开"):tr("关")));


        setOutputStateArry[i]->setText(QString(tr("设置为%1")).arg((!(p->outPutState))?tr("开"):tr("关")));


        //如果标志位为真,表示更新地址
        if(p->addressFlag && !(p->updatAddressFlag))
        {
            addrQLineEditArry[i]->setText(p->getAddrStr());
            freqQLineEditArry[i]->setText(p->getFreqStr());
            p->addressFlag = false;
        }

        if(proData.now_ch == i)
        {
            ui->labelNowTitle->setText(tmpTitle);
            //ui->labelNowVolta->setText(tr("电压:") + p->outPutVoltageStr() + "V");
            //ui->labelNowElect->setText(tr("电流:") + p->outPutCurrentStr() + "A");
            ui->labelNowVolta->setText(p->outPutVoltageStr() + "V");
            ui->labelNowElect->setText(p->outPutCurrentStr() + "A");
        }

//===================================================================================
        //if(p->colorUpdatFlag == true)
        {

            QPalette pal;


//            showQLabelArry[i][0]->setAutoFillBackground(true);
            for(int j = 0;j < 8;j++)
            {
                if(proData.MDP[i].addrEmpty)
                {
                    pal.setColor(QPalette::WindowText, QColor(128,128,128));
                }
                else
                {
                    pal.setColor(QPalette::WindowText, p->color);
                }


                showQLabelArry[i][j]->setPalette(pal);
            }

            p->colorUpdatFlag = false;
        }

        //鉴别905和906最大电流值
        {
           if(proData.MDP[i].machineType == machine::P906)
           {
               if(defaultElectQDoubleSpinBox[i]->maximum() != 10.0)
               {
                   defaultElectQDoubleSpinBox[i]->setMaximum(10.0);
               }

           }
           else if(proData.MDP[i].machineType == machine::P905)
           {
               if(defaultElectQDoubleSpinBox[i]->maximum() != 5.0)
               {
                   defaultElectQDoubleSpinBox[i]->setMaximum(5.0);
               }

           }
           else
           {
               if(defaultElectQDoubleSpinBox[i]->maximum() != 5.0)
               {
                   defaultElectQDoubleSpinBox[i]->setMaximum(5.0);
               }
           }
        }

        p++;
    }


    if(proData.MDP[proData.now_ch].onLineUpdatFlag == true && proData.MDP[proData.now_ch].onLine == false)
    {
        on_pushButtonCleanWave_clicked();
        proData.MDP[proData.now_ch].onLineUpdatFlag = false;
    }


}

void MainWindow::slotQButtonGroupDelAddr(int ch)
{
    //说明没有改变
    addrQLineEditArry[ch]->setText("0:0:0:0:0");
    freqQLineEditArry[ch]->setText("2.400");

    slotQButtonGroupSetAddr(ch);
}

void MainWindow::slotQButtonGroupSetAddr(int ch)
{
    //说明没有改变
//    if(false == addrQLineEditArry[ch]->isModified() &&
//       false == freqQLineEditArry[ch]->isModified() )return;

    proData.MDP[ch].setNewAddr(addrQLineEditArry[ch]->text(),freqQLineEditArry[ch]->text());

    emit signalSendAddr(ch);

    writeAddrtoFile();
    emit signalSendAddr(ch);
    ui->statusBar->showMessage(QString(tr("提示信息:%1号机地址更新成功!")).arg(ch + 1),3000);
}
//自写个四舍五入函数
//其中floor()是向下取整函数
uint16_t MainWindow::doubleToInt(double d)
{
    double intPart = floor(d);
    if ((d - intPart) >= static_cast<double>(0.5))
    {
        return (static_cast<uint16_t>(intPart + 1.0));
    }
    else
    {
        return static_cast<uint16_t>(intPart);
    }
}

void MainWindow::slotSetChToUi(int index)
{
    //if()
    on_comboBoxSelectCh_currentIndexChanged(index);
}

void MainWindow::slotErr240()
{
    ui->statusBar->showMessage(tr("警告:设备无线模块错误,请检查设备!"));
}
void MainWindow::slotQButtonGroupSetDefaultVolta(int ch)
{

    double tmp_V,tmp_I;
    tmp_V = defaultVoltageQDoubleSpinBox[ch]->value();
    tmp_I = defaultElectQDoubleSpinBox[ch]->value();

    double tmp_max_p = 0.00000001;
    if(machine::P905 == proData.MDP[ch].machineType)
    {
        tmp_max_p = 90; //限制90W
    }
    else if(machine::P906 == proData.MDP[ch].machineType)
    {
        tmp_max_p = 300;  //限制300W
    }
    else
    {
        return;
    }

    if(tmp_V * tmp_I > tmp_max_p)
    {
        tmp_V = tmp_max_p / tmp_I;  //设置为电压最大了
        defaultVoltageQDoubleSpinBox[ch]->setValue(tmp_V);
        ui->statusBar->showMessage(QString(tr("提示信息:超过限定功率,已经为你智能设置到%1V").arg(tmp_V)),3000);
    }


    tmp_V = tmp_V * 1000;
    tmp_I = tmp_I * 1000;

    uint16_t tmp_16_V,tmp_16_I;

    tmp_16_V = doubleToInt(tmp_V);
    tmp_16_I = doubleToInt(tmp_I);

    proData.MDP[ch].updatSetPutVoltage = tmp_16_V;
    proData.MDP[ch].updatSetPutCurrent = tmp_16_I;
    proData.MDP[ch].updatSetPutFlag = true;


    emit signalSendDefualtVolta(ch);
    emit signalSendDefualtVolta(ch);
//    emit signalSendDefualtVolta(ch);

    //QThread::msleep(30);
//    emit signalSendDefualtVolta(ch);
}

void MainWindow::slotQButtonGroupSetDefaultElect(int ch)
{

    double tmp_V,tmp_I;
    tmp_V = defaultVoltageQDoubleSpinBox[ch]->value();
    tmp_I = defaultElectQDoubleSpinBox[ch]->value();

    double tmp_max_p = 0.00000001;
    if(machine::P905 == proData.MDP[ch].machineType)
    {
        tmp_max_p = 90; //限制90W
    }
    else if(machine::P906 == proData.MDP[ch].machineType)
    {
        tmp_max_p = 300;  //限制300W
    }
    else
    {
        return;
    }

    if(tmp_V * tmp_I > tmp_max_p)
    {
        tmp_I = tmp_max_p / tmp_V;  //设置为电流最大了
        defaultElectQDoubleSpinBox[ch]->setValue(tmp_I);
        ui->statusBar->showMessage(QString(tr("提示信息:超过限定功率,已经为你智能设置到%1A").arg(tmp_I)),3000);
    }

    tmp_I = tmp_I * 1000.0;
    tmp_V = tmp_V * 1000.0;

    uint16_t tmp_16_V,tmp_16_I;
//    tmp_16_V = static_cast<uint16_t>(tmp_V);
//    tmp_16_I = static_cast<uint16_t>(tmp_I);

    tmp_16_V = doubleToInt(tmp_V);
    tmp_16_I = doubleToInt(tmp_I);

    proData.MDP[ch].updatSetPutVoltage = tmp_16_V;
    proData.MDP[ch].updatSetPutCurrent = tmp_16_I;
    proData.MDP[ch].updatSetPutFlag = true;

    emit signalSendDefualtElect(ch);
    emit signalSendDefualtElect(ch);
//    emit signalSendDefualtElect(ch);
    //QThread::msleep(30);
//    emit signalSendDefualtElect(ch);
}

void MainWindow::slotQButtonGroupSetOutputState(int ch)
{
    //如果为真则表示当前状态为开,所以设置为关
    if(proData.MDP[ch].outPutState)
    {
        proData.MDP[ch].updatoutPutState = false;
    }
    else
    {
        proData.MDP[ch].updatoutPutState = true;
    }

    proData.MDP[ch].updatoutPutStateFlag = true;

    emit signalSendSetoutPutState(ch);
}

void MainWindow::slotOpenComisOk(bool ok)
{
    if(ok)
    {

        swComForm.hide();
        //连接成功开始刷新界面
        //if(!delayOutFlag)
        {
            devIsPull(false);
        }
        qDebug() << "open Com success!";
    }
}

void MainWindow::slotUpdatUiaddr()
{
    machine *p = proData.MDP;
    for(int i = 0;i < 6;i++)
    {
        if(p->addressFlag || p->updatAddressFlag)
        {
            addrQLineEditArry[i]->setText(p->getAddrStr());
            freqQLineEditArry[i]->setText(p->getFreqStr());
            p->addressFlag = false;
        }
        p++;
    }
}

void MainWindow::slotbatchSetQTimer()
{
    static int count = 0;
    static bool setChFlag = false;
    static int lastCh = 0;
    ui->pushButtonOpenAll->setEnabled(false);
    ui->pushButtonCloseAll->setEnabled(false);
    ui->pushButtonSetAllV->setEnabled(false);
    ui->pushButtonSetAllI->setEnabled(false);



    //有没有切换到该通道
    if(!setChFlag )
    {
        if(count == 0)
        {
            //记录下当前通道,批量设置完成切换回去
            lastCh = proData.now_ch;
        }

        if(proData.MDP[count % 6].onLine == true)
        {
            proData.changeChannelCount = 20;
            on_comboBoxSelectCh_currentIndexChanged(count);
            setChFlag = true;
        }
        else
        {

            goto count_out;
        }
        return;
    }

    if(atchSetOut)
    {
        ui->statusBar->showMessage(tr("提示信息:正在批量打开输出,请稍后...."));
        if(proData.MDP[count % 6].onLine == true && proData.MDP[count % 6].outPutState == false)
        {
            proData.MDP[count % 6].updatoutPutState = true;
            proData.MDP[count % 6].updatoutPutStateFlag = true;
            emit signalSendSetoutPutState(count % 6);
        }
    }
    else if(atchSetClose)
    {
        ui->statusBar->showMessage(tr("提示信息:正在批量关闭输出,请稍后...."));
        if(proData.MDP[count % 6].onLine == true && proData.MDP[count % 6].outPutState == true)
        {

            proData.MDP[count % 6].updatoutPutState = false;
            proData.MDP[count % 6].updatoutPutStateFlag = true;
            emit signalSendSetoutPutState(count % 6);
        }

    }
    else if(atchSetV)
    {
        ui->statusBar->showMessage(tr("提示信息:正在批量设置电压,请稍后...."));
        if(proData.MDP[count % 6].onLine == true)
        {

            double tmp_V,tmp_I;
            tmp_V = ui->doubleSpinBoxSetALLV->value() * 1000.0;
            tmp_I = ui->doubleSpinBoxSetALLI->value() * 1000.0;

            uint16_t tmp_16_V,tmp_16_I;
            tmp_16_V = static_cast<uint16_t>(tmp_V);
            tmp_16_I = static_cast<uint16_t>(tmp_I);

            proData.MDP[count % 6].updatSetPutVoltage = tmp_16_V;
            proData.MDP[count % 6].updatSetPutCurrent = tmp_16_I;
            proData.MDP[count % 6].updatSetPutFlag = true;

            emit signalSendDefualtVolta(count % 6);
        }
    }
    else if(atchSetI)
    {
        ui->statusBar->showMessage(tr("提示信息:正在批量设置电流,请稍后...."));
        if(proData.MDP[count % 6].onLine == true)
        {
            double tmp_V,tmp_I;
            tmp_V = ui->doubleSpinBoxSetALLV->value() * 1000.0;
            tmp_I = ui->doubleSpinBoxSetALLI->value() * 1000.0;

            uint16_t tmp_16_V,tmp_16_I;
            tmp_16_V = static_cast<uint16_t>(tmp_V);
            tmp_16_I = static_cast<uint16_t>(tmp_I);

            proData.MDP[count % 6].updatSetPutVoltage = tmp_16_V;
            proData.MDP[count % 6].updatSetPutCurrent = tmp_16_I;
            proData.MDP[count % 6].updatSetPutFlag = true;

            emit signalSendDefualtElect(count % 6);
        }
    }

count_out:
    count++;
    setChFlag = false;
    if(count >= 6)
    {
        QThread::msleep(300); //该延时避免批量设置完成后,立刻切换通道无法切换成功
        on_comboBoxSelectCh_currentIndexChanged(lastCh);

        setChFlag = false;
        count = 0;
        atchSetV = false;
        atchSetI = false;
        atchSetOut = false;
        atchSetClose = false;


        ui->pushButtonOpenAll->setEnabled(true);
        ui->pushButtonCloseAll->setEnabled(true);
        ui->pushButtonSetAllV->setEnabled(true);
        ui->pushButtonSetAllI->setEnabled(true);
        ui->statusBar->showMessage(tr("提示信息:批量操作完成!"),3000);
        batchSetQTimer.stop();
        QThread::msleep(100); //该延时避免批量设置完成后,立刻切换通道无法切换成功
        on_comboBoxSelectCh_currentIndexChanged(lastCh);
        QThread::msleep(100); //该延时避免批量设置完成后,立刻切换通道无法切换成功
        on_comboBoxSelectCh_currentIndexChanged(lastCh);
        lastCh = 0;
    }
    //qDebug() << 123;

}

void MainWindow::slotNoPack(bool ok)
{
    if(ok)
    {
        ui->statusBar->showMessage(tr("提示信息:端口已经成功打开!但是没有接收到数据,请检查设备是否正常,重新插拔设备"));
        //cleanAllUi();
        devIsPull(true);
    }
}

void MainWindow::slotMenuToDfu(bool togger)
{
    Q_UNUSED(togger);
    QMessageBox::StandardButton tmp = QMessageBox::warning(this, tr("警告"), tr("是否进入DFU更新固件模式"), QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(tmp == QMessageBox::Yes)
    {
        emit signalSendToDfu();
    }

}

void MainWindow::slotScanCom(bool)
{
    ui->statusBar->showMessage(tr("提示信息:正在重新扫描端口"));
    if(scanComList())
    {
        ui->statusBar->showMessage(tr("提示信息:扫描端口完成,发现设备正在连接."),3000);
    }
    else
    {
        ui->statusBar->showMessage(tr("提示信息:扫描端口完成,没有发现设备."),3000);
    }

}

void MainWindow::slotShowWaveLableV(const QPointF &point, bool state)
{
    if (state)
    {
        if(point.y() < 0.00000001)
        {
            showWaveLabelV.setText(QString().sprintf("%.3fV", 0.0000001));
        }
        else
        {
             showWaveLabelV.setText(QString().sprintf("%.3fV", point.y()));
             qDebug() << point;
        }

        QPoint curPos = mapFromGlobal(QCursor::pos());
        showWaveLabelV.move(curPos.x() , curPos.y()  );//移动数值  - showWaveLabelV.height()
        showWaveLabelV.raise();
        showWaveLabelV.show();//显示出来
    }
    else
    {
         showWaveLabelV.hide();//进行隐藏
    }
}

void MainWindow::slotShowWaveLableI(const QPointF &point, bool state)
{
    if (state)
    {
        if(point.y() < 0.00000001)
        {
            showWaveLabelI.setText(QString().sprintf("%.3fA", 0.0000001));
        }
        else
        {
             showWaveLabelI.setText(QString().sprintf("%.3fA", point.y()));
        }

        QPoint curPos = mapFromGlobal(QCursor::pos());
        showWaveLabelI.move(curPos.x() , curPos.y()  );//移动数值  - showWaveLabelI.height()
        showWaveLabelI.raise();
        showWaveLabelI.show();//显示出来
    }
    else
    {
         showWaveLabelI.hide();//进行隐藏
    }
}

void MainWindow::slotGetMachineType()
{
    emit signalGetMachineType();
}

void MainWindow::slotSetMachineType()
{
    if(processingData::noLcd == proData.machineType)
    {
        if(!delayOutFlag)
        {

            ui->statusBar->showMessage(tr("提示信息:设备为M02正在设置地址"),3000);

        }

        ui->actionUptoDFU->setEnabled(true);
        on_pushButtonSetAllAddr_clicked();

    }
    else if(processingData::haveLcd == proData.machineType)
    {
        if(!delayOutFlag)
        {
            ui->statusBar->showMessage(tr("提示信息:设备为M01正在读取地址"),3000);

        }

        ui->actionUptoDFU->setEnabled(false);
        on_pushButtonReadAllAddrToPc_clicked();
    }
    else if(processingData::noType == proData.machineType)
    {
        slotGetMachineType();
    }

}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);

    MSG* msg = reinterpret_cast<MSG*>(message);//第一层解算
    UINT msgType = msg->message;
    if(msgType==WM_DEVICECHANGE)
    {
        PDEV_BROADCAST_HDR lpdb = PDEV_BROADCAST_HDR(msg->lParam);//第二层解算
        switch (msg->wParam) {
        case DBT_DEVICEARRIVAL:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = PDEV_BROADCAST_DEVICEINTERFACE(lpdb);
                QString device_name = "插入设备(name)：" + QString::fromWCharArray(pDevInf->dbcc_name,int(pDevInf->dbcc_size)).toUtf8();
                qDebug()<< device_name;

                if(thrCom.comisOpen())break;
                //不可缺少延时,M01插入usb后,可能无法快速搜索到串口,所以先延时,再去扫描设备
                QThread::msleep(400);
                scanComList();

            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = PDEV_BROADCAST_DEVICEINTERFACE(lpdb);
                QString msg = QString::fromWCharArray(pDevInf->dbcc_name,int(pDevInf->dbcc_size)).toUtf8();
                QString device_name = "移除设备(name)：" + msg;
                qDebug() << device_name;


                if(thrCom.comisOpen() && thrCom.getComID() != "" && msg.indexOf(thrCom.getComID()) != -1)
                {
                    proData.machineType = processingData::noType;
                    emit signalCloseCom();
                    delayOutFlag = true;
                    delayOutQtimer.start(delayOutTime);

                    //devIsPull(true);

                    //qDebug() << "拔出串口";

                }
            }
            break;
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    writeAddrtoFile();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QMainWindow::paintEvent(e);

//    QPainter slicer(this);
//    slicer.setPen(Qt::red);

    //slicer.drawLine(0,0, 300,300);

//    slicer.drawText(139 + proData.now_ch * 150,490,"🔴");
//    qDebug() << 123;
}

void MainWindow::on_pushButtonStartAutoMach_clicked()
{
     QPalette pal;
    //为false表示还没按下
    if(false == autoMachFlag)
    {
        emit signalSendStartAutoMatch();
        ui->pushButtonStartAutoMach->setText(tr("关闭手动匹配"));
        ui->statusBar->showMessage(tr("提示信息:请在P905上操作进行地址匹配!           请关闭该功能才进行其他操作"));
        autoMachUiSet(true);
        //ui->pushButtonStartAutoMach->setStyleSheet("color: red");

        updatDataToUiQTimer.stop();
        pal.setColor(QPalette::ButtonText, QColor(Qt::red));
        emit signalSendStartAutoMatch();

        QMessageBox::information(this, tr("信息"), tr("进入手动匹配模式!请在P905上操作进行匹配! \r\n\r\n"
                                                      "匹配地址完成后,请关闭该功能,才进行其他操作!"));
        emit signalSendStartAutoMatch();


        autoMatchGetAddr.start(500);
    }
    else
    {
        emit signalSendStopAutoMatch();
        ui->pushButtonStartAutoMach->setText(tr("开启手动匹配"));
        ui->statusBar->showMessage(tr("提示信息:手动匹配已经关闭"),3000);
        pal.setColor(QPalette::ButtonText, QColor(Qt::black));
        autoMachUiSet(false);

        autoMatchGetAddr.stop();
        updatDataToUiQTimer.start(updatDataToUiQTimertim);


        machine *p = proData.MDP;
        for(int i = 0;i < 6;i++)
        {
            p->upDatAddress[0] = p->address[4];
            p->upDatAddress[1] = p->address[3];
            p->upDatAddress[2] = p->address[2];
            p->upDatAddress[3] = p->address[1];
            p->upDatAddress[4] = p->address[0];
            p->upDatFreq = p->freq;
            p++;
        }

        emit signalSendStopAutoMatch();
    }

    writeAddrtoFile();
    ui->pushButtonStartAutoMach->setPalette(pal);
    autoMachFlag = !autoMachFlag;



}

void MainWindow::on_pushButtonSetAllAddr_clicked()
{
    for(int i = 0;i < 6;i++)
    {
        proData.MDP[i].setNewAddr(addrQLineEditArry[i]->text(),freqQLineEditArry[i]->text());
    }

    emit signalSendAllAddr();
}

void MainWindow::on_pushButtonRGB_clicked()
{

    if(false == RGBFlag)
    {
        ui->pushButtonRGB->setText(tr("关闭RGB闪灯"));
        ui->statusBar->showMessage(tr("提示信息:已经开启RGB闪灯,如果没有效果请重新操作!"));
        emit signalSendStartRGB();
    }
    else
    {
        ui->pushButtonRGB->setText(tr("打开RGB闪灯"));
        ui->statusBar->showMessage(tr("提示信息:已经关闭RGB闪灯,如果没有效果请重新操作!"));
        emit signalSendStopRGB();
    }

    RGBFlag = !RGBFlag;

}

void MainWindow::on_pushButtonReadAllAddrToPc_clicked()
{
   emit signalSendReadAllAddrToPc();
//   ui->statusBar->showMessage(tr("提示信息:读取地址完成!"),2000);
}



void MainWindow::on_pushButtonCleanWave_clicked()
{
    emit signalSendCleanWave();
//    ui->statusBar->showMessage(tr("提示信息:清空波形完成!"),3000);
}

void MainWindow::uiToArry()
{
    //地址编辑框
    addrQLineEditArry[0] = ui->lineEditAddr_1;
    addrQLineEditArry[1] = ui->lineEditAddr_2;
    addrQLineEditArry[2] = ui->lineEditAddr_3;
    addrQLineEditArry[3] = ui->lineEditAddr_4;
    addrQLineEditArry[4] = ui->lineEditAddr_5;
    addrQLineEditArry[5] = ui->lineEditAddr_6;
    //频率编辑框
    freqQLineEditArry[0] = ui->lineEditFreq_1;
    freqQLineEditArry[1] = ui->lineEditFreq_2;
    freqQLineEditArry[2] = ui->lineEditFreq_3;
    freqQLineEditArry[3] = ui->lineEditFreq_4;
    freqQLineEditArry[4] = ui->lineEditFreq_5;
    freqQLineEditArry[5] = ui->lineEditFreq_6;
    //展示数据指针
    showQLabelArry[0][0] =        ui->labelShowNum_1;
    showQLabelArry[0][1] =      ui->labelShowVolat_1;
    showQLabelArry[0][2] =      ui->labelShowElect_1;
    showQLabelArry[0][3] =      ui->labelShowPower_1;
    showQLabelArry[0][4] = ui->labelShowInputVolat_1;
    showQLabelArry[0][5] = ui->labelShowInputElect_1;
    showQLabelArry[0][6] =       ui->labelShowtemp_1;
    showQLabelArry[0][7] =        ui->labelDefault_1;
    showQLabelArry[0][8] =    ui->labelOutputState_1;

    showQLabelArry[1][0] =        ui->labelShowNum_2;
    showQLabelArry[1][1] =      ui->labelShowVolat_2;
    showQLabelArry[1][2] =      ui->labelShowElect_2;
    showQLabelArry[1][3] =      ui->labelShowPower_2;
    showQLabelArry[1][4] = ui->labelShowInputVolat_2;
    showQLabelArry[1][5] = ui->labelShowInputElect_2;
    showQLabelArry[1][6] =       ui->labelShowtemp_2;
    showQLabelArry[1][7] =        ui->labelDefault_2;
    showQLabelArry[1][8] =    ui->labelOutputState_2;

    showQLabelArry[2][0] =        ui->labelShowNum_3;
    showQLabelArry[2][1] =      ui->labelShowVolat_3;
    showQLabelArry[2][2] =      ui->labelShowElect_3;
    showQLabelArry[2][3] =      ui->labelShowPower_3;
    showQLabelArry[2][4] = ui->labelShowInputVolat_3;
    showQLabelArry[2][5] = ui->labelShowInputElect_3;
    showQLabelArry[2][6] =       ui->labelShowtemp_3;
    showQLabelArry[2][7] =        ui->labelDefault_3;
    showQLabelArry[2][8] =    ui->labelOutputState_3;

    showQLabelArry[3][0] =        ui->labelShowNum_4;
    showQLabelArry[3][1] =      ui->labelShowVolat_4;
    showQLabelArry[3][2] =      ui->labelShowElect_4;
    showQLabelArry[3][3] =      ui->labelShowPower_4;
    showQLabelArry[3][4] = ui->labelShowInputVolat_4;
    showQLabelArry[3][5] = ui->labelShowInputElect_4;
    showQLabelArry[3][6] =       ui->labelShowtemp_4;
    showQLabelArry[3][7] =        ui->labelDefault_4;
    showQLabelArry[3][8] =    ui->labelOutputState_4;

    showQLabelArry[4][0] =        ui->labelShowNum_5;
    showQLabelArry[4][1] =      ui->labelShowVolat_5;
    showQLabelArry[4][2] =      ui->labelShowElect_5;
    showQLabelArry[4][3] =      ui->labelShowPower_5;
    showQLabelArry[4][4] = ui->labelShowInputVolat_5;
    showQLabelArry[4][5] = ui->labelShowInputElect_5;
    showQLabelArry[4][6] =       ui->labelShowtemp_5;
    showQLabelArry[4][7] =        ui->labelDefault_5;
    showQLabelArry[4][8] =    ui->labelOutputState_5;

    showQLabelArry[5][0] =        ui->labelShowNum_6;
    showQLabelArry[5][1] =      ui->labelShowVolat_6;
    showQLabelArry[5][2] =      ui->labelShowElect_6;
    showQLabelArry[5][3] =      ui->labelShowPower_6;
    showQLabelArry[5][4] = ui->labelShowInputVolat_6;
    showQLabelArry[5][5] = ui->labelShowInputElect_6;
    showQLabelArry[5][6] =       ui->labelShowtemp_6;
    showQLabelArry[5][7] =        ui->labelDefault_6;
    showQLabelArry[5][8] =    ui->labelOutputState_6;

    addrDelQPushButtonArry[0] = ui->pushButtonDelAddr_1;
    addrDelQPushButtonArry[1] = ui->pushButtonDelAddr_2;
    addrDelQPushButtonArry[2] = ui->pushButtonDelAddr_3;
    addrDelQPushButtonArry[3] = ui->pushButtonDelAddr_4;
    addrDelQPushButtonArry[4] = ui->pushButtonDelAddr_5;
    addrDelQPushButtonArry[5] = ui->pushButtonDelAddr_6;

    addrSetQPushButtonArry[0] = ui->pushButtonSetAddr_1;
    addrSetQPushButtonArry[1] = ui->pushButtonSetAddr_2;
    addrSetQPushButtonArry[2] = ui->pushButtonSetAddr_3;
    addrSetQPushButtonArry[3] = ui->pushButtonSetAddr_4;
    addrSetQPushButtonArry[4] = ui->pushButtonSetAddr_5;
    addrSetQPushButtonArry[5] = ui->pushButtonSetAddr_6;


    defaultVoltageQDoubleSpinBox[0] = ui->doubleSpinBoxSetDefautVoltage_1;
    defaultVoltageQDoubleSpinBox[1] = ui->doubleSpinBoxSetDefautVoltage_2;
    defaultVoltageQDoubleSpinBox[2] = ui->doubleSpinBoxSetDefautVoltage_3;
    defaultVoltageQDoubleSpinBox[3] = ui->doubleSpinBoxSetDefautVoltage_4;
    defaultVoltageQDoubleSpinBox[4] = ui->doubleSpinBoxSetDefautVoltage_5;
    defaultVoltageQDoubleSpinBox[5] = ui->doubleSpinBoxSetDefautVoltage_6;

    defaultElectQDoubleSpinBox[0] = ui->doubleSpinBoxSetDefautElect_1;
    defaultElectQDoubleSpinBox[1] = ui->doubleSpinBoxSetDefautElect_2;
    defaultElectQDoubleSpinBox[2] = ui->doubleSpinBoxSetDefautElect_3;
    defaultElectQDoubleSpinBox[3] = ui->doubleSpinBoxSetDefautElect_4;
    defaultElectQDoubleSpinBox[4] = ui->doubleSpinBoxSetDefautElect_5;
    defaultElectQDoubleSpinBox[5] = ui->doubleSpinBoxSetDefautElect_6;


    defaultVoltageQPushButtonArry[0] = ui->pushButtonSetVoltageDefault_1;
    defaultVoltageQPushButtonArry[1] = ui->pushButtonSetVoltageDefault_2;
    defaultVoltageQPushButtonArry[2] = ui->pushButtonSetVoltageDefault_3;
    defaultVoltageQPushButtonArry[3] = ui->pushButtonSetVoltageDefault_4;
    defaultVoltageQPushButtonArry[4] = ui->pushButtonSetVoltageDefault_5;
    defaultVoltageQPushButtonArry[5] = ui->pushButtonSetVoltageDefault_6;

    defaultElectQPushButtonArry[0] = ui->pushButtonSetElectDefault_1;
    defaultElectQPushButtonArry[1] = ui->pushButtonSetElectDefault_2;
    defaultElectQPushButtonArry[2] = ui->pushButtonSetElectDefault_3;
    defaultElectQPushButtonArry[3] = ui->pushButtonSetElectDefault_4;
    defaultElectQPushButtonArry[4] = ui->pushButtonSetElectDefault_5;
    defaultElectQPushButtonArry[5] = ui->pushButtonSetElectDefault_6;

    setOutputStateArry[0] = ui->pushButtonSetOutputState_1;
    setOutputStateArry[1] = ui->pushButtonSetOutputState_2;
    setOutputStateArry[2] = ui->pushButtonSetOutputState_3;
    setOutputStateArry[3] = ui->pushButtonSetOutputState_4;
    setOutputStateArry[4] = ui->pushButtonSetOutputState_5;
    setOutputStateArry[5] = ui->pushButtonSetOutputState_6;

}


void MainWindow::on_pushButtonSaveWave_clicked()
{
    QPixmap p = QPixmap::grabWidget(chartview);
    QImage image=p.toImage();
    QString fileName = QFileDialog::getSaveFileName(this,
            tr(""),
            "",
            tr("(*.jpeg)"));

    if (!fileName.isNull())
    {

        image.save(fileName);
    }
}

void MainWindow::on_pushButtonWaitWave_clicked()
{
    QPalette pal;
    if(!proData.waitWaveFlag)
    {
        ui->pushButtonWaitWave->setText(tr("恢复运行"));
        pal.setColor(QPalette::ButtonText, QColor(Qt::red));
    }
    else
    {
        ui->pushButtonWaitWave->setText(tr("暂停波形"));
        pal.setColor(QPalette::ButtonText, QColor(Qt::black));
    }

    ui->pushButtonWaitWave->setPalette(pal);
   emit signalSendWaitWave(!proData.waitWaveFlag);
}

void MainWindow::on_comboBoxSelectCh_currentIndexChanged(int index)
{
    if(index < 0 || index >= 6)return;
    if(!(proData.MDP[index].addrEmpty))
    {
        //设置数值,那边发送10次包才会进行切换,避免界面鬼畜
        proData.changeChannelCount = 10;
        emit signalSendSetNowCh(static_cast<char>(index));
        on_pushButtonCleanWave_clicked();
        ui->tabWidgetSetVI->setCurrentIndex(index);
        proData.now_ch = index;
        int tmp = 139 + proData.now_ch * 145;
        //qDebug() << proData.now_ch;
        tmp = (tmp < 139)?139:tmp;
        showNowCh.setGeometry(tmp,470,64,64);
        //ui->statusBar->showMessage(QString(tr("提示信息:当前选中%1号机")).arg(index + 1),3000);
    }
    else
    {
        ui->comboBoxSelectCh->setCurrentIndex(proData.now_ch);
        //ui->statusBar->showMessage(QString(tr("提示信息:%1号机通道地址信息为空,无法切换")).arg(index + 1),3000);
    }

}




void MainWindow::on_pushButtonOpenAll_clicked()
{
    batchSetQTimer.start(batchSetQTimertime);
    atchSetOut = true;
}

void MainWindow::on_pushButtonCloseAll_clicked()
{
    batchSetQTimer.start(batchSetQTimertime);
    atchSetClose = true;
}

void MainWindow::on_pushButtonSetAllV_clicked()
{
    atchSetV = true;
    batchSetQTimer.start(batchSetQTimertime);
}

void MainWindow::on_pushButtonSetAllI_clicked()
{
    atchSetI = true;
    batchSetQTimer.start(batchSetQTimertime);
}

void MainWindow::on_tabWidgetSetVI_currentChanged(int index)
{
    if(proData.MDP[index].addrEmpty)
    {
        ui->tabWidgetSetVI->setCurrentIndex(proData.now_ch);
//        ui->statusBar->showMessage(QString(tr("提示信息:%1号机通道地址信息为空,无法切换")).arg(index + 1),3000);
    }
    else
    {
        switch(index)
        {
        case 0:
            ui->widget_ch1->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch1{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 1:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch2{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 2:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch3{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 3:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch4{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 4:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch5{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            ui->widget_ch6->setStyleSheet("");
            break;
        case 5:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("QLabel{background-color: rgb(0, 50, 0);border-radius:8px;}"
                                          "#widget_ch6{background-color: rgb(0, 50, 0);border:2px solid rgb(0,150,0);border-radius:8px;};");
            break;
        default:
            ui->widget_ch1->setStyleSheet("");
            ui->widget_ch2->setStyleSheet("");
            ui->widget_ch3->setStyleSheet("");
            ui->widget_ch4->setStyleSheet("");
            ui->widget_ch5->setStyleSheet("");
            ui->widget_ch6->setStyleSheet("");
        break;
        }
        ui->comboBoxSelectCh->setCurrentIndex(index);
        on_comboBoxSelectCh_currentIndexChanged(index);
    }

}

void MainWindow::autoMachUiSet(bool sw)
{
    QPalette pal;

    if(sw)
    {
        ui->tabWidgetSetVI->setEnabled(false);
        ui->groupBox->setEnabled(false);
        ui->pushButtonReadAllAddrToPc->setEnabled(false);
        chartview->setEnabled(false);
        for(int i = 0;i < 6;i++)
        {
            pal.setColor(QPalette::ButtonText, QColor(128,128,128));
            //addrQLineEditArry[i]->setEnabled(false);
            //freqQLineEditArry[i]->setEnabled(false);
            addrSetQPushButtonArry[i]->setEnabled(false);
            addrDelQPushButtonArry[i]->setEnabled(false);
            showQLabelArry[i][7]->setPalette(pal);
        }
    }
    else
    {
        ui->tabWidgetSetVI->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->pushButtonReadAllAddrToPc->setEnabled(true);
        chartview->setEnabled(true);
        pal.setColor(QPalette::ButtonText, QColor(0,0,0));
        for(int i = 0;i < 6;i++)
        {
            //addrQLineEditArry[i]->setEnabled(true);
            //freqQLineEditArry[i]->setEnabled(true);
            addrSetQPushButtonArry[i]->setEnabled(true);
            addrDelQPushButtonArry[i]->setEnabled(true);
            showQLabelArry[i][7]->setPalette(pal);
        }
    }
}



void MainWindow::on_tabWidgetAddr_currentChanged(int index)
{
    Q_UNUSED(index);

    if(autoMachFlag)
    {
        ui->tabWidgetAddr->setCurrentIndex(1);
        ui->statusBar->showMessage(tr("提示信息:必须关闭手动匹配功能才能进行操作!!!"));
        autoMachUiSet(true);
    }
}

void MainWindow::on_actionchina_triggered(bool checked)
{
    Q_UNUSED(checked);
    setLanguage(cn);
}

void MainWindow::on_actionEnglish_triggered(bool checked)
{
    Q_UNUSED(checked);
    setLanguage(en);
}

void MainWindow::on_tabWidgetSetVI_tabBarClicked(int index)
{
    Q_UNUSED(index);
//    if(proData.MDP[index].addrEmpty)
//    {
//        ui->tabWidgetSetVI->setCurrentIndex(proData.now_ch);
//        ui->statusBar->showMessage(QString(tr("提示信息:%1号机通道地址信息为空,无法切换")).arg(index + 1),3000);
//    }
//    else
//    {
//        ui->comboBoxSelectCh->setCurrentIndex(index);
//        on_comboBoxSelectCh_currentIndexChanged(index);
//    }
}

void MainWindow::on_action_triggered(bool checked)
{
    Q_UNUSED(checked);
    swComForm.hide();
    swComForm.on_pushButtonScanCom_clicked();
    swComForm.show();
}

void MainWindow::on_actionVer_triggered(bool checked)
{

    Q_UNUSED(checked);
    QString verStr = tr("当前软件版本为:") + softwareVersion + "\r\n";

    verStr.append(tr("使用该软件需要设备固件版本V1.24.0及以上"));

    //QMessageBox::information(this, tr("版本信息"), verStr);
    QMessageBox *verDialog = new QMessageBox(QMessageBox::Information,tr("版本信息"),verStr,QMessageBox::NoButton);
    verDialog->setStyleSheet("background-color:rgb(240,240,240);");
    verDialog->exec();
}



void MainWindow::on_pushButton_clicked()
{
    chart->zoom(1.2);
}


void MainWindow::on_pushButton_2_clicked()
{
    chart->zoom(0.8);
}

void MainWindow::on_pushButton_3_clicked()
{
    chart->zoomReset();
}

