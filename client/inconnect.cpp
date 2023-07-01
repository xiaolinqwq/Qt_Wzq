#include "inconnect.h"
#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <iostream>
#include <QDebug>
inconnect::inconnect()
{
}
inconnect::~inconnect()
{

}
int inconnect::internet_connect()
{
    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

    if(clnt_sock == -1) {
        //qDebug() << "socket create fail";
        return -1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(3456);

    if(::connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        //qDebug() << "connect fail";
        return -1;
    }
    return 0;
}

QString inconnect::internet_read()
{
    QString res;
    char *message = new char[1024];
    int str_len = recv(clnt_sock, message, 1024, 0);
    if(str_len == -1) return res;
    for(int i=0;i<strlen(message);i++){
        res += message[i];
    }
    delete message;
    return res;
}

int inconnect::internet_send()
{
   int ret = send(clnt_sock, msg, sizeof(msg), 0);
   return ret;
}

int inconnect::internet_close()
{
    int ret = ::close(clnt_sock);
    return ret;
}

int inconnect::setmsg(char* message)
{
    strcpy(msg, message);
    return 0;
}
