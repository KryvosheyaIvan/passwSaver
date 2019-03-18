#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <QDebug>
#include "modules/userProfiles/userprofiles.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix(":/img/keysApp.png");
    ui->pictLabel->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));
     ui->debugLabel->setText("App starting...");

    /* Create file of registered users of the pwdSaver app, if it does not exist yet */
    //QString appDir = QCoreApplication::applicationDirPath();
    //QFile file(appDir + "/users.json");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    /* Getting password and username when #login button is clicked */
    QString appUsername = ui->userLineEdit->text();
    QString appPassword = ui->passwLineEdit->text();

    // here it is needed to compare username and password to thos that was
    // registered earlier
    bool isLogOk = pUserProfiles->Login(appUsername, appPassword, this);

    if(isLogOk) {
        QMessageBox::information(this, "Login", "Username and password is correct");

        hide(); //hide main window
        pPassw = new passw();
        pPassw->show();

    }
    else {
        QMessageBox::information(this, "Login", "Username and/or password is wrong!\n"
                                                 "Let's get another try.");
    }
}

void MainWindow::on_regButton_clicked()
{
    bool isAdded = pUserProfiles->addUserProfile(ui->userLineEdit->text(),ui->passwLineEdit->text(), this);//, QJsonObject &jsonFile);

    /* Display message if registration was successful */
    if (isAdded) {
       QMessageBox::information(this, "User registration", "New user was registered");
    }
}











