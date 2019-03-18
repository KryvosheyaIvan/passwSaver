#include "passw.h"
#include "ui_passw.h"

passw::passw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::passw)
{
    ui->setupUi(this);
}

passw::~passw()
{
    delete ui;
}
