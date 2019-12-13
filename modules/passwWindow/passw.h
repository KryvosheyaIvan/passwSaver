#ifndef PASSW_H
#define PASSW_H

#include <QMainWindow>
#include <QFile>
#include "modules/createPassw/createpassw.h"
#include "modules/deletePassw/deletepassw.h"
#include "modules/userProfiles/userprofiles.h"

#define COLUMN_1 0
#define COLUMN_2 1
#define COLUMN_3 2
#define COLUMN_4 3

#define DELTA 0                     // value to disable scrolling, when unnecessary in pwd table (height is ok, but scrolling is on bug)
#define EMPTY_PWD_WIND_HEIGHT 140   // height of the password window when table  is empty (no data)
#define MAX_VISIBLE_ROWS_NUM  15    // maximum visible rows of resources without scrolling

//define class for compilation purposes
class createPassw;    // form to add pwds
class deletePassw;    // form to delete pwds
class userProfiles;   // managing DB


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
    void  initActionsConnections();
    int   fillPwdTable(void);                                      // fill table of pwd,res, descr with data from users pwd .json file
    QSize getPwdTableMinSize(void);                                // Calculates an appropriate size for tableWidget
    void  resizeMainWindow(QSize sizeTable);                       // Resizes main window according to table dimensions

private slots:
    void on_linePwdSearch_textChanged(const QString &arg1);        // when someone enters text
    void openCreatePasswWindow(void);                              // opening new window to save new password
    void openDeletePasswWindow(void);                              // opening new window to choose some password to delete
    void updatePwdTable(void);                                     // Refills passwords table
    void clearPwdTable(void);                                      // clear table

private:
    Ui::passw *ui;

    QString CurrentUser;                          //current user
    QFile *filePasswords;
    QVector<QString> Resource;
    QVector<QString> Password;
    QVector<QString> Description;

    createPassw* newPassw;                        // class-form to add pwd
    deletePassw* delPassw;                        // class-form to delete pwd
    userProfiles *pUserProfiles;                  // to get pwas of current users
};

#endif // PASSW_H
