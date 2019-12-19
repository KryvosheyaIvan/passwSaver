#include "createpassw.h"
#include "ui_createpassw.h"
#include <QMessageBox>
#include <QDebug>
#include <QCursor>
#include "modules/userProfiles/userprofiles.h"

/* Default constructor (must not be used)*/
createPassw::createPassw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createPassw)
{

}

/* Constructor, instance gets username */
createPassw::createPassw(QWidget *parent, QString user) :
    QDialog(parent),
    ui(new Ui::createPassw)
{
    //qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "...Constructor" << endl;

    username = user; //init user

    /* Init GUI */
    ui->setupUi(this);

    /* Set title */
    setWindowTitle(tr("New password creation"));

    /* Configure signal->slot */
    connect(ui->buttonOk,     SIGNAL(clicked()), this, SLOT(checkInputs()));
    connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(cleanAndHide()));

    /* Clean fields and Write tips for user */
    clearFields();

}

createPassw::~createPassw()
{
    //qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "...Destructor" << endl;
    parentWidget();

    delete ui;
}

/* Slot. When OK button is clicked */
void createPassw::checkInputs(QWidget *parent)
{
    QString errMsg = tr("Checking inputs...\n");

    // waiting cursor (sandbox)
    //just for fun
    //this->setCursor(Qt::BusyCursor);

   /* Get all inputs from text labels */
   QString pwd1        = ui->linePassw1->text();
   QString pwd2        = ui->linePassw2->text();
   QString resource    = ui->lineResource->text();
   QString description = ui->lineDescription->text();
   QString currUser    = username;

   /* Check inputs for correctness */
   bool isInputsOk = pUserProfiles->checkNewPassword(currUser, description, resource, pwd1, pwd2, this, errMsg);

   /* If inputs are wrong -> end execution */
   if( !isInputsOk)
   {
       QMessageBox::information(this,"New password", errMsg);
       return;
   }

   bool isNewDataAdded = pUserProfiles->addLockKeyPair(currUser, resource, pwd1, description, this, errMsg);

   if (isNewDataAdded)
   {
      QMessageBox::information(this,"New password", tr("New password added!"));

      /* Clean fields and Write tips for user */
      clearFields();

      hide();

      /* Normal end of operation */

   }
   else
   {
      // errMsg now contains error info
      QMessageBox::information(this,"New password", errMsg);
   }


}

/* Slot. Cancel button clicked. Clear text and hide window */
void createPassw::cleanAndHide(QWidget *parent)
{
   qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "...Clean" << endl;
   clearFields();
   hide();
}

/* Clears text fields and setting default grey helpful titles */
void createPassw::clearFields(void)
{
    //Clearing fields
   ui->lineResource->clear();
   ui->linePassw1->clear();
   ui->linePassw2->clear();
   ui->lineDescription->clear();

   // Write tips for user
   ui->lineResource->setPlaceholderText("Enter resource to save password from...");
   ui->linePassw1->setPlaceholderText("Enter password here...");
   ui->linePassw2->setPlaceholderText("Type password once again...");
   ui->lineDescription->setPlaceholderText("Optionally, type descrition...");
}
