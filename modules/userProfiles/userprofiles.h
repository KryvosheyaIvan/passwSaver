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

    /* Work with App users profiles */
    void createUsersRegFile(void);                                                                                     // init file with app users
    bool addUserProfile(QString username, QString pswd, QWidget *parent);                                              // registration
    bool Login(QString username, QString pswd, QWidget *parent);                                                       // logging in

    /* Work with DB file */
    bool addLockKeyPair(   QString currUser, QString lock,   QString key,   QString description, QWidget *parent, QString &errMsg);     // add new resource and password
    bool deleteLockKeyPair(QString currUser, QString lock,   QString key,   QString description, QWidget *parent, QString &errMsg);     // remove existing resource and password

    bool replaceDBvalue(   QString currUser, QString lock,   QString key,   QString description,
                           QString newValue, int elementType, QWidget *parent, QString &errMsg);

    bool checkNewPassword(QString currUser, QString descr, QString lock, QString key1, QString key2, QWidget *parent, QString &errMsg); // check new password and resource for correctness


    QVector<QString> getArrayElement(QString strUsername, int elementType, QWidget *parent);                           // returns array of DB (one type)

private:
    bool isUsernameExist(QString username, QWidget *parent);                                    // ...in Users file
    bool isPswdExist(QString pswd, QWidget *parent);                                            // ...in Users file

    bool getPwdDB(QString username, QString &fileContent, QWidget *parent, QString &errMsg);                           // opens password databse and copies its content into QString
    bool rewritePwdDB(QString username, QJsonDocument &jsDoc, QString &errMsg);                                        // opens password databse and copies jsDocumnet into DB file

    bool isJSONvalid(QJsonDocument &jsDoc, QString &errMsg);                                                           // check json for validity

    QVector<int> getDataOccurences(QString currUser,int elementType,  QString strToCheck, QWidget *parent, QString &errMsg);            // store lines numbers of occurences of
                                                                                                                       // lock || key || descr of the DB into vector
    bool isSameFieldsExist( QVector<int> &descrIdxs, QVector<int> &loginIdxs, QVector<int> &pwdIdxs, QWidget *parent); // check whether current input object is unique (pwd DB)

    bool getJsUsersReg(QString &errReason, QJsonArray &jsArray);                                                       // Sets content of /users.json into jsArray

signals:

public slots:
};

#endif // USERPROFILES_H
