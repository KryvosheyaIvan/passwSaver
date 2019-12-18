#include "passw.h"
#include "ui_passw.h"
#include <QPixmap>
#include <QMessageBox>
#include <QIcon>
#include <QTableWidgetItem>
#include <QModelIndex>
#include <QHeaderView>
#include <QtGui>
#include <QtCore>
#include <QMenuBar>
#include <QCheckBox>

#include "modules/createPassw/createpassw.h"
#include "modules/deletePassw/deletepassw.h"

/* Default constructor. Not used */
passw::passw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::passw)
{
    ui->setupUi(this);
}

/* Explicitly defined constructor with User input as a string*/
passw::passw(QWidget *parent, QString user) :
    QMainWindow(parent),
    ui(new Ui::passw)
{
    ui->setupUi(this);

    qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "passw created" << endl;

    /* Save pointer of the SignIn window to launch it, when necessary */
    signInWindow = parent;

    /* Set current username */
    CurrentUser = user;

    /* Set title */
    setWindowTitle(tr("Password Saver"));

    /* Set table cell activated nto default state */
    DEACTIVATE_CELL();

    /* Tool Bar*/
    //QToolBar *fileToolBar = addToolBar(tr("File"));
    //const QIcon exEye = QIcon::fromTheme("document-new", QIcon(":/img/eye.png"));
    //QAction *exAction = new QAction(exEye,tr("&New"),this);
    //exAction->setShortcut(QKeySequence::New);
    //exAction = fileToolBar->
    //toolBar->addAction(exAction);

    /* Add search picture */
    QPixmap pixSearch(":/img/search.png");
    ui->labelSearch->setPixmap(pixSearch);
    ui->labelSearch->setPixmap(pixSearch.scaled(60,60,Qt::KeepAspectRatio));

    /* Set grayed out text (tip)*/
    ui->linePwdSearch->setMaxLength(355);
    ui->linePwdSearch->setPlaceholderText("Enter resource here...");

    // fill table of pwd,res, descr with data from users pwd .json file
    fillPwdTable();

    // instanciate classes to add/remove data
    newPassw = new createPassw(this,CurrentUser);

    /* Slot --> Action */
    initActionsConnections(parent);
}

passw::~passw()
{
    qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "passw Destroyed" << endl;
    delete ui;
}

/* Triggered when someone changes taxt inside label */
void passw::on_linePwdSearch_textChanged(const QString &arg1)
{
   //debug QMessageBox::information(this,"ska","ska");
}

/* Slot --> Action */
void passw::initActionsConnections(QWidget *parent)
{
   connect(ui->actionAdd,    SIGNAL(triggered()),  this, SLOT(openCreatePasswWindow()));             // new window with a form to create new lock-key pair
   //connect(ui->actionDelete, SIGNAL(triggered()),  this, SLOT(openDeletePasswWindow()));             // new window with a form to delete some lock-key pair
   connect(ui->actionReload, SIGNAL(triggered()),  this, SLOT(updatePwdTable()));                    // reload database
   connect(ui->actionDelete, SIGNAL(triggered()),  this, SLOT(deletePwdObject()));
   connect(ui->actionExit,   SIGNAL(triggered()),  this, SLOT(goToSignIn()));

   //connect(ui->tablePwd,     SIGNAL(cellClicked(int,int)),  this,  SLOT(setCellActivated(int, int)));  // remember activated cell coordinates
   connect(ui->tablePwd,     SIGNAL(itemSelectionChanged()), this, SLOT(onItemsSelectedChange()));
}

void passw::goToSignIn(void)
{
  /*Show initial window*/
  signInWindow->show();

  /* Close current window */
  this->close();

  /* Free resources of this widget */
  delete this;
}

void passw::deletePwdObject(void)
{
    QString keyToDelete;
    QString lockToDelete;
    QString descrToDelete;

    QTableWidgetItem* itemDescrToDelete;
    QTableWidgetItem* itemLockToDelete;
    QTableWidgetItem* itemKeyToDelete;

    int idxRowToDelete;

    int i = 0;
    for( i = 0; i < this->sCellClicked.row.size(); i++)
    {
       idxRowToDelete =  sCellClicked.row.at(i);

       itemDescrToDelete = ui->tablePwd->item(idxRowToDelete, COLUMN_2);
       descrToDelete = itemDescrToDelete->text();

       itemLockToDelete = ui->tablePwd->item(idxRowToDelete, COLUMN_3);
       lockToDelete = itemLockToDelete->text();

       itemKeyToDelete = ui->tablePwd->item(idxRowToDelete, COLUMN_4);
       keyToDelete = itemKeyToDelete->text();

      // TO DO. Needed to get string of data to delete!
      pUserProfiles->deleteLockKeyPair(this->CurrentUser, lockToDelete, keyToDelete, descrToDelete, nullptr);
    }
    updatePwdTable();
    //qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "...SKA!" << endl;
  /*
    if (IS_ROW_ACTIVATED())
  {
     QMessageBox::information(this,"Clicked","Clicked!deactivate...");
     DEACTIVATE_CELL();
  }
  else
  {
    QMessageBox::information(this,"Clicked","Choose item to delete!");
  }
  */
}

/* Sets activated lines into sCellClicked structure
 * ( to delete them if needed)
 */
void passw::onItemsSelectedChange(void)
{

  QList<QTableWidgetSelectionRange> listRanges = ui->tablePwd->selectedRanges();

   //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "listRanges " << listRanges.size() << endl;

   int k = 0;
   for( k = 0; k < listRanges.size(); k++)
   {
      // get one range from the list of ranges
      QTableWidgetSelectionRange range = listRanges.at(k);

      if( range.topRow() == range.bottomRow() )
      {
          // one row selected.. (by user)

          int singleRow = range.topRow();

          // reset rows activated
          // bu preventing (if 1 row - no change)
          if( listRanges.size() == 1 ) { DEACTIVATE_ROW(); }

          //activate row
          ACTIVATE_ROW(singleRow);

          // set blue to single selected row (GUI)
          // if selected at least 1 cell, all row will be blue
          ui->tablePwd->setRangeSelected( QTableWidgetSelectionRange(singleRow, COLUMN_1, singleRow, COLUMN_4), true);
      }
      else // several rows selected... (by user)
      {
          // if user selected area without breaks
          // ex. several rows, but 1 area (without ctrl + click)
          if( listRanges.size() == 1 ) { DEACTIVATE_ROW(); }

          //add all selected rows into vector
          int idx = 0;

          // display number of selected rows
          //int numRowsSelected = range.bottomRow() - range.topRow() + 1;
          //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "numRowsSelected " << numRowsSelected << endl;

          // add every row to vector, going from top to bottom (increasing i)
          for( idx = range.topRow(); idx < (range.bottomRow() + 1) ; idx++)
          {
              ACTIVATE_ROW(idx);
          }

          // make several rows blue.
          ui->tablePwd->setRangeSelected( QTableWidgetSelectionRange(range.topRow(), COLUMN_1, range.bottomRow(), COLUMN_4), true);
      }
   }
   /* Debug session, displaying indexes of all selected rows
      qDebug() << "ROWS SELECTED ";
      int j = 0;
      for( j = 0; j < this->sCellClicked.row.size(); j++)
      {
        qDebug() <<  this->sCellClicked.row.at(j) ;
      }
  */

}

/* save clicked cell coordinates. Not used */
void passw::setCellActivated(int row, int column)
{
  qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "setCellActivated()" << endl;

  // save clicked cell coordinates
  ACTIVATE_ROW(row);
  ACTIVATE_COL(column);
}

/* Open new window with New password Form*/
void passw::openCreatePasswWindow(void)
{
    // execute newPassw class (old main window doesnt work)
    newPassw->exec();

    //update table
    this->updatePwdTable();
}

/* Open new window with New password Form*/
void passw::openDeletePasswWindow(void)
{
    delPassw = new deletePassw(this, this->CurrentUser , "lock");

    delPassw->exec();

    //update table
    this->updatePwdTable();

    // free resources
    delete delPassw;
}

/* fill table of pwd,res, descr with data from users pwd .json file */
int passw::fillPwdTable(void)
{
    /* Clearing vectors on update */
    if ( !this->Resource.isEmpty())
    {
        this->Resource.clear();
    }

    if ( !this->Password.isEmpty())
    {
        this->Password.clear();
    }

    if ( !this->Password.isEmpty())
    {
        this->Password.clear();
    }

    // getting resources array
    this->Resource    = pUserProfiles->getArrayElement(this->CurrentUser, userProfiles::RESOURCE,    this);
    this->Password    = pUserProfiles->getArrayElement(this->CurrentUser, userProfiles::PASSWORD,    this);
    this->Description = pUserProfiles->getArrayElement(this->CurrentUser, userProfiles::DESCRIPTION, this);

    //qVariant just for fun
    QVariant numRes   = this->Resource.length();
    QVariant numPwd   = this->Password.length();
    QVariant numDescr = this->Description.length();

    /* Password table creation */
    ui->tablePwd->setRowCount(numRes.value<int>());
    ui->tablePwd->setColumnCount(4);
    ui->tablePwd->setHorizontalHeaderLabels(QStringList() << tr("Icon") << tr("Description") << tr("Login") << tr("Password") );


    ui->tablePwd->horizontalHeader()->setSectionResizeMode(COLUMN_1,QHeaderView::Fixed);
    ui->tablePwd->horizontalHeader()->setSectionResizeMode(COLUMN_2,QHeaderView::Stretch);
    ui->tablePwd->horizontalHeader()->setSectionResizeMode(COLUMN_3,QHeaderView::Stretch);
    ui->tablePwd->horizontalHeader()->setStretchLastSection(true);

    //ui->tablePwd->horizontalHeader()->resizeSection(0,5);

    /* Set text values to appropriate table items */
    for(int row = 0; row < ui->tablePwd->rowCount(); row++)
    {
        QTableWidgetItem* itemRes = new QTableWidgetItem;
        QTableWidgetItem* itemPwd = new QTableWidgetItem;
        QTableWidgetItem* itemDsc = new QTableWidgetItem;

        itemDsc->setText(this->Description.value(row));
        ui->tablePwd->setItem(row,COLUMN_2,itemDsc);

        itemRes->setText(this->Resource.value(row));
        ui->tablePwd->setItem(row,COLUMN_3,itemRes);

        itemPwd->setText(this->Password.value(row));
        ui->tablePwd->setItem(row,COLUMN_4,itemPwd);
    }

    /* Calculate and set appropriate size for tableWidget */
    layout()->setSizeConstraint(QLayout::SetMinimumSize); //not sure if its helpful
    QSize sizeTablePwd = this->getPwdTableMinSize();
    ui->tablePwd->setMaximumSize(sizeTablePwd);
    ui->tablePwd->updateGeometry();

    //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "max height after" + QString::number(ui->tablePwd->maximumHeight()) << endl;
    //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "height after" + QString::number(ui->tablePwd->height()) << endl;

    //Resizes main window according to table dimensions *
    this->resizeMainWindow(sizeTablePwd);


    /* Setting icon working example
    QWidget *pWidget = new QWidget();
    QString iconSrc = ":/img/option.png";
    QSize sizeIcon(32,32);
    QLabel *label = new QLabel;
    label->setMaximumSize(sizeIcon);
    label->setScaledContents(true);
    label->setPixmap(QPixmap(iconSrc));
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(label);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0,0,0,0);
    pWidget->setLayout(pLayout);
    ui->tablePwd->setCellWidget(0,0,pWidget);
    */

    return 0;
}

/* Calculates an appropriate size for tableWidget
*
* brief: function gets current number of rows and gets bigger (vertically)
* according to it,
* but, if rows >= MAX_VISIBLE_ROWS_NUM,then
* scrollBar is appeared and height remains so,
* that only MAX_VISIBLE_ROWS_NU fits there
*/
QSize passw::getPwdTableMinSize(void)
{
   int width = ui->tablePwd->verticalHeader()->width() + DELTA;
   int height;

   //width is not important...
   for (int i = 0; i < ui->tablePwd->columnCount(); i++)
   {
      width += ui->tablePwd->columnWidth(i); // seems to include gridline (on my machine)
   }

   //get current height
   height = ui->tablePwd->horizontalHeader()->height() + DELTA;

   /* Limit height of the app window to MAX_VISIBLE_ROWS_NUM*/
   if(ui->tablePwd->rowCount() < MAX_VISIBLE_ROWS_NUM)
   {
     // enlarge height on 1 row size (increment)
     for (int j = 0; j < ui->tablePwd->rowCount(); j++)
     {
        height += ui->tablePwd->rowHeight(j);
     }
   }
   else
   {
      // set height on MAX_VISIBLE_ROWS_NUM*height and show scroll bar
      height += ui->tablePwd->rowHeight(0)*MAX_VISIBLE_ROWS_NUM;
      ui->tablePwd->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   }
   //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "rows " + QString::number(ui->tablePwd->rowCount()) << endl;
   //qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "height " + QString::number(height) << endl;

   return QSize(width, height);
}

/* Resizes main window according to table dimensions */
void passw::resizeMainWindow(QSize sizeTable)
{
   //get current size
   QSize appSize(this->width(), EMPTY_PWD_WIND_HEIGHT);

   //add height
   this->setMaximumHeight(EMPTY_PWD_WIND_HEIGHT + sizeTable.height());
   appSize.setHeight(EMPTY_PWD_WIND_HEIGHT + sizeTable.height());

   this->resize(appSize);

}

/* Refills passwords table */
void passw::updatePwdTable(void)
{
   //Clear table
    clearPwdTable();

    //fill table of pwd,res, descr with data from users pwd .json file
    fillPwdTable();

    QMessageBox::information(this,"Database reload","Updated!");
}

/* clear table*/
void passw::clearPwdTable(void)
{
    //clear all contents but leaves number of rows and columns and titles the same
    ui->tablePwd->clearContents();
}
















