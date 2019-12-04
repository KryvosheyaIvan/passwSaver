#ifndef PASSW_H
#define PASSW_H

#include <QMainWindow>
//#include <QMenuBar>
#include <QFile>
#include "modules/createPassw/createpassw.h"


namespace Ui {
class passw;
}

class passw : public QMainWindow
{
    Q_OBJECT

public:
    explicit passw(QWidget *parent = 0);
    ~passw();

private:
    void initActionsConnections();

private slots:
    void on_linePwdSearch_textChanged(const QString &arg1);        //when someone enters text
    void openCreatePasswWindow();                                  // opening new window to save new password

private:
    Ui::passw *ui;
    QFile *filePasswords;
    createPassw* newPassw;
};

#endif // PASSW_H
