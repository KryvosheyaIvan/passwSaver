#ifndef USERPROFILES_H
#define USERPROFILES_H

#include <QObject>
#include <QFile>

class userProfiles : public QObject
{
    Q_OBJECT
public:
    explicit userProfiles(QObject *parent = 0);
    void addUserProfile(QString username, QString pswd, QFile* dest);

signals:

public slots:
};

#endif // USERPROFILES_H
