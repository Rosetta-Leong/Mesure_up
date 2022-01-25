#ifndef MY_WIDGET_H
#define MY_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QList>



#define WIDGET_WIDTH  600       //窗口的宽度
#define WIDGET_HEIGHT 400       //信号窗口的高度
#define BASE_X  270      //统计图绘制开始的位置
#define BASE_Y  40


class my_widget : public QWidget
{
Q_OBJECT
public:
    my_widget (QWidget* parent=nullptr);
    ~my_widget(void)override{}
protected:
    void paintEvent(QPaintEvent* event)override;

private:

    void draw_back(QPainter& painter);

    QList <qint16> points;
    qint16 length;

private slots:
    void update_points(QList <qint16> new_points,qint16 new_length);
};


#endif // MY_WIDGET_H
