#ifndef DELETEPASSW_H
#define DELETEPASSW_H

#include <QDebug>
#include <QDialog>
#include <QWidget>

namespace Ui {
class deletePassw;
}

class deletePassw : public QDialog
{
    Q_OBJECT

public:
    explicit deletePassw(QWidget *parent = nullptr);                                                      // default constructor
    explicit deletePassw(QWidget *parent = nullptr, QString user = "noUser", QString lock = "noLock");    // gets name of resource to delete

    ~deletePassw();

private:
    Ui::deletePassw *ui;

    QString lockToDelete;      // name of lock to be deleted
    QString username;          // current username, which DB to remove from
};

#endif // DELETEPASSW_H
