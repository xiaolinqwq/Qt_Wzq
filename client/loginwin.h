#ifndef LOGINWIN_H
#define LOGINWIN_H
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "player.h"
class LoginWin : public QDialog
{
public:
    LoginWin();
private:
    QLineEdit account;
    QLineEdit passwd;
    QPushButton login_btn;
    QPushButton sign_btn;
    QLabel acc;
    QLabel psswd;

public slots:
    void login_bnt_event();
    void sign_bnt_event();

public:
    player user;
};

#endif // LOGINWIN_H
