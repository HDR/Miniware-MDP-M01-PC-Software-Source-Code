#include "automatchexplain.h"
#include "ui_automatchexplain.h"

AutoMatchExplain::AutoMatchExplain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoMatchExplain)
{
    ui->setupUi(this);

    fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/font/SourceHanSansCN-Regular.ttf"));
    //qDebug()<<"7:"<<QFontDatabase::applicationFontFamilies(fontId[6]).at(0);
    fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);

    ui->label->setFont(QFont(fontName,16,QFont::Bold));
    ui->label_2->setFont(QFont(fontName,16,QFont::Bold));
    ui->label_4->setFont(QFont(fontName,16,QFont::Bold));

    ui->pushButton->setFont(QFont(fontName,14,QFont::Bold));
}

AutoMatchExplain::~AutoMatchExplain()
{
    delete ui;
}
