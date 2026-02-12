#ifndef AUTOMATCHEXPLAIN_H
#define AUTOMATCHEXPLAIN_H

#include <QWidget>
#include <QFontDatabase>

namespace Ui {
class AutoMatchExplain;
}

class AutoMatchExplain : public QWidget
{
    Q_OBJECT

public:
    explicit AutoMatchExplain(QWidget *parent = nullptr);
    ~AutoMatchExplain();

private:
    Ui::AutoMatchExplain *ui;

    int fontId;
    QString fontName;
};

#endif // AUTOMATCHEXPLAIN_H
