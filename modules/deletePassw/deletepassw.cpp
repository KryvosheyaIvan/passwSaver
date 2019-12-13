#include "modules/deletePassw/deletepassw.h"
#include "ui_deletepassw.h"

// default constuctor
deletePassw::deletePassw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deletePassw)
{
    ui->setupUi(this);
}

// constuctor getting name of resource to delete
deletePassw::deletePassw(QWidget *parent, QString user, QString lock) :
    QDialog(parent),
    ui(new Ui::deletePassw)
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "DEL created" << endl;

    ui->setupUi(this);

    /* Set title */
    setWindowTitle(tr("Password deletion"));

    // init vars
    this->username     = user;
    this->lockToDelete = lock;
}


deletePassw::~deletePassw()
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "DEL destroyed" << endl;
    delete ui;
}
