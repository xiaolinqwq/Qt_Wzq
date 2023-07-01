#include "qz.h"
#include <QEvent>
#include <QPixmap>
#include <QIcon>
#include <QSize>
qz::qz()
{
    x = 1;
    y = 1;
    q_type = -1;
    oper = false;
    setFlat(true);
    setStyleSheet("QPushButton{border:none;background:transparent;}");
}

bool qz::event(QEvent *event)
{
        switch (event->type())
        {
        case QEvent::Enter:
            if(!oper && playing!=NULL && (*playing == 1)){
                qDebug() << *playing;
                QPixmap pixmap;
                if(super == 1){
                    pixmap.load(":/image/image/b.png");
                }else{
                    pixmap.load(":/image/image/w.png");
                }
                QPixmap fitpixmap=pixmap.scaled(163,163).scaled(45, 45, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                setIcon(QIcon(fitpixmap));
                setIconSize(QSize(45, 45));
                setFlat(true);//就是这句能够实现按钮透明，用png图片时很有用
                setStyleSheet("border: 0px");//消除边框，取消点击效果
            }
            break;
        case QEvent::Leave:
            if(!oper&& playing!=NULL && (*playing == 1)){
                qDebug() << *playing;
                this->setIcon(QIcon(""));
                this->setStyleSheet("QPushButton{border:none;background:transparent;}");
            }
            break;
        case QEvent::MouseButtonPress:
            if(!oper&& playing!=NULL && (*playing == 1)){
                qDebug() << *playing;
                //this->setIcon(QIcon(QPixmap(":/image/image/b.png").scaled(rect().size())));
                //QPixmap bgPixmap(":/image/image/b.png");
                QPixmap pixmap;
                if(super == 1){
                    pixmap.load(":/image/image/b.png");
                }else{
                    pixmap.load(":/image/image/w.png");
                }
                QPixmap fitpixmap=pixmap.scaled(163,163).scaled(30, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                setIcon(QIcon(fitpixmap));
                setIconSize(QSize(30, 30));
                setFlat(true);//就是这句能够实现按钮透明，用png图片时很有用
                setStyleSheet("border: 0px");//消除边框，取消点击效果
                this->q_type = super;
                oper = true;
            }
            break;
        //case QEvent::MouseButtonRelease:
        //    this->setIcon(QIcon(":/icons/images/c1.png"));
        //    break;
        default:
            break;
        }
    return QPushButton::event(event);
}

