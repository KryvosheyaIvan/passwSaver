#ifndef PASSW_H
#define PASSW_H

#include <QMainWindow>
#include <QFile>
#include <QTableWidgetItem>
#include "modules/createPassw/createpassw.h"
#include "modules/deletePassw/deletepassw.h"
#include "modules/userProfiles/userprofiles.h"

#define COLUMN_1   0
#define COLUMN_2   1
#define COLUMN_3   2
#define COLUMN_4   3

#define DESCR_COLUMN COLUMN_2
#define LOCK_COLUMN  COLUMN_3
#define PWD_COLUMN   COLUMN_4
//#define COLUMN_NUM 4

#define DELTA                   0   // value to disable scrolling, when unnecessary in pwd table (height is ok, but scrolling is on bug)
#define EMPTY_PWD_WIND_HEIGHT 140   // height of the password window when table  is empty (no data)
#define MAX_VISIBLE_ROWS_NUM   15   // maximum visible rows of resources without scrolling
#define NOT_ACTIVATED          -1   // default state of table cell, meaning "No click done"



/* Macroses to work woth rows/columns selected by user */
#define IS_CELL_ACTIVATED()   ((!this->sCellSelected.row.isEmpty()) &&          \
                               (this->sCellSelected.column != NOT_ACTIVATED) )

#define IS_ROW_ACTIVATED()    !this->sCellSelected.row.isEmpty()

#define ACTIVATE_ROW(rw)     do {                                             \
                                   if( !this->sCellSelected.row.contains(rw))  \
                                   {                                           \
                                     this->sCellSelected.row.push_back(rw);    \
                                    }                                          \
                              } while(0)

#define ACTIVATE_COL(col)     this->sCellSelected.column = col

#define DEACTIVATE_ROW()      this->sCellSelected.row.clear()

#define DEACTIVATE_CELL()     do { this->sCellSelected.row.clear();             \
                                  this->sCellSelected.column = NOT_ACTIVATED;   \
                              } while(0)



//define class for compilation purposes
class createPassw;    // form to add pwds
class deletePassw;    // form to delete pwds
class userProfiles;   // managing DB

struct cellActivated;

namespace Ui {
class passw;
}

/* Coordinated of a Cell(s) selected */
struct cellActivated
{
    QVector<int> row;
    int column;
};

/* holds one object of DB */
typedef struct
{
    QString descr;
    QString lock;
    QString passw;
} oneDBObject;

typedef struct
{
    int row;
    int column;
} slocation;

/* Holds 1 object of original state
 * and 1 of changed
 * and its coordinates
 */
struct cellClicked
{
   oneDBObject original;  // original row
   oneDBObject afterEdit; // row after editing
   slocation   location;
};

class passw : public QMainWindow
{
    Q_OBJECT

public:
    explicit passw(QWidget *parent = 0);
    explicit passw(QWidget *parent = 0, QString user="noUser");
    ~passw();

private:
    void  initActionsConnections(void);
    int   fillPwdTable(void);                                      // fill table of pwd,res, descr with data from users pwd .json file
    QSize getPwdTableMinSize(void);                                // Calculates an appropriate size for tableWidget
    void  resizeMainWindow(QSize sizeTable);                       // Resizes main window according to table dimensions

private slots:
    void  on_linePwdSearch_textChanged(const QString &arg1);       // when someone enters text
    void  openCreatePasswWindow(void);                             // opening new window to save new password
    void  openDeletePasswWindow(void);                             // opening new window to choose some password to delete
    void  goToSignIn(void);                                        // closes current window and launch SignIn window

    void  updatePwdTable(void);                                    // Refills passwords table
    void  clearPwdTable(void);                                     // clear table
    void  deletePwdObject(void);                                   //

    /* Pwd table slots */
    void  setCellClicked(int row, int column);                     // remembers activated cell coordinates (on table item clicked)
    void  onItemsSelectedChange(void);                             // Sets activated lines into sCellClicked structur
    void  saveCellEdited(QTableWidgetItem* pItemEdited);           // Edit DB file (when user edits field in the table)

public:
    QString moduleName;

private:
    Ui::passw *ui;
    QWidget *signInWindow;

    struct cellActivated sCellSelected;           // holds coordinates of slected cell (to Delete if needed)
    struct cellClicked   sCellUnderEdit;          // hold original row and row after edit, alonf with its coordinates

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
