#include "passw.h"
#include "ui_passw.h"
#include <QPixmap>

passw::passw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::passw)
{
    ui->setupUi(this);
    /* Set title */
    setWindowTitle(tr("Password Saver"));

    /* Add search picture */
    QPixmap pixSearch(":/img/search.png");
    ui->labelSearch->setPixmap(pixSearch);
    ui->labelSearch->setPixmap(pixSearch.scaled(60,60,Qt::KeepAspectRatio));
}

passw::~passw()
{
    delete ui;
}
