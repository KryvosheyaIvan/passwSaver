#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "modules/passwWindow/passw.h"

class userProfiles;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();

    void on_regButton_clicked();

    /* Eye button clicked */
    void on_toolButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    userProfiles *pUserProfiles;
    QFile *fileUsers;
    passw *pPassw;                                 //new windows with passwords
};



#endif // MAINWINDOW_H
