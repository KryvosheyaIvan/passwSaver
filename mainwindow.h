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

    /* Language constants */
    enum eLanguage
    {
        ENGLISH   = 0x01U,
        GERMAN    = 0x02U,
        UKRAINIAN = 0x03U
    } ;

private slots:
    void on_loginButton_clicked(void);

    void on_regButton_clicked(void);

    /* Eye button clicked */
    void on_toolButton_toggled(bool checked);

    void on_langButton_clicked(void);

private:
    Ui::MainWindow *ui;
    userProfiles *pUserProfiles;
    passw *pPassw;                                 //new window with passwords
};




#endif // MAINWINDOW_H
