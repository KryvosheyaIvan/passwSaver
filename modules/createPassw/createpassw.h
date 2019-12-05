#ifndef CREATEPASSW_H
#define CREATEPASSW_H

#include <QDialog>
#include <QFile>

namespace Ui {
class createPassw;
}

class createPassw : public QDialog
{
    Q_OBJECT

public:
    explicit createPassw(QWidget *parent = nullptr);
    ~createPassw();

private:
    Ui::createPassw *ui;
    QFile *fileMain;
};

#endif // CREATEPASSW_H
