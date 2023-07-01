#include "widget.h"

#include <QApplication>
#include <loginwin.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    LoginWin d;
    d.show();
    if(d.exec() == QDialog::Accepted){
        w.user = d.user;
        d.close();
        w.show();
        w.updateplayer();
        return a.exec();
    }
}
