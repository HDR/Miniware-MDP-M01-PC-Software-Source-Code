#include "mainwindow.h"
#include <QApplication>
#include <windows.h>
#include <QSplashScreen>
#include <QTranslator>
#include <QMessageBox>
bool checkOneRun(void)
{
    //  创建互斥量
    HANDLE m_hMutex  =  CreateMutex(nullptr, FALSE,  L"14BCCFD7721BD580" );
    //  检查错误代码
    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)
    {
        //  如果已有互斥量存在则释放句柄并复位互斥量
        CloseHandle(m_hMutex);
        m_hMutex  =  nullptr;
        //  程序退出
        return  false;
    }
    else
        return true;
}


int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    //检测是否单独运行
    if(!checkOneRun())
    {

        QMessageBox::warning(nullptr, a.tr("警告/warning"), a.tr("程序已经运行,请勿重复运行!\r\nThe program has been run, please do not repeat!"), QMessageBox::Yes);
        //MessageBox(nullptr, TEXT("程序已经运行,请勿重复运行"), TEXT("请勿重复运行"), MB_OK);
        return 0;
    }

    QSplashScreen splash;
    splash.setPixmap(QPixmap(":/pic/logo.png"));
    splash.show();
    a.processEvents();
    MainWindow w;

    w.scanComList();
    //展示Logo
    //Sleep(200);
    w.show();

    //隐藏开机logo
    splash.finish(&w);

    return a.exec();
}
