#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "player.h"
#include "gametable.h"
#include "gtable.h"
#include "qz.h"
#include <QLineEdit>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void updateplayer();
private:
    QPushButton wzq;
    QPushButton wzqp;
    QPushButton wzqc;
    QPushButton inroom;
    QLabel header;
    QLabel name;
    QLabel v;
    QLabel d;
    QLabel gwzq;
    QLineEdit addr;
public:
    player user;
    gtable gt;
    bool playing = false;
public slots:
    void wzqp_btn_event();
    void wzqc_btn_event();
    void inroom_btn_event();
};
#endif // WIDGET_H
