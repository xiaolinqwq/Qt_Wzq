#include "loginwin.h"
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <iostream>
#include "inconnect.h"

LoginWin::LoginWin()
{
    setWindowTitle("login");
    setFixedSize(400, 300);
    acc.setText("account");
    psswd.setText("passwd");
    acc.setParent(this);
    psswd.setParent(this);
    acc.setGeometry(80,60,80,30);
    psswd.setGeometry(80,100,80,30);
    acc.setStyleSheet("QLabel{font:16px;color:blue;background-color:rgb(f9,f9,f9);}");
    psswd.setStyleSheet("QLabel{font:16px;color:blue;background-color:rgb(f9,f9,f9);}");
    account.setParent(this);
    passwd.setParent(this);
    passwd.setEchoMode(QLineEdit::Password);
    account.setGeometry(160,60,160,30);
    passwd.setGeometry(160,100,160,30);
    login_btn.setParent(this);
    login_btn.setText("login");
    login_btn.setGeometry(120, 160, 60, 30);

    sign_btn.setParent(this);
    sign_btn.setText("sign");
    sign_btn.setGeometry(240, 160, 60, 30);

    connect(&login_btn, &QPushButton::clicked, this, &LoginWin::login_bnt_event);
    connect(&sign_btn, &QPushButton::clicked, this, &LoginWin::sign_bnt_event);
}

void LoginWin::login_bnt_event()
{
    inconnect icn;
    QString acc, pss, pid, name, v, d;
    int ret = icn.internet_connect();
    if(ret == -1) {
        QMessageBox::warning( this, tr("Warning"), tr("connect error!"), QMessageBox::Yes);
        return ;
    }
    char* sql = new char[200];
    sprintf(sql,"0SELECT * from coum WHERE user_acc=\"%s\";",
            std::string(account.text().toStdString()).c_str());
    icn.setmsg(sql);
    ret = icn.internet_send();
    if(ret == -1) {
        QMessageBox::warning( this, tr("Warning"), tr("connect error!"), QMessageBox::Yes);
        return ;
    }
    else{
        QString res = icn.internet_read();

        if(res == "NULL") {
            QMessageBox::warning( this, tr("Warning"), tr("passwd null!"), QMessageBox::Yes);
            passwd.clear();
            return ;
        }

        int cnt = 0;
        //qDebug() << res;
        for(int i=cnt;;i++){
            //qDebug() << res[i] ;
            if(res[i] == ';'){
                cnt = i + 1;
                break;
            }
            else pid += res[i];
        }
        for(int i=cnt;;i++){
            if(res[i] == ';'){
                cnt = i + 1;
                break;
            }
            else acc += res[i];
        }
        for(int i=cnt;;i++){
            if(res[i] == ';'){
                cnt = i + 1;
                break;
            }
            else pss += res[i];
        }
        for(int i=cnt;;i++){
            if(res[i] == ';'){
                cnt = i + 1;
                break;
            }
            else name += res[i];
        }
        for(int i=cnt;;i++){
            if(res[i] == ';'){
                cnt = i + 1;
                break;
            }
            else v += res[i];
        }
        for(int i=cnt;;i++){
            if(res[i] == ';'){
                cnt = i + 1;
                break;
            }
            else d += res[i];
        }
        //qDebug() << pid << " " << acc << " " << pss << " " << name << " " << v << " " << d;
        icn.internet_close();
    }
    if(account.text()==acc && passwd.text()==pss)
    {
        accept();
        user.player_id = pid;
        user.player_name = name;
        user.player_v = v;
        user.player_d = d;
        user.headimg = QPixmap(":/image/image/headimg.png");
    }
    else
    {
        QMessageBox::warning( this, tr("Warning"), tr("account passwd error!"), QMessageBox::Yes);
        passwd.clear();
    }
}

void LoginWin::sign_bnt_event()
{
    QString accline, passline;
    accline = account.text();
    passline = passwd.text();
    if(passline.length() == 0) {
        QMessageBox::warning( this, tr("Warning"), tr("passwd null!"), QMessageBox::Yes);
        return ;
    }
    inconnect icn;
    int ret = icn.internet_connect();
    if(ret == -1) {
        QMessageBox::warning( this, tr("Warning"), tr("connect error!"), QMessageBox::Yes);
        return ;
    }
    char* sql = new char[200];
    sprintf(sql,"1INSERT INTO coum (user_acc, user_passwd,user_name,user_v, user_d, submission_date) VALUES (\"%s\", \"%s\",\"player\",0,0, NOW())",
            std::string(accline.toStdString()).c_str(), std::string(passline.toStdString()).c_str());
    //QString sql = "1INSERT INTO coum (user_acc, user_passwd,user_name,user_v, user_d, submission_date) VALUES \"xiaoin\",  \"123456\",\"player\",0,0, NOW())";
    //qDebug()  << sql << "\n";
    icn.setmsg(sql);
    ret = icn.internet_send();
    if(ret == -1) {
        QMessageBox::warning( this, tr("Warning"), tr("connect error!"), QMessageBox::Yes);
        return ;
    }
    else{
        passwd.clear();
    }
}
