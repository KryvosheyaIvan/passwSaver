#include "userprofiles.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
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

    if ( !file.open(QIODevice::ReadOnly /*WriteOnly*/ | QIODevice::Text)) {
        qDebug() << "addUserProfile: failed to open file" << endl;
        return false;
    }
    QString strFile = file.readAll();

    //first need to read from file and then append
    QJsonDocument jsReadDoc;
    //jsReadDoc = QJsonDocument::fromJson(file.readAll());
    jsReadDoc = QJsonDocument::fromJson(strFile.toUtf8());

    if (jsReadDoc.isArray()) {
        qDebug() << "doc is array" << endl;
    }
    else {
        qDebug() << "doc is not an array" << endl;
    }

    if (jsReadDoc.isEmpty()) {
        qDebug() << "doc is empty" << endl;
    }
    else {
        qDebug() << "doc is not an empty" << endl;
    }

    QByteArray byteArray = jsReadDoc.toJson();
    qDebug() << byteArray << endl;



    QJsonObject jsObject;
    jsObject["username"] = username;
    jsObject["password"] = pswd;

    QJsonValue jsValue(jsObject);

    QJsonArray jsArray;
    jsArray.append(jsValue);




    file.close();
    if ( !file.open(QIODevice::WriteOnly/* | QIODevice::Text*/)) {
        qDebug() << "addUserProfile: failed to open file 2" << endl;
        return false;
    }

    QJsonDocument jsDoc(jsArray);
    file.write(jsDoc.toJson());
    file.close();


    //file.write(jsDoc.toJson());

    qDebug() << "username: " + username << endl;
    qDebug() << "password: " + pswd << endl;

    return true;
}

