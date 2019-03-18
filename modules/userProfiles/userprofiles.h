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
    bool addUserProfile(QString username, QString pswd, QWidget *parent);

private:
    bool isUsernameExist(QString username);

signals:

public slots:
};

#endif // USERPROFILES_H
