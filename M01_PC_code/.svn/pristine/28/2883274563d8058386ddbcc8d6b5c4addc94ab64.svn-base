#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QObject>
#include <QChartView>
#include <QRubberBand>
#include <QValueAxis>
QT_CHARTS_USE_NAMESPACE

class MyChartView : public QChartView
{
public:
    MyChartView(QChart *chart, QWidget *parent = nullptr);
    MyChartView(QWidget *parent = nullptr);
    void saveAxisRange();

    void set_I_axix(QValueAxis *axisI_Y);

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e); //键盘松开事件
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *event);


private:
    bool isLeftButtonClicking;

    int zoomCount=10;

    int xOld;
    int yOld;

    double x_min = 0;
    double x_max = 60000;

    double V_min = 0;
    double V_max = 31;

    double I_min = -0;
    double I_max = 11;

    QPoint m_lastPoint;
    bool m_isPress;
    bool m_ctrlPress;
    bool m_altPress;
    bool m_xPress;
    bool m_alreadySaveRange;
    double m_xMin, m_xMax, m_yMin, m_yMax;

    double m_Imin,m_Imax;
    QGraphicsSimpleTextItem* m_coordItem;

    QValueAxis *axisI_Y;
//    bool m_bMiddleButtonPressed;
//    QPoint m_oPrePos;

};

#endif // MYCHARTVIEW_H
