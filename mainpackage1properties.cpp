#include "mainpackage.h"
#include "ui_mainpackage.h"

// Вывод свойств
void MainPackage::updateToProperties()
{
    this->handlerToPackage->uploadProperties();

    QString packageName = handlerToPackage->properties["packageName"];
    QString pathToAnnotation = handlerToPackage->properties["pathToAnnotation"];
    QString pathToLiterature = handlerToPackage->properties["pathToLiterature"];
    QString pathToAuthor = handlerToPackage->properties["pathToAuthor"];
    QString pathToGlossary = handlerToPackage->properties["pathToGlossary"];

    ui->linePackageName->setText(packageName);
    ui->linePackageName->setProperty("backup", packageName);

    ui->linePathToAnnotation->setText(pathToAnnotation);
    ui->linePathToAnnotation->setProperty("backup", pathToAnnotation);

    ui->linePathToLiterature->setText(pathToLiterature);
    ui->linePathToLiterature->setProperty("backup", pathToLiterature);

    ui->linePathToAuthor->setText(pathToAuthor);
    ui->linePathToAuthor->setProperty("backup", pathToAuthor);

    ui->linePathToGlossary->setText(pathToGlossary);
    ui->linePathToGlossary->setProperty("backup", pathToGlossary);

    ui->buttonPropertiesSave->setEnabled(false);
    ui->buttonPropertiesUndo->setEnabled(false);
}

// Сохранение и откат свойств
void MainPackage::on_buttonPropertiesSave_clicked()
{
    this->handlerToPackage->properties["packageName"] = ui->linePackageName->text();
    this->handlerToPackage->properties["pathToAnnotation"] = ui->linePathToAnnotation->text();
    this->handlerToPackage->properties["pathToLiterature"] = ui->linePathToLiterature->text();
    this->handlerToPackage->properties["pathToAuthor"] = ui->linePathToAuthor->text();
    this->handlerToPackage->properties["pathToGlossary"] = ui->linePathToGlossary->text();
    this->handlerToPackage->unloadProperties();
    this->updateToProperties();
    this->hasChange();
}

void MainPackage::on_buttonPropertiesUndo_clicked()
{
    this->updateToProperties();
    this->hasChange();
}

// Имя пакета и выбор файлов
void MainPackage::on_linePackageName_textEdited(const QString &arg1)
{
    this->hasChangeToProperties();
    this->hasChange();
}

void MainPackage::on_buttonPathToAnnotation_clicked()
{
    QMap<QString, QVariant> argument;
    argument["titleDialog"] = "Выберите файл аннотации";
    argument["fileFormat"] = "*.pdf";
    argument["pathToPackage"] = this->handlerToPackage->pathToFile;
    this->getFilename(argument, this->ui->linePathToAnnotation);
}

void MainPackage::on_buttonPathToLiterature_clicked()
{
    QMap<QString, QVariant> argument;
    argument["titleDialog"] = "Выберите файл литературы";
    argument["fileFormat"] = "*.pdf";
    argument["pathToPackage"] = this->handlerToPackage->pathToFile;
    this->getFilename(argument, this->ui->linePathToLiterature);
}

void MainPackage::on_buttonPathToAuthor_clicked()
{
    QMap<QString, QVariant> argument;
    argument["titleDialog"] = "Выберите файл авторы";
    argument["fileFormat"] = "*.pdf";
    argument["pathToPackage"] = this->handlerToPackage->pathToFile;
    this->getFilename(argument, this->ui->linePathToAuthor);
}

void MainPackage::on_buttonPathToGlossary_clicked()
{
    QMap<QString, QVariant> argument;
    argument["titleDialog"] = "Выберите файл глоссария";
    argument["fileFormat"] = "*.pdf";
    argument["pathToPackage"] = this->handlerToPackage->pathToFile;
    this->getFilename(argument, this->ui->linePathToGlossary);
}

