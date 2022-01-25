#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>

static QString getLocalIP(void);


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QHostInfo::localHostName()+" IP:"+getLocalIP());

    //Dark Mode
    QPalette bgpal = palette();
    bgpal.setColor (QPalette::Background, QColor (128, 128 , 128, 255));
    bgpal.setColor (QPalette::Foreground, QColor (255,255,255,255)); setPalette (bgpal);

    cart=new my_widget(this);
    cart->setGeometry(BASE_X,BASE_Y,WIDGET_WIDTH,WIDGET_HEIGHT);
    cart->show();
    connect(this,SIGNAL(update_points(QList<qint16>,qint16)) ,
            this->cart,SLOT(update_points(QList<qint16>,qint16)) );



    udp_socket=new QUdpSocket(this);

    //QHostAddress adress("192.168.1.15");
    udp_socket->bind(1234);
    connect(udp_socket,SIGNAL(readyRead()),this,SLOT(onSocketReadyRead()));


    connect(ui->button_start,SIGNAL(clicked()),this,SLOT(start_button_clicked()));
    is_started=false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

static QString getLocalIP(void)
{
    QString hostName=QHostInfo::localHostName();//本地主机名
    QHostInfo   hostInfo=QHostInfo::fromName(hostName);
    QString   localIP="";

    QList<QHostAddress> addList=hostInfo.addresses();//

    if (!addList.isEmpty())
    for (int i=0;i<addList.count();i++)
    {
        QHostAddress aHost=addList.at(i);
        if (QAbstractSocket::IPv4Protocol==aHost.protocol())
        {
            localIP=aHost.toString();
            break;
        }
    }
    return localIP;
}


void MainWindow::onSocketReadyRead()
{   
    while(udp_socket->hasPendingDatagrams())
    {
        QByteArray   datagram;
        datagram.resize(udp_socket->pendingDatagramSize());

        QHostAddress    peerAddr;       //远端地址
        quint16         peerPort;       //远端端口
        udp_socket->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);
        int size=datagram.size();
        //this->ui->THDx_label->setText(QString("%1").arg(size));
        //显示直接传输的消息
        if(is_started)
        {
            if((datagram[0]=='T'&&size==303))
            {
                QList <qint16> points;
                qint16  length;
                for(int i=0;i<150;i++)
                {
                    points.append( ( qint16((datagram[2*i+1]&0x7f))<<8)|(qint16((datagram[2*i+2]&0x7f))) );
                }
                length=(qint16((datagram[301]&0x7f))<<7)|(qint16((datagram[302]&0x7f)) );

                emit update_points(points,length);
            }
            else if((size==13&&datagram[0]=='D'))
            {
                qint16 tmp_data;
                tmp_data=(qint16((datagram[1]&0x7f))<<7)|(qint16((datagram[2]&0x7f)));
                this->ui->THDx_label->setText("信号失真度THDx  : "+QString::number(float(tmp_data)*100/0x2000,'f',2)+"%");

                tmp_data=(qint16((datagram[3]&0x7f))<<7)|(qint16((datagram[4]&0x7f)));
                this->ui->one_label->setText("基波   : "+QString::number(float(tmp_data)*100/0x2000,'f',2)+"%");

                tmp_data=(qint16((datagram[5]&0x7f))<<7)|(qint16((datagram[6]&0x7f)));
                this->ui->two_label->setText("二次谐波: "+QString::number(float(tmp_data)*100/0x2000,'f',2)+"%");

                tmp_data=(qint16((datagram[7]&0x7f))<<7)|(qint16((datagram[8]&0x7f)));
                this->ui->three_label->setText("三次谐波: "+QString::number(float(tmp_data)*100/0x2000,'f',2)+"%");

                tmp_data=(qint16((datagram[9]&0x7f))<<7)|(qint16((datagram[10]&0x7f)));
                this->ui->four_label->setText("四次谐波: "+QString::number(float(tmp_data)*100/0x2000,'f',2)+"%");

                tmp_data=(qint16((datagram[11]&0x7f))<<7)|(qint16((datagram[12]&0x7f)));
                this->ui->five_label->setText("五次谐波: "+QString::number(float(tmp_data)*100/0x2000,'f',2)+"%");
            }
        }
    }//while
}


void MainWindow::start_button_clicked(void)
{
    if(is_started)
    {
        is_started=false;
        ui->button_start->setText("开始测量");
    }
    else
    {
        is_started=true;
        ui->button_start->setText("开始测量");
    }
}
