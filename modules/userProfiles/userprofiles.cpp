#include "userprofiles.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QCoreApplication>


userProfiles::userProfiles(QObject *parent) : QObject(parent)
{

}

/*
 * Adding user profile to file
 * returns true if success
*/
bool userProfiles::addUserProfile(QString username, QString pswd, QWidget *parent, QJsonObject *jsonFile)
{
    if(username.length() < MIN_STRING_LEN) {
        QMessageBox::information(parent, "Incorrect value", "Too short username");
        return false;
    }
    else if(pswd.length() < MIN_STRING_LEN) {
        QMessageBox::information(parent, "Incorrect value", "Too short password");
        return false;
    }

    /* Open file for writing */
    QString appDir = QCoreApplication::applicationDirPath();
    QFile file(appDir + "/users.json");

    if ( !file.open(QIODevice::WriteOnly/* | QIODevice::Text*/)) {
        qDebug() << "addUserProfile: failed to open file" << endl;
        return false;
    }

    QJsonObject jsObject;
    jsObject["username"] = username;
    jsObject["password"] = pswd;



    QJsonDocument jsDoc(jsObject);
    file.write(jsDoc.toJson());
    //jsDoc.toJson();
    //jsDoc

    //file.write(jsDoc.toJson());

    qDebug() << "username: " + username << endl;
    qDebug() << "password: " + pswd << endl;

    return true;
}

