#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTranslator>

#include <QDebug>
#include <QMenuBar>
#include "modules/userProfiles/userprofiles.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Set title */
    setWindowTitle(tr("Password Saver"));

    ui->userLabel->setText(tr("Username"));

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

    connect(ui->langButton, SIGNAL(triggered()), this, SLOT(on_langButton_clicked()) );
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

/* Change language button */
void MainWindow::on_langButton_clicked(void)
{
    static unsigned char langConstNum = ENGLISH;
    QString strLangIconPath;

    langConstNum++;

    switch(langConstNum)
    {
       case ENGLISH:
        strLangIconPath = ":/img/eng_round.png";

        // Load English
        ui->langlabel->setText("Eng");
        translator.load(":/translations/QtLanguage_Eng.qm");
        qApp->installTranslator(&translator);
        break;

       case GERMAN:
        strLangIconPath = ":/img/ger_round.png";

        // Load German
        ui->langlabel->setText("Deu");
        translator.load(":/translations/QtLanguage_Ger.qm");
        qApp->installTranslator(&translator);
        break;

       case UKRAINIAN:
        strLangIconPath = ":/img/ukr_round.png";

        // Load Ukrainian
        ui->langlabel->setText("Укр");
        translator.load(":/translations/QtLanguage_Ukr.qm");
        qApp->installTranslator(&translator);

        //reset counter
        langConstNum = 0;
        break;

    default:
        qDebug() << "must not be here" << endl;
        //strLangIconPath = ":/img/eng_round.png";
        //ui->langlabel->setText("Eng");
        langConstNum = ENGLISH;
        break;
    }

  //qDebug() << QString::number(langConstNum) << endl;

   // Set appropriate language abbr. and icon
   QPixmap LangPixmap(strLangIconPath);
   QIcon langIcon(LangPixmap);
   ui->langButton->setIcon(langIcon);
}

/* Chenge event interrupt */
void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        //qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "Retranslate" << endl;
        ui->retranslateUi(this);
    }
}


