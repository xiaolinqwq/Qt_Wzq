#ifndef READER_H
#define READER_H
#include <QThread>
#include "inconnect.h"
#include "player.h"
#include "qz.h"
#include <QTextEdit>
#include <QString>
class reader : public QThread
{
public:
    inconnect *icn;
    int flag = 0;
    player *p1, *p2;
    qz (*qzs_ptr)[15];
    int* isok;
    int* super;
    int* playing = NULL;
    int romid;
    QString* m;
//3.重写run函数
protected:
    void run();//线程入口函数
    int jud(int x, int y);
};

#endif // READER_H
