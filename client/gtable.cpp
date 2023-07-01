#include "gtable.h"
#include <QPainter>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include<sys/epoll.h>
#include "inconnect.h"
#include "reader.h"
#include <QApplication>
#include <QTextEdit>
gtable::gtable()
{
    setFixedSize(960, 720);
    setAutoFillBackground(true);

    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            qzs[i][j].x = i;
            qzs[i][j].y = j;
            qzs[i][j].q_type = 0;
            qzs[i][j].setParent(this);
            qzs[i][j].setGeometry( i*40 + 30, j*40 + 60, 50, 50);
        }
    }

    QFont font("Microsoft YaHei", 14, 75);

    roomid.setParent(this);
    roomid.setFont(font);
    roomid.setGeometry(360, 40, 100, 30);

    header1.setParent(this);
    header1.setGeometry(800, 20, 100, 100);

    name1.setParent(this);
    name1.setFont(font);
    name1.setGeometry(820, 140, 200, 30);

    v1.setParent(this);
    v1.setFont(font);
    v1.setGeometry(810, 180, 40, 30);

    d1.setParent(this);
    d1.setFont(font);
    d1.setGeometry(870, 180, 40, 30);

    mess.setParent(this);
    mess.setGeometry(800, 250, 100, 50);
    mess.setVisible(false);

    print.setParent(this);
    print.setGeometry(700, 300, 220, 100);
    print.setReadOnly(true);
    print.setStyleSheet ("background:transparent;border-width:0;border-style:outset");

    send.setParent(this);
    send.setGeometry(700, 420, 220, 30);
    send.setStyleSheet ("background:transparent;border-width:15px;");
    send.setPlaceholderText("在此输入信息与对手聊天吧");
    send.setMaxLength(100);
    sendmess.setParent(this);
    sendmess.setGeometry(870, 460, 50, 30);
    sendmess.setText("Send");
    header.setParent(this);
    header.setGeometry(800, 550, 100, 100);

    name.setParent(this);
    name.setFont(font);
    name.setGeometry(820, 650, 200, 30);

    v.setParent(this);
    v.setFont(font);
    v.setGeometry(810, 680, 40, 30);

    d.setParent(this);
    d.setFont(font);
    d.setGeometry(870, 680, 40, 30);

    readthread = new reader();
    readthread->setParent(this);
    start.setParent(this);
    start.setText("开始游戏");
    start.setGeometry(300, 40, 60, 30);
    connect(&start, &QPushButton::clicked, this, &gtable::startgame);
    connect(&mess, &QPushButton::clicked, this, &gtable::replay);
    connect(&sendmess, &QPushButton::clicked, this, &gtable::send_btn_event);

    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            connect(&qzs[i][j], &qz::clicked, this, [=](){sendxy(i, j);});
        }
    }
    playing = 0;
    isok = 0;
}

void gtable::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 背景图片
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/image/image/gamebackground.png"));
    painter.setPen(QPen(Qt::black,2));//设置画笔形式
    for(int i=0;i<15;i++){
        painter.drawLine(55,i*40+85,15*40 + 15, i*40 + 85);//画直线
        painter.drawLine(i*40+55, 85,i*40+55 , 14*40+85);//画直线
    }
    painter.drawRect(20,30,650,650);//画矩形

    header1.setPixmap(p1.headimg);
    name1.setText(p2.player_name);
    v1.setText(p2.player_v);
    d1.setText(p2.player_d);
    print.setText(updatemess);
    if(isok == 2){
        isok = 0;
        playing = 0;
        //qDebug() << "vec";
        mess.setText("You Win!");
        mess.setVisible(true);

    }else if(isok == 3){
        isok = 0;
        playing = 0;
        //qDebug() << "de";
        mess.setText("You Lose!");
        mess.setVisible(true);
    }
}

void gtable::g_update(QString s)
{

    header.setPixmap(p1.headimg);
    name.setText(p1.player_name);
    v.setText(p1.player_v);
    d.setText(p1.player_d);
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            qzs[i][j].super = this->super;
            qzs[i][j].playing = &(playing);
        }
    }
    romid = atoi(s.toStdString().c_str());
    readthread->icn = &icn;
    readthread->flag = flag;
    readthread->p1 = &p1;
    readthread->p2 = &p2;
    readthread->qzs_ptr = qzs;
    readthread->isok = &(this->isok);
    readthread->super = &(this->super);
    readthread->playing = &(this->playing);
    readthread->m = &(updatemess);
    readthread->romid = romid;
    readthread->start();
    roomid.setText(s);
}

void gtable::closeEvent(QCloseEvent *e){
    // 想要实现的操作
    this->close();
}

void gtable::sendxy(int x, int y)
{
    if(isok == 1 && playing == 1){
        qDebug() << "x, y: " << x << " " << y;
        inconnect con;
        con.internet_connect();
        char* message = new char[1024];
        sprintf(message, "4%d;%d;%d;%d;", romid, x, y, super);
        qDebug() << message ;
        con.setmsg(message);
        con.internet_send();
        con.internet_close();
    }
}

void gtable::startgame()
{

    inconnect con;
    con.internet_connect();
    char* message = new char[1024];
    sprintf(message, "5OK%d", romid);
    con.setmsg(message);
    con.internet_send();
    con.internet_close();
    delete message;
    this->start.setVisible(false);
}

void gtable::tabinit()
{
    super = 0;
    mess.setVisible(false);
    isok = 0;
    playing = 0;
    for(int i=0;i<15;i++){
        for(int j=0;j<15;j++){
            qzs[i][j].oper = false;
            qzs[i][j].q_type = -1;
            qzs[i][j].setIcon(QIcon(""));
            qzs[i][j].setStyleSheet("QPushButton{border:none;background:transparent;}");
        }
    }
    start.setVisible(true);
}

void gtable::replay()
{
    this->close();
}

void gtable::send_btn_event()
{
    if(isok == 1){
        inconnect con;
        con.internet_connect();
        char* message = new char[1024];
        QString s = send.text();
        sprintf(message, "7%d;%s;", romid,s.toStdString().c_str());
        con.setmsg(message);
        con.internet_send();
        con.internet_close();
        delete message;
        send.clear();
    }
}

