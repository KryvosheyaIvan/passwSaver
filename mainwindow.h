#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

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

private:
    Ui::MainWindow *ui;
    userProfiles *pUserProfiles;
    QFile *fileUsers;
};



#endif // MAINWINDOW_H
