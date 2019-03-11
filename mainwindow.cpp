#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("D:/QTprojects/pswdSaver/pswdSaver/keysApp.png");
    ui->pictLabel->setPixmap(pix.scaled(150,150,Qt::KeepAspectRatio));
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
