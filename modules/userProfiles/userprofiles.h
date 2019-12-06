#ifndef USERPROFILES_H
#define USERPROFILES_H

#include <QObject>
#include <QFile>

#define MIN_STRING_LEN 4

class userProfiles : public QObject
{
    Q_OBJECT
public:
    explicit userProfiles(QObject *parent = 0);

    bool addUserProfile(QString username, QString pswd, QWidget *parent);                                   // registration
    bool Login(QString username, QString pswd, QWidget *parent);                                            // logging in
    bool addLockKeyPair(QString currUser, QString lock, QString key, QString description, QWidget *parent); // add new resource and password
    bool checkNewPassword(QString lock, QString key1, QString key2, QWidget *parent);                       // check new password and resource for correctness

private:
    bool isUsernameExist(QString username, QWidget *parent);
    bool isPswdExist(QString pswd, QWidget *parent);

signals:

public slots:
};

#endif // USERPROFILES_H
