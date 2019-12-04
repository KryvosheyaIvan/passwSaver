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

passw::passw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::passw)
{
    ui->setupUi(this);
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

    /* Try to crate table */
    ui->tablePwd->setRowCount(2);
    ui->tablePwd->setColumnCount(4);
    ui->tablePwd->setHorizontalHeaderLabels(QStringList() << "Icon" <<"Lock" << "Key" << "Description" );

    //ui->tablePwd->horizontalHeader()->setMinimumSectionSize(10);

    ui->tablePwd->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tablePwd->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tablePwd->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    ui->tablePwd->horizontalHeader()->setStretchLastSection(true);

    ui->tablePwd->horizontalHeader()->resizeSection(0,5);

    /* Try to set icon*/
    QPixmap pix(":/img/eye.png");
    QIcon iconExample(pix.scaled(20,20,Qt::KeepAspectRatio));
    QTableWidgetItem *tableItem = new QTableWidgetItem;
    tableItem->setIcon(iconExample);
    tableItem->setTextAlignment(Qt::AlignCenter);
    //ui->tablePwd->item(0,0)->setTextAlignment(Qt::AlignRight);
    ui->tablePwd->setItem(0,0,tableItem);


    //ui->tablePwd->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    //QTableWidgetItem tableItem = ui->tablePwd->takeItem(0,0);
    //tableItem = ui->tablePwd->itemFromIndex(indexTable);

    newPassw = new createPassw;
    /* Slot --> Action */
    initActionsConnections();
}

passw::~passw()
{
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
   connect(ui->actionAdd, SIGNAL(triggered()), newPassw, SLOT(show()));
}

/* Open new window with New password Form*/
void passw::openCreatePasswWindow()
{
  newPassw->exec();
}
