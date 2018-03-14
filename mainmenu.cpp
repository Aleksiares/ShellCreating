#include "mainmenu.h"
#include "ui_mainmenu.h"

void openMainPackage();

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/icon"));
    this->setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint);
    handlerPackage = new HandlerToPackages();
    handlerTest = new HandlerToTests();
}

MainMenu::~MainMenu()
{
    delete ui;
}

// Создание нового пакета и открытие формы пакетов
void MainMenu::on_buttonCreatePackage_clicked()
{
    if (!handlerPackage->createPackage())
    {
        ui->statusBar->setStyleSheet("color: red;");
        ui->statusBar->showMessage(this->handlerPackage->lastError, 3000);
        return;
    };

    MainPackage *mainPackage = new MainPackage(handlerPackage);
    mainPackage->show();

    this->on_buttonExit_clicked();
}

// Выбор пакета и открытие формы пакетов
void MainMenu::on_buttonOpenPackage_clicked()
{
    if (!handlerPackage->openPackage())
    {
        ui->statusBar->setStyleSheet("color: red;");
        ui->statusBar->showMessage(this->handlerPackage->lastError, 3000);
        return;
    };

    MainPackage *mainPackage = new MainPackage(handlerPackage);
    mainPackage->show();

    this->on_buttonExit_clicked();
}

// Создание теста и открытие формы тестов
void MainMenu::on_buttonCreateTest_clicked()
{
    if (!handlerTest->createTest())
    {
        ui->statusBar->setStyleSheet("color: red;");
        ui->statusBar->showMessage(this->handlerTest->lastError, 3000);
        return;
    };

    MainTests *mainTest = new MainTests(handlerTest);
    mainTest->show();

    this->on_buttonExit_clicked();
}

// Выбор теста и открытие формы тестов
void MainMenu::on_buttonOpenTest_clicked()
{
    if (!handlerTest->openTest())
    {
        ui->statusBar->setStyleSheet("color: red;");
        ui->statusBar->showMessage(this->handlerTest->lastError, 3000);
        return;
    };

    MainTests *mainTest = new MainTests(handlerTest, 0);
    mainTest->show();

    this->on_buttonExit_clicked();
}

// Закрытие приложения
void MainMenu::on_buttonExit_clicked()
{
    this->close();
}
