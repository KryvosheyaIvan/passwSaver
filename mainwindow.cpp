#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <QDebug>
#include <QMenuBar>
#include "modules/userProfiles/userprofiles.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Password Saver"));

    QPixmap pix(":/img/keysApp.png");
    ui->pictLabel->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));


    /* Create file of registered users of the pwdSaver app, if it does not exist yet */
    QFile checkFile("users.json");
    if(checkFile.exists())
    {
        // QMessageBox::information(this, "debug", "File exists");
        //file was already created, no actions
    }
    else
    {
       //QMessageBox::information(this, "debug", "File does not exist");

        if ( !checkFile.open(QIODevice::WriteOnly)) {
            qDebug() << __FILE__ << __LINE__ << " checkFile failed " << endl;

        }
        checkFile.close();
        qDebug() << __FILE__ << __LINE__ <<" : created file for login " << endl;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked(void)
{
    /* Getting password and username when #login button is clicked */
    QString appUsername = ui->userLineEdit->text();
    QString appPassword = ui->passwLineEdit->text();

    // here it is needed to compare username and password to those that was
    // registered earlier
    bool isLogOk = pUserProfiles->Login(appUsername, appPassword, this);

    if(isLogOk) {
        //QMessageBox::information(this, "Login", "Username and password is correct");

        hide(); //hide main window
        pPassw = new passw(this, appUsername); // call constructor with a Current user
        pPassw->show();

    }
    else {
        QMessageBox::information(this, "Login", "Username and/or password is wrong!\n"
                                                 "Let's get another try.");
    }
}

void MainWindow::on_regButton_clicked(void)
{
    bool isAdded = pUserProfiles->addUserProfile(ui->userLineEdit->text(),ui->passwLineEdit->text(), this);//, QJsonObject &jsonFile);

    /* Display message if registration was successful */
    if (isAdded) {
       QMessageBox::information(this, "User registration", "New user was registered");
    }
    else {
    //   QMessageBox::information(this, "User registration", "Internal error");
    }
}

/* Set password readable/unreadable */
void MainWindow::on_toolButton_toggled(bool checked)
{
   if(checked)
   {
       ui->passwLineEdit->setEchoMode(QLineEdit::Password);
   }
   else
   {
     ui->passwLineEdit->setEchoMode(QLineEdit::Normal);
   }
}
