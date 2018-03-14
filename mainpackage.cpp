#include "mainpackage.h"
#include "ui_mainpackage.h"

MainPackage::MainPackage(HandlerToPackages *inpHandlerPackage, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainPackage)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/icon"));
    this->handlerToPackage = inpHandlerPackage;
    this->handlerToPackage->createBuffer();
    this->handlerToPackage->connectToDatabase();

    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    this->handlerToPackage->uploadProperties();
    this->updateToProperties();

    this->setProperty("isProgramInput", false);

    this->handlerToPackage->tableModels["topics"] = new QSqlTableModel(0, this->handlerToPackage->databaseTemporary);
    this->handlerToPackage->tableModels["topics"]->setTable("topics");
    this->handlerToPackage->tableModels["topics"]->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->handlerToPackage->tableModels["topics"]->setFilter("topicID > 0");
    this->handlerToPackage->tableModels["topics"]->setSort(2, Qt::AscendingOrder);
    this->ui->tableTopics->setModel(this->handlerToPackage->tableModels["topics"]);
    this->ui->tableTopics->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableTopics->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableTopics->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableTopics->verticalHeader()->hide();
    this->ui->tableTopics->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->updateTopics();

    this->handlerToPackage->tableModels["objects"] = new QSqlTableModel(0, this->handlerToPackage->databaseTemporary);
    this->handlerToPackage->tableModels["objects"]->setTable("objects");
    this->handlerToPackage->tableModels["objects"]->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->handlerToPackage->tableModels["objects"]->setSort(5, Qt::AscendingOrder);
    this->ui->tableObjects->setModel(this->handlerToPackage->tableModels["objects"]);
    this->ui->tableObjects->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableObjects->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableObjects->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableObjects->verticalHeader()->hide();
    this->ui->tableObjects->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->updateObjects();

    this->ui->comboObjectsFiltersType->addItem(OBJECT_TYPES_NAME["lecture"]);
    this->ui->comboObjectsFiltersType->addItem(OBJECT_TYPES_NAME["exercise"]);
    this->ui->comboObjectsFiltersType->addItem(OBJECT_TYPES_NAME["additional_material"]);
    this->ui->comboObjectsFiltersType->addItem(OBJECT_TYPES_NAME["presentation"]);
    this->ui->comboObjectsFiltersType->addItem(OBJECT_TYPES_NAME["test"]);
    this->ui->comboObjectsFiltersType->addItem(OBJECT_TYPES_NAME["video"]);

    this->ui->comboObjectsType->addItem(OBJECT_TYPES_NAME["lecture"]);
    this->ui->comboObjectsType->addItem(OBJECT_TYPES_NAME["exercise"]);
    this->ui->comboObjectsType->addItem(OBJECT_TYPES_NAME["additional_material"]);
    this->ui->comboObjectsType->addItem(OBJECT_TYPES_NAME["presentation"]);
    this->ui->comboObjectsType->addItem(OBJECT_TYPES_NAME["test"]);
    this->ui->comboObjectsType->addItem(OBJECT_TYPES_NAME["video"]);

//    this->generatePackage();
}

MainPackage::~MainPackage()
{
    this->handlerToPackage->destroyBuffer();
    this->handlerToPackage->databaseCore.close();
    delete ui;
}

void MainPackage::on_buttonAllSave_clicked()
{
    if(this->hasChangeToProperties())
        this->on_buttonPropertiesSave_clicked();

    if(this->hasChangeToTopics())
        this->on_buttonTopicsSave_clicked();

    if(this->hasChangeToObjects())
        this->on_buttonObjectsSave_clicked();
}

void MainPackage::on_buttonAllUndo_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Оболочка");
    msgBox.setText("<b>Вы уверены, что хотите отменить все изменения?</b>");
    msgBox.setInformativeText("<p style=\"color: grey;\">Будут отменены все "
                              "изменения с последнего сохранения</p>");

    msgBox.addButton("Отменить", QMessageBox::YesRole);
    msgBox.addButton(" Не отменять ", QMessageBox::NoRole);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QDialogButtonBox::NoRole) return;

    if(this->hasChangeToProperties())
        this->on_buttonPropertiesUndo_clicked();

    if(this->hasChangeToTopics())
        this->on_buttonTopicsUndo_clicked();

    if(this->hasChangeToObjects())
        this->on_buttonObjectsUndo_clicked();

    this->hasChange();
}

void MainPackage::on_buttonExit_clicked()
{
    bool isCheck = false;
    if (hasChangeToProperties() || hasChangeToTopics() || hasChangeToObjects())
        isCheck = true;

    if (isCheck)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Оболочка");
        msgBox.setText("<b>Вы хотите сохранить изменения?</b>");
        msgBox.setInformativeText("<p style=\"color: grey;\">Будут потеряны все "
                                  "изменения с последнего сохранения</p>");

        msgBox.addButton("Сохранить", QMessageBox::YesRole);
        msgBox.addButton(" Не сохранять ", QMessageBox::NoRole);
        msgBox.addButton("Отмена", QMessageBox::ResetRole);
        msgBox.setDefaultButton(QMessageBox::Cancel);

        int resultMsgBox = msgBox.exec();
        if (resultMsgBox == 2) return;
        if (resultMsgBox == 0)
            this->on_buttonAllSave_clicked();
    }

    this->handlerToPackage->destroyBuffer();
    this->handlerToPackage->databaseCore.close();
    this->close();
}

void MainPackage::on_buttonExitToMenu_clicked()
{
    bool isCheck = false;
    if (hasChangeToProperties() || hasChangeToTopics() || hasChangeToObjects())
        isCheck = true;

    if (isCheck)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Оболочка");
        msgBox.setText("<b>Вы хотите сохранить изменения?</b>");
        msgBox.setInformativeText("<p style=\"color: grey;\">Будут потеряны все "
                                  "изменения с последнего сохранения</p>");

        msgBox.addButton("Сохранить", QMessageBox::YesRole);
        msgBox.addButton(" Не сохранять ", QMessageBox::NoRole);
        msgBox.addButton("Отмена", QMessageBox::ResetRole);
        msgBox.setDefaultButton(QMessageBox::Reset);

        int resultMsgBox = msgBox.exec();
        qDebug() << resultMsgBox;
        if (resultMsgBox == 2) return;
        if (resultMsgBox == 0)
            this->on_buttonAllSave_clicked();
    }

    MainMenu *mainMenu = new MainMenu();
    mainMenu->show();
    this->close();
}

void MainPackage::getFilename(QMap<QString, QVariant> arguments,
                              QLineEdit* lineOutput)
{
    QString titleDialog = arguments["titleDialog"].toString();
    QString fileFormat = arguments["fileFormat"].toString();
    QString pathToPackage = arguments["pathToPackage"].toString();

    QString pathToFile = "";
    pathToFile += QFileDialog::getOpenFileName(this, titleDialog,
                                               pathToPackage, fileFormat);
    if (pathToFile == "") return;

    pathToFile = pathToFile.remove(pathToPackage + "/");
    lineOutput->setText(pathToFile);
    this->hasChange();
}

int MainPackage::getCountItems(QString tableName)
{
    QSqlQuery query(this->handlerToPackage->databaseTemporary);
    query.prepare("SELECT COUNT(*) FROM " + tableName);
    query.exec();
    query.next();
    int countItems = query.value(0).toInt();
    return countItems;
}

void MainPackage::on_buttonAbout_clicked()
{
    QMessageBox::information(0, "О программе", "    Учебная оболочка<br>"
                                               "    Версия: 1.0<br>");
}
