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

#include "modules/createPassw/createpassw.h"

/* Default constructor */
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

    /* Set current username */
    CurrentUser = user;

    /* Set title */
    setWindowTitle(tr("Password Saver"));

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

    fillPwdTable();





/*
    QMessageBox::information(this,"Debug","NumRes: " + numRes.toString() + "\n" +
                                          "NumPwd: " + numPwd.toString() + "\n" +
                                          "NumDescr: " + numDescr.toString()
                             );
*/



    //ui->tablePwd->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    //QTableWidgetItem tableItem = ui->tablePwd->takeItem(0,0);
    //tableItem = ui->tablePwd->itemFromIndex(indexTable);

    newPassw = new createPassw(this,CurrentUser);
    /* Slot --> Action */
    initActionsConnections();
}

passw::~passw()
{
    qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << "...Dead" << endl;
    delete ui;
}

/* Triggered when someone changes taxt inside label */
void passw::on_linePwdSearch_textChanged(const QString &arg1)
{
   //debug QMessageBox::information(this,"ska","ska");
}

/* Slot --> Action */
void passw::initActionsConnections()
{
   connect(ui->actionAdd, SIGNAL(triggered()), newPassw, SLOT(show()));  //new window with a form to create new lock-key pair
}

/* Open new window with New password Form*/
void passw::openCreatePasswWindow()
{
    //newPassw->exec();
    newPassw->show();
}

/* fill table of pwd,res, descr with data from users pwd .json file */
int passw::fillPwdTable(void)
{
    // getting resources array
    this->Resource    = pUserProfiles->getArrayElement(this->CurrentUser, userProfiles::RESOURCE,    this);
    this->Password    = pUserProfiles->getArrayElement(this->CurrentUser, userProfiles::PASSWORD,    this);
    this->Description = pUserProfiles->getArrayElement(this->CurrentUser, userProfiles::DESCRIPTION, this);

    QVariant numRes   = this->Resource.length();
    int numPwd   = this->Password.length();
    QVariant numDescr = this->Description.length();

    /* Try to create table */
    ui->tablePwd->setRowCount(numRes.value<int>());
    ui->tablePwd->setColumnCount(4);
    ui->tablePwd->setHorizontalHeaderLabels(QStringList() << "Icon" <<"Lock" << "Key" << "Description" );

    //ui->tablePwd->horizontalHeader()->setMinimumSectionSize(10);

    ui->tablePwd->horizontalHeader()->setSectionResizeMode(COLUMN_1,QHeaderView::Fixed);
    ui->tablePwd->horizontalHeader()->setSectionResizeMode(COLUMN_2,QHeaderView::Stretch);
    ui->tablePwd->horizontalHeader()->setSectionResizeMode(COLUMN_3,QHeaderView::Stretch);
    ui->tablePwd->horizontalHeader()->setStretchLastSection(true);

    ui->tablePwd->horizontalHeader()->resizeSection(0,5);

    /* Set text values to appropriate table items */
    for(int row = 0; row < ui->tablePwd->rowCount(); row++)
    {
        QTableWidgetItem* itemRes = new QTableWidgetItem;
        QTableWidgetItem* itemPwd = new QTableWidgetItem;
        QTableWidgetItem* itemDsc = new QTableWidgetItem;

        itemRes->setText(this->Resource.value(row));
        ui->tablePwd->setItem(row,COLUMN_2,itemRes);

        itemPwd->setText(this->Password.value(row));
        ui->tablePwd->setItem(row,COLUMN_3,itemPwd);

        itemDsc->setText(this->Description.value(row));
        ui->tablePwd->setItem(row,COLUMN_4,itemDsc);
    }

    /* Calculate and set appropriate size for tableWidget */
    layout()->setSizeConstraint(QLayout::SetMinimumSize);
    QSize size = this->getPwdTableMinSize();
    ui->tablePwd->setMaximumSize(size);

    /* Try to set icon*/
    /*
    QPixmap pix(":/img/eye.png");
    QIcon iconExample(pix.scaled(20,20,Qt::KeepAspectRatio));
    QTableWidgetItem *tableItem = new QTableWidgetItem;
    tableItem->setIcon(iconExample);
    tableItem->setTextAlignment(Qt::AlignCenter);
    //ui->tablePwd->item(0,0)->setTextAlignment(Qt::AlignRight);
    ui->tablePwd->setItem(0,0,tableItem);
    */
    return 0;
}

/* Calculates an appropriate size for tableWidget */
QSize passw::getPwdTableMinSize(void)
{
   int width = ui->tablePwd->verticalHeader()->width() + DELTA;
   int height;

   for (int i = 0; i < ui->tablePwd->columnCount(); i++)
   {
      width += ui->tablePwd->columnWidth(i); // seems to include gridline (on my machine)
   }
   height = ui->tablePwd->horizontalHeader()->height() + DELTA;

   for (int j = 0; j < ui->tablePwd->rowCount(); j++)
   {
      height += ui->tablePwd->rowHeight(j);
   }
   return QSize(width, height);
}






















