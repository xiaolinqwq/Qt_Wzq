#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QPixmap>
class player
{
public:
    player();
public:
    QString player_id;
    QString player_name;
    QString player_v;
    QString player_d;
    QPixmap headimg;
};

#endif // PLAYER_H
