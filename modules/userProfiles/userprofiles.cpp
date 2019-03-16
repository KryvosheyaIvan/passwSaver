#include "userprofiles.h"
#include <QMessageBox>
#include <QDebug>


userProfiles::userProfiles(QObject *parent) : QObject(parent)
{

}


void userProfiles::addUserProfile(QString username, QString pswd, QFile* dest)
{
    qDebug() << "username: " + username << endl;
    qDebug() << "password: " + pswd << endl;
}

