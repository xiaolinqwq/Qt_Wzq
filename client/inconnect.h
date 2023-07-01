#ifndef INCONNECT_H
#define INCONNECT_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<error.h>

#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include <QString>
#include<queue>
#include <fcntl.h>

const int msg_size = 1024;
class inconnect
{
public:
    inconnect();
    ~inconnect();
public:
    int clnt_sock;
    struct sockaddr_in serv_addr;
    char msg[msg_size];
public:
    int internet_connect();
    QString internet_read();
    int internet_close();
    int internet_send();
    int setmsg(char* message);
};

#endif // INCONNECT_H
