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
    QString appDir = QCoreApplication::applicationDirPath();
    QFile file(appDir + "/users.json");

    /* Show some info at the bottom of the app */
    if (file.exists())
    {
        ui->debugLabel->setText("Users accounts founded.");
    }
    else{
        ui->debugLabel->setText("Creating users database...");
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.close();
    }
    else {
        qDebug() << "Fail to open file";
       // QTextStream out(&file);
       // out << "{}" << endl;
    }

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

    if(appUsername == "test" && appPassword == "test") {
        QMessageBox::information(this, "Login", "Username and password is correct");
    }
    else {
        QMessageBox::warning(this, "Login", "Username and password is wrong!");
    }
}

void MainWindow::on_regButton_clicked()
{
    pUserProfiles->addUserProfile(ui->userLineEdit->text(),ui->passwLineEdit->text(), this, 0);//, QJsonObject &jsonFile);
}











