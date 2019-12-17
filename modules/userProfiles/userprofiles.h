#ifndef USERPROFILES_H
#define USERPROFILES_H

#include <QObject>
#include <QFile>

#define MIN_STRING_LEN 4             //min length for resource name and the password

class userProfiles : public QObject
{
    Q_OBJECT
public:
    explicit userProfiles(QObject *parent = 0);

    /* Constants for getArrayElement()
     * to choose what type of element must be returned
     */
    enum getTypeFlag {
        RESOURCE    = 0x0000,
        PASSWORD    = 0x0001,
        DESCRIPTION = 0x0002
    };

    bool addUserProfile(QString username, QString pswd, QWidget *parent);                                              // registration
    bool Login(QString username, QString pswd, QWidget *parent);                                                       // logging in
    bool addLockKeyPair(QString currUser, QString lock, QString key, QString description, QWidget *parent);            // add new resource and password
    bool checkNewPassword(QString currUser, QString descr, QString lock, QString key1, QString key2, QWidget *parent); // check new password and resource for correctness

    QVector<QString> getArrayElement(QString strUsername, int elementType, QWidget *parent);                           // returns array of DB (one type)

private:
    bool isUsernameExist(QString username, QWidget *parent);                                    // in Users file
    bool isPswdExist(QString pswd, QWidget *parent);                                            // in Users file

    QVector<int> getDataOccurences(QString currUser,int elementType,  QString strToCheck, QWidget *parent);            // store lines numbers of occurences of
                                                                                                                       // lock || key || descr of the DB into vector
    bool isSameFieldsExist( QVector<int> &descrIdxs, QVector<int> &loginIdxs, QVector<int> &pwdIdxs, QWidget *parent);    // check whether current input object is unique (pwd DB)

signals:

public slots:
};

#endif // USERPROFILES_H
