#ifndef PASSW_H
#define PASSW_H

#include <QMainWindow>
//#include <QMenuBar>
#include <QFile>
#include "modules/createPassw/createpassw.h"

//define class for compilation purposes
class createPassw;

namespace Ui {
class passw;
}

class passw : public QMainWindow
{
    Q_OBJECT

public:
    explicit passw(QWidget *parent = 0);
    explicit passw(QWidget *parent = 0, QString user="noUser");
    ~passw();

private:
    void initActionsConnections();

private slots:
    void on_linePwdSearch_textChanged(const QString &arg1);        //when someone enters text
    void openCreatePasswWindow();                                  // opening new window to save new password

private:
    QString CurrentUser;                          //current user
    Ui::passw *ui;
    QFile *filePasswords;
    createPassw* newPassw;
};

#endif // PASSW_H
