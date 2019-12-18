#ifndef PASSW_H
#define PASSW_H

#include <QMainWindow>
#include <QFile>
#include "modules/createPassw/createpassw.h"
#include "modules/deletePassw/deletepassw.h"
#include "modules/userProfiles/userprofiles.h"

#define COLUMN_1   0
#define COLUMN_2   1
#define COLUMN_3   2
#define COLUMN_4   3
//#define COLUMN_NUM 4

#define DELTA 0                     // value to disable scrolling, when unnecessary in pwd table (height is ok, but scrolling is on bug)
#define EMPTY_PWD_WIND_HEIGHT 140   // height of the password window when table  is empty (no data)
#define MAX_VISIBLE_ROWS_NUM  15    // maximum visible rows of resources without scrolling
#define NOT_ACTIVATED         -1    // default state of table cell, meaning "No click done"



#define IS_CELL_ACTIVATED()   ((!this->sCellClicked.row.isEmpty()) &&          \
                               (this->sCellClicked.column != NOT_ACTIVATED) )

#define IS_ROW_ACTIVATED()    !this->sCellClicked.row.isEmpty()

#define ACTIVATE_ROW(rw)     do {                                             \
                                   if( !this->sCellClicked.row.contains(rw))  \
                                   {                                           \
                                     this->sCellClicked.row.push_back(rw);    \
                                    }                                          \
                              } while(0)

#define ACTIVATE_COL(col)     this->sCellClicked.column = col

#define DEACTIVATE_ROW()      this->sCellClicked.row.clear()

#define DEACTIVATE_CELL()     do { this->sCellClicked.row.clear();             \
                                  this->sCellClicked.column = NOT_ACTIVATED;   \
                              } while(0)



//define class for compilation purposes
class createPassw;    // form to add pwds
class deletePassw;    // form to delete pwds
class userProfiles;   // managing DB

struct cellActivated;

namespace Ui {
class passw;
}

/* Coordinated of a Cell */
struct cellActivated
{
    QVector<int> row;
    int column;
};

class passw : public QMainWindow
{
    Q_OBJECT

public:
    explicit passw(QWidget *parent = 0);
    explicit passw(QWidget *parent = 0, QString user="noUser");
    ~passw();

private:
    void  initActionsConnections(QWidget *parent);
    int   fillPwdTable(void);                                      // fill table of pwd,res, descr with data from users pwd .json file
    QSize getPwdTableMinSize(void);                                // Calculates an appropriate size for tableWidget
    void  resizeMainWindow(QSize sizeTable);                       // Resizes main window according to table dimensions

private slots:
    void  on_linePwdSearch_textChanged(const QString &arg1);       // when someone enters text
    void  openCreatePasswWindow(void);                             // opening new window to save new password
    void  openDeletePasswWindow(void);                             // opening new window to choose some password to delete
    void  goToSignIn(void);

    void  updatePwdTable(void);                                    // Refills passwords table
    void  clearPwdTable(void);                                     // clear table

    void  setCellActivated(int row, int column);                   // remembers activated cell coordinates (on table item clicked)
    void  deletePwdObject(void);                                   //
    void  onItemsSelectedChange(void);                             // Sets activated lines into sCellClicked structure

private:
    Ui::passw *ui;
    QWidget *signInWindow;

    struct cellActivated sCellClicked;            // holds coordinates of clicked cell

    QString CurrentUser;                          // current user
    QFile *filePasswords;
    QVector<QString> Resource;
    QVector<QString> Password;
    QVector<QString> Description;

    createPassw* newPassw;                        // class-form to add pwd
    deletePassw* delPassw;                        // class-form to delete pwd
    userProfiles *pUserProfiles;                  // to get pwds of current users
};



#endif // PASSW_H
