#include "reader.h"
#include <QThread>
#include <QDebug>
#include <sstream>
#include <QPixmap>
void reader::run()
{
    //icn->internet_connect();
    while(1)
    {
        qDebug() << "This is once read().";
        if(flag != -1){
            qDebug() << "start read()";
            //icn->internet_read();
            QString s = icn->internet_read();
            qDebug() << "reader: " << s ;
            if(s[0] == '3'){
                int a[6];
                int cnt = 0;
                for(int i=0;i<s.length();i++){
                    if(s[i] == ';'){
                        a[cnt++] = i;
                    }
                }
                QString name, vv, dd, romid;
                for(int i=1;i<a[0];i++) name += s[i];
                for(int i=a[0]+1;i<a[1];i++) vv += s[i];
                for(int i=a[1]+1;i<a[2];i++) dd += s[i];
                for(int i=a[2]+1;i<a[3];i++) romid += s[i];
                this->romid = (romid.toStdString().c_str()[0] - '0');
                p2->player_name = name;
                p2->player_d = dd;
                p2->player_v = vv;

            }else if(s[0] == '4'){
                int a[5];
                int cnt = 0;
                for(int i=0;i<s.length();i++){
                    if(s[i] == ';'){
                        a[cnt++] = i;
                    }
                }
                QString xx, yy, ty;
                for(int i=1;i<a[0];i++) xx += s[i];
                for(int i=a[0]+1;i<a[1];i++) yy += s[i];
                for(int i=a[1]+1;i<a[2];i++) ty += s[i];
                int x, y ,typ;
                std::istringstream tox(xx.toStdString());
                tox >> x;
                std::istringstream toy(yy.toStdString());
                toy >> y;
                std::istringstream tot(ty.toStdString());
                tot >> typ;
                qzs_ptr[x][y].q_type = typ;
                qzs_ptr[x][y].oper = true;
                if(typ == *super){
                    *playing = 0;
                }else{
                    *playing = 1;
                }
                QPixmap pixmap;
                if(typ == 1){
                    pixmap.load(":/image/image/b.png");
                }else{
                    pixmap.load(":/image/image/w.png");
                }
                QPixmap fitpixmap=pixmap.scaled(163,163).scaled(30, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                qzs_ptr[x][y].setIcon(QIcon(fitpixmap));
                qzs_ptr[x][y].setIconSize(QSize(30, 30));
                qzs_ptr[x][y].setFlat(true);//就是这句能够实现按钮透明，用png图片时很有用
                qzs_ptr[x][y].setStyleSheet("border: 0px");//消除边框，取消点击效果
                if(jud(x, y) && typ == *super){
                    char* message = new char[1024];
                    sprintf(message, "6%d;%d;", romid, typ);
                    icn->setmsg(message);
                    icn->internet_send();
                    delete message;
                }
            }else if(s[0] == '5'){
                *isok = 1;
                if(*super == 1){
                    *playing = 1;
                }else{
                    *playing = 0;
                }
            }else if(s[0] == '6'){
                if(s[1] == 'v'){
                    *isok = 2;
                    return ;
                }else {
                    *isok = 3;
                    return ;
                }
            }else if(s[0] == '7'){
                QString ss;
                for(int i=1;i<s.length();i++){
                    ss += s[i];
                }
                *m = ss;
            }
            qDebug() << "endcread()";
        }
    }
}

int reader::jud(int x, int y)
{
    int t = qzs_ptr[x][y].q_type;
    // up
    int cnt = 0;
    for(int i=1;;i++){
        int xx = x - i;
        if(xx < 0) break;
        if(qzs_ptr[xx][y].oper && qzs_ptr[xx][y].q_type == t) cnt ++;
        else break;
    }
    for(int i=1;;i++){
        int xx = x + i;
        if(xx >= 15) break;
        if(qzs_ptr[xx][y].oper && qzs_ptr[xx][y].q_type == t) cnt ++;
        else break;
    }
    if(cnt >= 4) {return 1;}
    // left
    cnt = 0;
    for(int i=1;;i++){
        int yy = y - i;
        if(yy < 0) break;
        if(qzs_ptr[x][yy].oper && qzs_ptr[x][yy].q_type == t) cnt++;
        else break;
    }
    for(int i=1;;i++){
        int yy = y + i;
        if(yy >= 15) break;
        if(qzs_ptr[x][yy].oper && qzs_ptr[x][yy].q_type == t) cnt++;
        else break;
    }
    if(cnt >= 4) {return 1;}
    //x1
    cnt = 0;
    for(int i=1;;i++){
        int yy = y - i, xx = x - i;
        if(yy < 0 || xx < 0) break;
        if(qzs_ptr[xx][yy].oper && qzs_ptr[xx][yy].q_type == t) cnt++;
        else break;
    }
    for(int i=1;;i++){
        int yy = y + i,xx = x + i;
        if(yy >= 15 || xx >= 15) break;
        if(qzs_ptr[xx][yy].oper && qzs_ptr[xx][yy].q_type == t) cnt++;
        else break;
    }
    if(cnt >= 4) {return 1;}
    // x2
    cnt = 0;
    for(int i=1;;i++){
        int yy = y + i, xx = x - i;
        if(yy >= 15 || xx < 0) break;
        if(qzs_ptr[xx][yy].oper && qzs_ptr[xx][yy].q_type == t) cnt++;
        else break;
    }
    for(int i=1;;i++){
        int yy = y - i,xx = x + i;
        if(yy < 0 || xx >= 15) break;
        if(qzs_ptr[xx][yy].oper && qzs_ptr[xx][yy].q_type == t) cnt++;
        else break;
    }
    if(cnt >= 4) {return 1;}
    return 0;
}
