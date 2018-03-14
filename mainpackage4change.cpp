#include "mainpackage.h"
#include "ui_mainpackage.h"

// Проверка на наличие изменения свойств
bool MainPackage::hasChangeToProperties()
{
    bool isCheckToChange = false;
    QString backup = "";

    backup = this->ui->linePackageName->property("backup").toString();
    if (this->ui->linePackageName->text() != backup)
        isCheckToChange = true;

    backup = this->ui->linePathToAnnotation->property("backup").toString();
    if (this->ui->linePathToAnnotation->text() != backup)
        isCheckToChange = true;

    backup = this->ui->linePathToLiterature->property("backup").toString();
    if (this->ui->linePathToLiterature->text() != backup)
        isCheckToChange = true;

    backup = this->ui->linePathToAuthor->property("backup").toString();
    if (this->ui->linePathToAuthor->text() != backup)
        isCheckToChange = true;

    backup = this->ui->linePathToGlossary->property("backup").toString();
    if (this->ui->linePathToGlossary->text() != backup)
        isCheckToChange = true;

    this->ui->buttonPropertiesSave->setEnabled(isCheckToChange);
    this->ui->buttonPropertiesUndo->setEnabled(isCheckToChange);
    return isCheckToChange;
}

// Проверка на наличие изменения тем
bool MainPackage::hasChangeToTopics()
{
    QSqlQuery query(this->handlerToPackage->databaseTemporary);
    query.exec("SELECT * FROM topics EXCEPT SELECT * FROM backupTopics");
    int count = 0;
    while(query.next())
        ++count;

    if (count == 0)
    {
        this->ui->buttonTopicsSave->setEnabled(false);
        this->ui->buttonTopicsUndo->setEnabled(false);
        return false;
    }
    this->ui->buttonTopicsSave->setEnabled(true);
    this->ui->buttonTopicsUndo->setEnabled(true);
    return true;
}

// Проверка на наличие изменение объектов
bool MainPackage::hasChangeToObjects()
{
    QSqlQuery query(this->handlerToPackage->databaseTemporary);
    query.exec("SELECT * FROM objects EXCEPT SELECT * FROM backupObjects");
    int count = 0;
    while(query.next())
        ++count;
    if (count == 0)
    {
        this->ui->buttonObjectsSave->setEnabled(false);
        this->ui->buttonObjectsUndo->setEnabled(false);
        return false;
    }
    this->ui->buttonObjectsSave->setEnabled(true);
    this->ui->buttonObjectsUndo->setEnabled(true);
    return true;
}

// Проверка на все изменения
void MainPackage::hasChange()
{
    bool isCheckToChange = false;
    if (hasChangeToProperties() || hasChangeToTopics() || hasChangeToObjects())
        isCheckToChange = true;    
    ui->buttonAllSave->setEnabled(isCheckToChange);
    ui->buttonAllUndo->setEnabled(isCheckToChange);
}
