#include "createpassw.h"
#include "ui_createpassw.h"
#include <QMessageBox>
#include "modules/userProfiles/userprofiles.h"

/* Default constructor (must not be used)*/
createPassw::createPassw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createPassw)
{

}

createPassw::createPassw(QWidget *parent, QString user) :
    QDialog(parent),
    ui(new Ui::createPassw)
{
    username = user; //init user

    /* Init GUI */
    ui->setupUi(this);

    /* Set title */
    setWindowTitle(tr("New password creation"));

    /* Configure signal->slot */
    connect(ui->buttonOk,     SIGNAL(clicked()), this, SLOT(checkInputs()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(hide()));

    //ui->lineResource->clear(); doesnt work
    /* Write tips for user */
    ui->lineResource->setPlaceholderText("Enter resource to save password from...");
    ui->linePassw1->setPlaceholderText("Enter password here...");
    ui->linePassw2->setPlaceholderText("Type password once again...");
    ui->lineDescription->setPlaceholderText("Optionally, type descrition...");
}

createPassw::~createPassw()
{
    delete ui;
}


void createPassw::checkInputs(QWidget *parent)
{
   //QMessageBox::information(this, "debug", "button OK pressed!");
   /* Get all inputs from text labels */
   QString pwd1        = ui->linePassw1->text();
   QString pwd2        = ui->linePassw2->text();
   QString resource    = ui->lineResource->text();
   QString description = ui->lineDescription->text();
   QString currUser    = username;

   /* Check inputs for correctness */
   bool isInputsOk = pUserProfiles->checkNewPassword(resource, pwd1, pwd2, this);

   /* If inputs are wrong -> end execution */
   if( isInputsOk != true) return;

   bool isNewDataAdded = pUserProfiles->addLockKeyPair(currUser, resource, pwd1, description, this);

   if (isNewDataAdded)
   {
      QMessageBox::information(parent,"New password", "New password added!");
   }

}
