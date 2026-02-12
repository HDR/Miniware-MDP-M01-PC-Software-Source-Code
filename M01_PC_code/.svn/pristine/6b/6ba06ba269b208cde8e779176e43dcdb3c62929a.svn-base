#include "MyChartView.h"
#include <QDebug>

MyChartView::MyChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    isLeftButtonClicking(false),
    xOld(0), yOld(0)
{
    setRubberBand(QChartView::RectangleRubberBand);

    m_isPress = false;
    m_ctrlPress = false;
    m_alreadySaveRange = false;
    m_altPress = false;
    m_coordItem = nullptr;

    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setMouseTracking(false);
}

MyChartView::MyChartView(QWidget *parent)
    :QChartView(parent),
    isLeftButtonClicking(false),
    xOld(0), yOld(0)
{
    setRubberBand(QChartView::RectangleRubberBand);

    m_isPress = false;
    m_ctrlPress = false;
    m_alreadySaveRange = false;
    m_altPress = false;
    m_coordItem = nullptr;

    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setMouseTracking(false);
}

void MyChartView::saveAxisRange()
{
    //QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
    QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axes(Qt::Horizontal).last());
    m_xMin = axisX->min();
    m_xMax = axisX->max();
    //QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());
    QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axes(Qt::Vertical).last());
    m_yMin = axisY->min();
    m_yMax = axisY->max();

    m_Imin = axisI_Y->min();
    m_Imax = axisI_Y->max();
}
void MyChartView::set_I_axix(QValueAxis *axisI_Y)
{
    this->axisI_Y = axisI_Y;
}

void MyChartView::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_lastPoint = e->pos();
        m_isPress = true;
    }
    else if(e->button() == Qt::RightButton)
    {
        //鼠标右键恢复初始化状态
        this->chart()->zoomReset();
        this->chart()->axes(Qt::Horizontal)[0]->setRange(0,4000);
        this->chart()->axes(Qt::Vertical)[0]->setRange(0,30);
        this->chart()->axes(Qt::Vertical)[1]->setRange(0,5);
        zoomCount=10;
    }
    /*
    if (e->button() == Qt::LeftButton)
    {
        m_lastPoint = e->pos();
        int tmp_x = e->pos().x();
        int tmp_y = e->pos().y();

        if(tmp_x > 41 && tmp_x < 90 && tmp_y > 55 && tmp_y < 366)
        {
            m_ctrlPress = true;
        }
        else if (tmp_x > 797 && tmp_x < 854 && tmp_y > 53 && tmp_y < 368)
        {
            m_altPress = true;
        }
        else if (tmp_x > 100 && tmp_x < 796 && tmp_y > 370 && tmp_y < 390)
        {
            m_xPress = true;
        }
        m_isPress = true;
    }
    else
    {
        QChartView::mousePressEvent(e);
    }

*/
}
void MyChartView::mouseMoveEvent(QMouseEvent *e)
{
    const QPoint curpos = e->pos();
    if(m_isPress)
    {
        QPoint offset = curpos - m_lastPoint;
        m_lastPoint = curpos;
        if (!m_alreadySaveRange)
        {
            this->saveAxisRange();
            m_alreadySaveRange = true;
        }
        this->chart()->scroll(-offset.x(), offset.y());
    }
    else
    {
#if 1
        if (!m_coordItem)
        {
            m_coordItem = new QGraphicsSimpleTextItem(this->chart());
            m_coordItem->setZValue(20);
            m_coordItem->setPos(100, 30);
            m_coordItem->show();
            m_coordItem->setFont(QFont("",15));
        }
        const QPoint curPos = e->pos();
        //QPointF curVal = this->chart()->mapToValue(QPointF(curPos));
        QString coordStr = QString().sprintf("X = %.2f, Y = %.2f",QPointF(curPos).x(),QPointF(curPos).y());
        m_coordItem->setBrush(QBrush(QColor(Qt::white)));
        m_coordItem->setText(coordStr);
        m_coordItem->hide();
        if (m_isPress)
        {
            QPoint offset = curPos - m_lastPoint;
            m_lastPoint = curPos;
            if (!m_alreadySaveRange)
            {
                this->saveAxisRange();
                m_alreadySaveRange = true;
            }


            //QValueAxis *tmp_axisV = dynamic_cast<QValueAxis*>(this->chart()->axisY());
            //QValueAxis *tmp_axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
            QValueAxis *tmp_axisV = dynamic_cast<QValueAxis*>(this->chart()->axes(Qt::Horizontal).last());
            QValueAxis *tmp_axisX = dynamic_cast<QValueAxis*>(this->chart()->axes(Qt::Vertical).last());
            //qDebug() << tmp_axisX->min() - offset.x();
            //qDebug() << offset.y() << offset.x();
            if(offset.y() > 0)
            {
                if (m_ctrlPress)
                {
                    double tmp_step = (tmp_axisV->max() - tmp_axisV->min()) / 6 / 2;
                    if(tmp_step < 0.5)tmp_step = 0.5;else tmp_step = (int)(tmp_step+0.5);
                    if(tmp_axisV->max() < V_max)
                        this->chart()->axes(Qt::Vertical).last()->setRange(tmp_axisV->min() + tmp_step,tmp_axisV->max() + tmp_step);
                    else
                        this->chart()->axes(Qt::Vertical).last()->setMax(V_max);
                }
                else if(m_altPress)
                {
                    double tmp_step = (axisI_Y->max() - axisI_Y->min()) / 6 / 2;
                    if(tmp_step < 0.5)tmp_step = 0.5;else tmp_step = (int)(tmp_step+0.5);
                    if(axisI_Y->max() < I_max)
                        axisI_Y->setRange(axisI_Y->min() + tmp_step,axisI_Y->max() + tmp_step);
                    else
                       axisI_Y->setMax(I_max);
                }


            }
            else if(offset.y() < 0)
            {
                if (m_ctrlPress)
                {
                    double tmp_step = (tmp_axisV->max() - tmp_axisV->min()) / 6 / 2;
                    if(tmp_step < 0.5)tmp_step = 0.5;else tmp_step = static_cast<int>(tmp_step+0.5);
                    if(tmp_axisV->min() > V_min)
                        this->chart()->axes(Qt::Vertical).last()->setRange(tmp_axisV->min() - tmp_step,tmp_axisV->max() - tmp_step);
                    else
                        this->chart()->axes(Qt::Vertical).last()->setMin(V_min);
                }
                else if(m_altPress)
                {
                    double tmp_step = (axisI_Y->max() - axisI_Y->min()) / 6 / 2;
                    if(tmp_step < 0.5)tmp_step = 0.5;else tmp_step = static_cast<int>(tmp_step+0.5);
                    if(axisI_Y->min() > I_min)
                        axisI_Y->setRange(axisI_Y->min() - tmp_step,axisI_Y->max() - tmp_step);
                    else
                        axisI_Y->setMin(I_min);
                }
            }

            if(offset.x() > 0)
            {
                double step = (tmp_axisX->max() - tmp_axisX->min()) / 12 ;
                if(step <= 100)step = 100; else step = 400;
                if(m_xPress)
                {
                    if(tmp_axisX->min() > x_min)
                        this->chart()->axes(Qt::Horizontal).last()->setRange(tmp_axisX->min() - step,tmp_axisX->max() - step);
                    else
                        this->chart()->axes(Qt::Horizontal).last()->setMin(x_min);
                }

            }
            else if (offset.x() < 0)
            {
                double step = (tmp_axisX->max() - tmp_axisX->min()) / 12 ;
                if(step <= 100)step = 100; else step = 400;
                if(m_xPress)
                {
                    if(tmp_axisX->max() < x_max)
                        this->chart()->axes(Qt::Horizontal).last()->setRange(tmp_axisX->min() + step,tmp_axisX->max() + step);
                    else
                        this->chart()->axes(Qt::Horizontal).last()->setMax(x_max);
                }
            }


        }
    #endif
        QChartView::mouseMoveEvent(e);
    }
}
void MyChartView::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    m_isPress = false;
    /*
    m_isPress = false;
    if (e->button() == Qt::RightButton)
    {
        if (m_alreadySaveRange)
        {
            this->chart()->axisX()->setRange(m_xMin, m_xMax);
            this->chart()->axisY()->setRange(m_yMin, m_yMax);
            axisI_Y->setMax(m_Imax);
            axisI_Y->setMin(m_Imin);
        }
    }
    else if(e->button() == Qt::LeftButton)
    {
        m_ctrlPress = false;
        m_xPress = false;
        m_altPress = false;
    }
    else
    {
        QChartView::mouseReleaseEvent(e);
    }
*/
}
void MyChartView::wheelEvent(QWheelEvent *event)
{
    if(chart()->series().size())
    {
        //const QPoint curPos = event->pos();
        //QPointF curVal = this->chart()->mapToValue(QPointF(curPos));

        //前滚
        if(event->angleDelta().y()>0)
        {
            if(++zoomCount<=17) chart()->zoom(2);
            else zoomCount--;
        }
        //后滚
        else if(event->angleDelta().y()<0)
        {
            if(--zoomCount>=7) chart()->zoom(0.5);
            else zoomCount++;
        }
        QChartView::wheelEvent(event);
    }
    /*
    //QPointF curVal = QPointF(0,0);
    if (!m_alreadySaveRange)
    {
        this->saveAxisRange();
        m_alreadySaveRange = true;
    }

    int tmp_x = event->x();
    int tmp_y = event->y();
    //qDebug() << tmp_x << tmp_y;
    if (tmp_x > 41 && tmp_x < 90 && tmp_y > 55 && tmp_y < 366)
    {
        QValueAxis *axisY = dynamic_cast<QValueAxis*>(this->chart()->axisY());

        if (event->delta() < 0)
        {
            double tmp_x_max = axisY->max();
            double tmp_x_min = axisY->min();
            //double tmp_step = (tmp_x_max - tmp_x_min ) / 12;
            double tmp_step = 0.5;
            //tmp_step = (tmp_step < 200)?200:500;

            axisY->setMin((tmp_x_min - tmp_step >= V_min)?tmp_x_min - tmp_step:V_min);
            axisY->setMax((tmp_x_max + tmp_step <= V_max)?tmp_x_max + tmp_step:V_max);
        }
        else if(event->delta() > 0)
        {
            double tmp_x_max = axisY->max();
            double tmp_x_min = axisY->min();
            //double tmp_step = (tmp_x_max - tmp_x_min ) / 12;
            double tmp_step = 0.5;
            //tmp_step = (tmp_step <= 200)?200:500;

            if(tmp_x_max - tmp_step >= tmp_x_min + 1)
            {
                tmp_x_max -= tmp_step;
                //tmp_x_min += tmp_step;
            }

            axisY->setMax(tmp_x_max);
            axisY->setMin(tmp_x_min);
        }

    }
    else if (tmp_x > 797 && tmp_x < 854 && tmp_y > 53 && tmp_y < 372)
    {
        QValueAxis *axisY = dynamic_cast<QValueAxis*>(axisI_Y);

        if (event->delta() < 0)
        {
            double tmp_x_max = axisY->max();
            //double tmp_x_min = axisY->min();
            //double tmp_step = (tmp_x_max - tmp_x_min ) / 12;
            double tmp_step = 0.5;
            //tmp_step = (tmp_step < 200)?200:500;

            //axisY->setMin((tmp_x_min - tmp_step >= I_min)?tmp_x_min - tmp_step:I_min);
            axisY->setMax((tmp_x_max + tmp_step <= I_max)?tmp_x_max + tmp_step:I_max);
        }
        else if(event->delta() > 0)
        {
            double tmp_x_max = axisY->max();
            double tmp_x_min = axisY->min();
            //double tmp_step = (tmp_x_max - tmp_x_min ) / 12;
            double tmp_step = 0.5;
            //tmp_step = (tmp_step <= 200)?200:500;

            if(tmp_x_max - tmp_step >= tmp_x_min + 0.5)
            {
                tmp_x_max -= tmp_step;
                tmp_x_min += tmp_step;
            }

            axisY->setMax(tmp_x_max);
            //axisY->setMin(tmp_x_min);
        }
    }
    else if (tmp_x > 100 && tmp_x < 796 && tmp_y > 370 && tmp_y < 390)
    {
        QValueAxis *axisX = dynamic_cast<QValueAxis*>(this->chart()->axisX());
        //qDebug() << "前" <<axisX->max() << axisX->min();
        if (event->delta() < 0)
        {
            double tmp_x_max = axisX->max();
            double tmp_x_min = axisX->min();
            double tmp_step = (tmp_x_max - tmp_x_min ) / 12;

            tmp_step = (tmp_step < 100)?100:500;

            //axisX->setMin((tmp_x_min - tmp_step >= x_min)?tmp_x_min - tmp_step:x_min);
            axisX->setMax((tmp_x_max + tmp_step <= x_max)?tmp_x_max + tmp_step:x_max);
        }
        else  if(event->delta() > 0)
        {
            double tmp_x_max = axisX->max();
            double tmp_x_min = axisX->min();
            double tmp_step = (tmp_x_max - tmp_x_min ) / 12;

            tmp_step = (tmp_step <= 100)?100:500;

            if(tmp_x_max - tmp_step >= tmp_x_min + 100)
            {
                tmp_x_max -= tmp_step;
            }

            axisX->setMax(tmp_x_max);
            axisX->setMin(tmp_x_min);

        }

    }

    QChartView::wheelEvent(event);
    */
}

void MyChartView::keyPressEvent(QKeyEvent *e)
{
    QChartView::keyPressEvent(e);
}
void MyChartView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Control:
        m_ctrlPress = false;
        break;
    case Qt::Key_Alt:
        m_altPress = false;
        break;
    default:
        QChartView::keyReleaseEvent(event);
    break;
    }
}
