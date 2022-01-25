#include "my_widget.h"

void my_widget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)             //标记不使用这个参数

    QPainter painter(this);     //创建一个绘画元件
    painter.setViewport(0,0,WIDGET_WIDTH,WIDGET_HEIGHT);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    draw_back(painter);

    pen.setColor(Qt::red);
    painter.setPen(pen);

    if(length==0)
    {
        return;
    }
    else
    {
        qint16 max_point=0,min_point=qint16(0x01<<14);
        float div_h;

        for(int i=0;i<150;i++)
        {
            if(points[i]>max_point)
            {
                max_point=points[i];
            }
            if(points[i]<min_point)
            {
                min_point=points[i];
            }
        }

        if(max_point-min_point==0)
        {
            return;
        }
        else
        {
            div_h=300.0f/float(max_point-min_point);
            if(div_h!=0)
            {
                QPoint last_point=QPoint(0,350-(points[0]-min_point)*div_h);
                QPoint this_point;
                for(int i=1;i<150;i++)
                {
                    this_point=QPoint(4*i,350-(points[i]-min_point)*div_h);
                    painter.drawLine(last_point,this_point);
                    last_point=this_point;
                }
                //painter.drawText(0,20,QString("/1%mv").arg(div_h));
            }
        }
    }
}

my_widget::my_widget(QWidget* parent):
    QWidget(parent)
{
    points.clear();
    length=0;
}

void my_widget::update_points(QList <qint16> new_points,qint16 new_length)
{
    points=new_points;
    length=new_length;
    this->update();
}



void my_widget::draw_back(QPainter& painter)
{
    painter.drawLine(1,0,1,400);
    for(int i=1;i<7;i++)
    {
        painter.drawLine(i*100,0,i*100,400);
    }

    painter.drawLine(0,1,600,1);
    for(int i=1;i<10;i++)
    {
        painter.drawLine(0,i*50,600,i*50);
    }
}



