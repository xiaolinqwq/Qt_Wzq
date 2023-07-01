#include "tableui.h"
#include "ui_tableui.h"

tableui::tableui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tableui)
{
    ui->setupUi(this);

}

tableui::~tableui()
{
    delete ui;
}
