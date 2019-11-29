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

    /* Set grayed out text (tip)*/
    ui->linePwdSearch->setMaxLength(355);
    ui->linePwdSearch->setPlaceholderText("Enter resource here...");

    /* Try to crate table */
    ui->tablePwd->setRowCount(2);
    ui->tablePwd->setColumnCount(3);
    ui->tablePwd->setHorizontalHeaderLabels(QStringList() << "Lock" << "Key" << "Description" );
}

passw::~passw()
{
    delete ui;
}
