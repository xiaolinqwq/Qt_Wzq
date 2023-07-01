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

#include<queue>
#include <mysql/mysql.h>
#include "sqlcon.h"
#include "room.h"
#include <fcntl.h>
using namespace std;
bool roomid[100] = {true};
const int msg_size = 1024;
const int epoll_event_size = 50;
room gameroom[100];
void server_init(int &serv_sock, struct sockaddr_in &serv_addr, char* ports);
int main(int argc, char* argv[])
{
    if(argc != 2) {printf("You don`t input port.\n"); exit(1);}
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char  msg[msg_size];
    server_init(serv_sock, serv_addr, argv[1]);
    int optset = 1;
    int ret;
    ret = setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&optset,sizeof(optset));
    if(ret == -1) {printf("setscokopt error.\n"); exit(1);}
    ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret == -1) {printf("bind error.\n"); exit(1);}
    ret = listen(serv_sock, 10);
    if(ret == -1) {printf("listen error.\n"); exit(1);}
    
    int epoll_fd;
    struct epoll_event event;
    epoll_fd = epoll_create(5000);
    struct epoll_event *events = (struct epoll_event*)malloc(sizeof(struct epoll_event) * epoll_event_size);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &event);

    for(int i=0;i<100;i++) roomid[i] = true;
    while(1){
        int event_cnt = epoll_wait(epoll_fd, events, epoll_event_size, -1);
        //if(event_cnt == -1){printf("epoll_wait() error.\n"); break;}
        for(int i=0;i<event_cnt;i++){
            if(events[i].data.fd == serv_sock){
                clnt_addr_size = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("[%d][Client]<IP:%s><PT:%d><***CONNECT***>\n",__LINE__,inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));
            }else{
                clnt_sock = events[i].data.fd;
                if(clnt_sock == gameroom[0].superplayer.sock) {std::cout << "is OK\n";}
                if(clnt_sock < 0) continue;
                memset(msg,0,sizeof(msg));
                ret=read(clnt_sock, msg, sizeof(msg));
                if(ret == 0){
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                    printf("client close\n");
                }
                std::cout << "Client message: ";
                puts(msg);
                if(msg[0] == '0'){
                    
                    Sqlcon msql;
                    if(!msql.InitDB("localhost","root","123456","user")){
                        std::cout << "Mysql Init failed.\n";
                    }
                    else std::cout << "Mysql Init success.\n";
                    std::string sql;
                    for(int i=1;i<strlen(msg);i++){
                        sql += msg[i];
                    }
                    // INSERT INTO coum (user_acc, user_passwd,user_name,user_v, user_d, submission_date) VALUES ("xiaoin",  "123456","player",0,0, NOW());
                    if(!msql.PlaySQL(sql)){
                        cout << "ERROR.\n";
                    }
                    else{
                        cout << "PLAY SQL.\n";
                    }
                    MYSQL_ROW row ;
                    MYSQL_RES* res = msql.get_res();
                    int nums = mysql_num_rows(res);
                    int flag = 1;
                    if(nums == 0) {
                        flag = 0;
                    }
                    if(flag == 1){
                        row = mysql_fetch_row(res);
                        //std::cout << row[1] << " " << row[2] << "\n";
                    }
                    char* message = new char[1024];
                    if(flag == 0) sprintf(message, "NULL");
                    else {
                        int cnt = 0;
                        for(int i=0;i<6;i++){
                            for(int j=0;j<strlen(row[i]);j++){
                                message[cnt++] = row[i][j];
                            }
                            message[cnt++] = ';';
                        }
                        message[cnt++] = '\0';

                    }
                    std::cout << message << "\n";
                    write(clnt_sock, message, 1024);
                    delete message;
                    message = NULL;
                    msql.resfree();
                    res = NULL;
                    
                }
                else if(msg[0] == '1'){
                    Sqlcon msql;
                    if(!msql.InitDB("localhost","root","123456","user")){
                        std::cout << "Mysql Init failed.\n";
                    }
                    else std::cout << "Mysql Init success.\n";
                    std::string sql;
                    for(int i=1;i<strlen(msg);i++){
                        sql += msg[i];
                    }
                    // INSERT INTO coum (user_acc, user_passwd,user_name,user_v, user_d, submission_date) VALUES ("xiaoin",  "123456","player",0,0, NOW());
                    if(!msql.PlaySQL(sql)){
                        cout << "ERROR.\n";
                    }
                    else{
                        cout << "PLAY SQL.\n";
                    }
                }
                else if(msg[0] == '2'){
                    char* message = new char[1024];
                    int i = 0;
                    for(i=0;i<100;i++){
                        if(roomid[i]) {
                            roomid[i] = false;
                            break;
                        }
                    }
                    string iid, name, vv, dd;
                    int a[6];
                    int cnt = 0;
                    for(int i=1;i<strlen(msg);i++){
                        if(msg[i] == ';') {
                            a[cnt++] = i;
                        }
                    }
                    for(int i=1;i<a[0];i++) iid += msg[i];
                    for(int i=a[0] + 1;i<a[1];i++) name += msg[i];
                    for(int i=a[1] + 1;i<a[2];i++) vv += msg[i];
                    for(int i=a[2] + 1;i<a[3];i++) dd += msg[i];
                    //sstd::cout << "id : " << i << endl;
                    sprintf(message, "%d", i);
                    gameroom[i].superplayer.sock = clnt_sock;
                    gameroom[i].superplayer.iid = iid;
                    gameroom[i].superplayer.name = name;
                    gameroom[i].superplayer.v = vv;
                    gameroom[i].superplayer.d = dd;
                    write(clnt_sock, message, 1024);
                    delete message;
                }
                else if(msg[0] == '3'){
                    sleep(1);
                    char c = msg[1];
                    int romid = c - '0';
                    //std::cout << "romid: " << romid << "\n";
                    gameroom[romid].complayer.sock = clnt_sock;
                    char* message = new char[1024];
                    int a[6];
                    int cnt = 0;
                    for(int i=0;i<strlen(msg);i++) {
                        if(msg[i] == ';') {
                            a[cnt++] = i;
                        }
                    }
                    std::string iid, name, vv,dd;
                    for(int i=a[0]+1;i<a[1];i++) {iid += msg[i];}
                    //std::cout << "name: " << name << "\n";
                    for(int i=a[1]+1;i<a[2];i++) {name += msg[i];}
                    for(int i=a[2]+1;i<a[3];i++) {vv += msg[i];}
                    for(int i=a[3]+1;i<a[4];i++) {dd += msg[i];}
                    gameroom[romid].complayer.iid = iid;
                    gameroom[romid].complayer.name = name;
                    gameroom[romid].complayer.v = vv;
                    gameroom[romid].complayer.d = dd;

                    sprintf(message, "3%s;%s;%s;%d;",name.c_str(), vv.c_str(), dd.c_str(), romid);
                    std::cout << message << '\n';
                    //write(gameroom[romid].complayer.sock, message,1024);
                    
                    write(gameroom[romid].superplayer.sock, message, 1024);
         
                    sprintf(message, "3%s;%s;%s;",gameroom[romid].superplayer.name.c_str(), gameroom[romid].superplayer.v.c_str(), gameroom[romid].superplayer.d.c_str());
                    write(gameroom[romid].complayer.sock, message, 1024);
                    //std::cout << "ret:" << ret <<  "\n"; 
                    
                    //write(gameroom[romid].complayer.sock, message, 1024);
                    
                    delete message;
                    
                }else if(msg[0] == '4'){
                    int romid = msg[1] - '0';
                    int a[5];
                    int cnt = 0;
                    char* message = new char[1024];
                    std::string xx,  yy, ty;
                    for(int i=0;i<strlen(msg);i++){
                            if(msg[i] == ';'){
                                a[cnt++] = i;
                            }
                    }
                    for(int i=a[0]+1;i<a[1];i++) xx += msg[i];
                    for(int i=a[1]+1;i<a[2];i++) yy += msg[i];
                    for(int i=a[2]+1;i<a[3];i++) ty += msg[i];
                    sprintf(message, "4%s;%s;%s;",xx.c_str(), yy.c_str(), ty.c_str());
                    //std::cout << "message: " << message << "\n";
                    write(gameroom[romid].superplayer.sock,message, 1024);
                    
                    write(gameroom[romid].complayer.sock, message, 1024);
                }else if(msg[0] == '5'){
                    if(msg[1] == 'O' && msg[2] == 'K'){
                        int romid = msg[3] - '0';
                        gameroom[romid].oknum ++;
                        if(gameroom[romid].oknum >= 2){
                            char message[1024] = "5OK";
                            write(gameroom[romid].superplayer.sock,message, 1024);
                            write(gameroom[romid].complayer.sock, message, 1024);
                        } 
                    }
                }else if(msg[0] == '6'){
                    int romid = msg[1] - '0';
                    int vet = msg[3] - '0';
                    if(vet == 1){
                        char m1[1024] = "6v";
                        char m2[1024] = "6d";
                        write(gameroom[romid].superplayer.sock, m1, 1024);
                        write(gameroom[romid].complayer.sock, m2, 1024);
                        Sqlcon msql;
                        if(!msql.InitDB("localhost","root","123456","user")){
                            std::cout << "Mysql Init failed.\n";
                        }
                        else std::cout << "Mysql Init success.\n";
                        std::cout << "super v:" << gameroom[romid].superplayer.v << "\n";
                        int vv = atoi(gameroom[romid].superplayer.v.c_str()) + 1;
                        std::cout << "vv: " << vv; 
                        std::string cc = std::to_string(vv);
                        std::string sql = "UPDATE coum SET user_v=";
                        sql += cc;
                        sql +=" WHERE user_id=";
                        sql += gameroom[romid].superplayer.iid.c_str();
                        sql += ";";
                        std::cout << "sql: " << sql << "\n";
                        // INSERT INTO coum (user_acc, user_passwd,user_name,user_v, user_d, submission_date) VALUES ("xiaoin",  "123456","player",0,0, NOW());
                        if(!msql.PlaySQL(sql)){
                            cout << "ERROR.\n";
                        }
                        else{
                            cout << "PLAY SQL.\n";
                        }
                        vv = atoi(gameroom[romid].complayer.d.c_str()) + 1;
                        cc = std::to_string(vv);
                        sql = "UPDATE coum SET user_d=";
                        sql += cc;
                        sql +=" WHERE user_id=";
                        sql += gameroom[romid].complayer.iid.c_str();
                        sql += ";";
                        std::cout << "sql: " << sql << "\n";
                        if(!msql.PlaySQL(sql)){
                            cout << "ERROR.\n";
                        }
                        else{
                            cout << "PLAY SQL.\n";
                        }
                    }else{
                        char m1[1024] = "6v";
                        char m2[1024] = "6d";
                        write(gameroom[romid].superplayer.sock, m2, 1024);
                        write(gameroom[romid].complayer.sock, m1, 1024);

                        Sqlcon msql;
                        if(!msql.InitDB("localhost","root","123456","user")){
                            std::cout << "Mysql Init failed.\n";
                        }
                        else std::cout << "Mysql Init success.\n";
                        std::cout << "super d:" << gameroom[romid].superplayer.d << "\n";
                        int vv = atoi(gameroom[romid].superplayer.d.c_str()) + 1;
                        std::string cc = std::to_string(vv);
                        std::string sql = "UPDATE coum SET user_d=";
                        sql += cc;
                        sql +=" WHERE user_id=";
                        sql += gameroom[romid].superplayer.iid.c_str();
                        sql += ";";
                        std::cout << "sql: " << sql << "\n";
                        // INSERT INTO coum (user_acc, user_passwd,user_name,user_v, user_d, submission_date) VALUES ("xiaoin",  "123456","player",0,0, NOW());
                        if(!msql.PlaySQL(sql)){
                            cout << "ERROR.\n";
                        }
                        else{
                            cout << "PLAY SQL.\n";
                        }
                        vv = atoi(gameroom[romid].complayer.v.c_str()) + 1;
                        cc = std::to_string(vv);
                        sql = "UPDATE coum SET user_v=";
                        sql += cc;
                        sql +=" WHERE user_id=";
                        sql += gameroom[romid].complayer.iid.c_str();
                        sql += ";";
                        std::cout << "sql: " << sql << "\n";
                        if(!msql.PlaySQL(sql)){
                            cout << "ERROR.\n";
                        }
                        else{
                            cout << "PLAY SQL.\n";
                        }
                    }
                    roomid[romid] = true;
                    gameroom[romid].oknum = 0;
                }else if(msg[0] == '7'){
                    int romid = msg[1] - '0';
                    char* message = new char[1024];
                    message[0] = '7';
                    int cnt = 1; 
                    for(int i=3;i<strlen(msg)-1;i++){
                        message[cnt++] = msg[i];
                    }
                    write(gameroom[romid].superplayer.sock, message, 1024);
                    write(gameroom[romid].complayer.sock, message, 1024);
                }
            }
        }
    }
    return 0;
}

void server_init(int &serv_sock, struct sockaddr_in &serv_addr, char* ports)
{
    serv_sock = socket(PF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
    if(serv_sock == -1) {printf("socket() error.\n"); exit(1);}
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(ports));

}