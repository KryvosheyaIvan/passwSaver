#include "createpassw.h"
#include "ui_createpassw.h"
#include <QMessageBox>
#include "modules/userProfiles/userprofiles.h"

createPassw::createPassw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createPassw)
{
    ui->setupUi(this);

    /* Set title */
    setWindowTitle(tr("New password creation"));

    /* Configure signal->slot */
    connect(ui->buttonOk,     SIGNAL(clicked()), this, SLOT(checkInputs()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(hide()));
}

createPassw::~createPassw()
{
    delete ui;
}


void createPassw::checkInputs(void)
{
   //QMessageBox::information(this, "debug", "button OK pressed!");
   /* Get all inputs from text labels */
   QString pwd1        = ui->linePassw1->text();
   QString pwd2        = ui->linePassw2->text();
   QString resource    = ui->lineResource->text();
   QString description = ui->lineDescription->text();

   /* Check inputs for correctness */
   bool isInputsOk = pUserProfiles->checkNewPassword(resource, pwd1, pwd2, this);

   /* If inputs are wrong -> end execution */
   if( isInputsOk != true) return;

   bool isNewDataAdded = pUserProfiles->addLockKeyPair("Ivan", resource, pwd1, description, this);

}
