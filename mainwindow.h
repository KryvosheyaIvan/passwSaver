#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTranslator>
#include <QEvent>
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

    /* Login button clicked */
    void on_loginButton_clicked(void);

    /* Register button clicked */
    void on_regButton_clicked(void);

    /* Eye button clicked */
    void on_toolButton_toggled(bool checked);

    /* Change button language clicked */
    void on_langButton_clicked(void);

protected:
    void changeEvent(QEvent* event) override;

private:
    Ui::MainWindow *ui;
    userProfiles *pUserProfiles;
    passw *pPassw;                                 // new window with passwords

    QTranslator translator;                        // to switch GUI between diff languages
};




#endif // MAINWINDOW_H
