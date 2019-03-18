#ifndef USERPROFILES_H
#define USERPROFILES_H

#include <QObject>
#include <QFile>

#define MIN_STRING_LEN 5

class userProfiles : public QObject
{
    Q_OBJECT
public:
    explicit userProfiles(QObject *parent = 0);
    bool addUserProfile(QString username, QString pswd, QWidget *parent);       // registration
    bool Login(QString username, QString pswd, QWidget *parent);                // logging in

private:
    bool isUsernameExist(QString username);
    bool isPswdExist(QString pswd);

signals:

public slots:
};

#endif // USERPROFILES_H
