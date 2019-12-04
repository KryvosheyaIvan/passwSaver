#include "createpassw.h"
#include "ui_createpassw.h"

createPassw::createPassw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createPassw)
{
    ui->setupUi(this);
}

createPassw::~createPassw()
{
    delete ui;
}
