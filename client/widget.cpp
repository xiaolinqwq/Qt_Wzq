#include "widget.h"
#include <QIcon>
#include "inconnect.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("棋牌室");
    QFont font("Microsoft YaHei", 16, 75);
    this->move(260, 220);
    setFixedSize(1080, 720);
    wzq.setParent(this);
    wzq.setGeometry(60, 80, 200, 200);
    QIcon icon;
    icon.addFile(":/image/image/wzq.jpg");
    wzq.setIcon(icon);
    wzq.setIconSize(QSize(200, 200));
    gwzq.setText("五子棋对战");
    gwzq.setParent(this);
    gwzq.setGeometry(120, 290, 80, 30);
    wzqp.setParent(this);
    wzqp.setText("随即匹配");
    wzqp.setGeometry(80, 330, 60, 30);
    wzqc.setParent(this);
    wzqc.setText("开房间");
    wzqc.setGeometry(160, 330, 60, 30);
    inroom.setParent(this);
    addr.setParent(this);
    addr.setGeometry(80, 370, 100, 30);
    inroom.setText("加入房间");
    inroom.setGeometry(190, 370, 60, 30);
    header.setParent(this);
    header.setGeometry(850, 20, 200, 200);

    name.setParent(this);
    name.setFont(font);
    name.setGeometry(850, 230, 200, 30);

    v.setParent(this);
    v.setFont(font);
    v.setGeometry(850, 260, 100, 60);

    d.setParent(this);
    d.setFont(font);
    d.setGeometry(960, 260, 100, 60);
    connect(&wzqp, &QPushButton::clicked, this, &Widget::wzqp_btn_event);
    connect(&wzqc, &QPushButton::clicked, this, &Widget::wzqc_btn_event);
    connect(&inroom, &QPushButton::clicked, this, &Widget::inroom_btn_event);
}

Widget::~Widget()
{

}

void Widget::updateplayer()
{
    user.headimg.scaled(header.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    header.setScaledContents(true);
    header.setPixmap(user.headimg);
    QString names = "昵称 " + user.player_name;
    name.setText(names);
    QString vs = "胜利 " + user.player_v;
    v.setText(vs);
    QString ds = "失败 " + user.player_d;
    d.setText(ds);
}

void Widget::wzqp_btn_event()
{
    gt.show();
}

void Widget::wzqc_btn_event()
{

    gt.tabinit();
    this->gt.flag = this->gt.icn.internet_connect();
    char* message = new char[1024];
    sprintf(message, "2%s;%s;%s;%s;", user.player_id.toStdString().c_str(),user.player_name.toStdString().c_str(), user.player_v.toStdString().c_str(), user.player_d.toStdString().c_str());
    this->gt.icn.setmsg(message);
    this->gt.icn.internet_send();
    gt.p1 = this->user;
    QString s = this->gt.icn.internet_read();
    gt.romid = s.toStdString().c_str()[0] - '0';
    gt.super = 1;
    gt.g_update(s);
    gt.setWindowFlags(gt.windowFlags() |Qt::Dialog);
    gt.setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    gt.show();
}

void Widget::inroom_btn_event()
{
    gt.tabinit();
    QString s = addr.text();
    //qDebug() << s ;
    char* message = new char[1024];
    sprintf(message, "3%s;%s;%s;%s;%s;", (s.toStdString()).c_str(), user.player_id.toStdString().c_str(),user.player_name.toStdString().c_str(), user.player_v.toStdString().c_str(), user.player_d.toStdString().c_str());
    this->gt.flag = this->gt.icn.internet_connect();
    this->gt.icn.setmsg(message);
    this->gt.icn.internet_send();
    gt.p1 = this->user;
    QString ss = "RoomId: " + s;
    gt.romid = atoi(s.toStdString().c_str());
    gt.super = 0;
    gt.g_update(ss);
    gt.setWindowFlags(gt.windowFlags() |Qt::Dialog);
    gt.setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体
    gt.show();
}
