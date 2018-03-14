#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <common.h>
#include <mainpackage.h>
#include <maintests.h>
#include <handlertotests.h>

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();

private slots:
    void on_buttonCreatePackage_clicked();
    void on_buttonOpenPackage_clicked();

    void on_buttonCreateTest_clicked();
    void on_buttonOpenTest_clicked();

    void on_buttonExit_clicked();

private:
    Ui::MainMenu *ui;
    HandlerToPackages *handlerPackage;
    HandlerToTests *handlerTest;
};

#endif // MAINMENU_H
