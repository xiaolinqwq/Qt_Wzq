#ifndef QZ_H
#define QZ_H
#include <QPushButton>
#include <QPixmap>
class qz : public QPushButton
{
public:
    qz();
public:
    int x, y;
    int q_type = -1;
    bool oper = false;
    bool event(QEvent *event);
    int super;
    int *playing;
};

#endif // QZ_H
