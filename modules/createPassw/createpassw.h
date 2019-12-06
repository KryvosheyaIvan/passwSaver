#ifndef CREATEPASSW_H
#define CREATEPASSW_H

#include <QDialog>
#include <QFile>
#include "modules/passwWindow/passw.h"
#include "modules/userProfiles/userprofiles.h"

//for compilation purposes
class userProfiles;

namespace Ui {
class createPassw;
}

class createPassw : public QDialog
{
    Q_OBJECT

public:
    explicit createPassw(QWidget *parent = nullptr); //default constructor (not used)
    explicit createPassw(QWidget *parent = nullptr, QString user = "noUser"); //
    ~createPassw();

private slots:
    void checkInputs(QWidget *parent = nullptr);

private:
    QString username;             //user for whom new password is created
    Ui::createPassw *ui;
    userProfiles *pUserProfiles;
    QFile *fileMain;
};

#endif // CREATEPASSW_H
