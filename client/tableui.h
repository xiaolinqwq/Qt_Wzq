#ifndef TABLEUI_H
#define TABLEUI_H

#include <QWidget>
#include "qz.h"
namespace Ui {
class tableui;
}

class tableui : public QWidget
{
    Q_OBJECT

public:
    explicit tableui(QWidget *parent = nullptr);
    ~tableui();
public:
    qz xy[15];
private:
    Ui::tableui *ui;
};

#endif // TABLEUI_H
