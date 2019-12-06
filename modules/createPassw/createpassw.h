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
    explicit createPassw(QWidget *parent = nullptr);
    ~createPassw();

private slots:
    void checkInputs(void);

private:
    Ui::createPassw *ui;
    userProfiles *pUserProfiles;
    QFile *fileMain;
};

#endif // CREATEPASSW_H
