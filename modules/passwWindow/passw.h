#ifndef PASSW_H
#define PASSW_H

#include <QMainWindow>
#include <QFile>
#include "modules/createPassw/createpassw.h"
#include "modules/userProfiles/userprofiles.h"

#define COLUMN_1 0
#define COLUMN_2 1
#define COLUMN_3 2
#define COLUMN_4 3

#define DELTA 2                 //value to disable scrolling, when unnecessary in pwd table (height is ok, but scrolling is on bug)

#define MAX_VISIBLE_ROWS_NUM 15 //maximum visible rows of resources without scrolling

//define class for compilation purposes
class createPassw;
class userProfiles;

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
    int fillPwdTable(void);                                        // fill table of pwd,res, descr with data from users pwd .json file
    QSize getPwdTableMinSize(void);                                // Calculates an appropriate size for tableWidget
    void resizeMainWindow(QSize sizeTable);                        // Resizes main window according to table dimensions

    QVector<QString> Resource;
    QVector<QString> Password;
    QVector<QString> Description;

private slots:
    void on_linePwdSearch_textChanged(const QString &arg1);        // when someone enters text
    void openCreatePasswWindow();                                  // opening new window to save new password

private:
    QString CurrentUser;                          //current user
    Ui::passw *ui;
    QFile *filePasswords;
    createPassw* newPassw;
    userProfiles *pUserProfiles;                  // to get pwas of current users
};

#endif // PASSW_H
