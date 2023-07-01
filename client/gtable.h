#ifndef GTABLE_H
#define GTABLE_H
#include <QWidget>
#include "qz.h"
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include<sys/epoll.h>
#include <QTime>
#include <QWidget>
#include <QCloseEvent>
#include "player.h"
#include "inconnect.h"
#include "reader.h"
#include <QPaintEvent>
#include <QTextEdit>
#include <QLineEdit>
class gtable : public QWidget
{
public:
    gtable();
    void paintEvent(QPaintEvent *event);
    void tabinit();
public:
    int playing = 0;
    qz qzs[15][15];
    QLabel header;
    QLabel name;
    QLabel v;
    QLabel d;
    QLabel header1;
    QLabel name1;
    QLabel v1;
    QLabel d1;
    QPushButton start;
    QLabel roomid;
    QPushButton mess;
    reader * readthread;
    void g_update(QString s);
    int romid;
    void closeEvent(QCloseEvent *e);
    inconnect icn;
    int flag = 0;
    player p1, p2;
    int super = 0;
    int isok = 0;

    QTextEdit print;
    QLineEdit send;
    QPushButton sendmess;
    QString updatemess;
public slots:
    void sendxy(int x, int y);
    void startgame();
    void replay();
    void send_btn_event();
};


#endif // GTABLE_H
