#ifndef __ROOM_H__
#define __ROOM_H__
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include "player.h"
class room{
public:
    player superplayer;
    player complayer;
    int roomid;
    int maps[15][15];
    int oknum = 0;
public:
    void update();
    void pd();

};

#endif