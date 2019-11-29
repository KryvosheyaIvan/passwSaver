#ifndef PASSW_H
#define PASSW_H

#include <QMainWindow>
//#include <QMenuBar>

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
    Ui::passw *ui;
};

#endif // PASSW_H
