#include "mainpackage.h"
#include "ui_mainpackage.h"

void MainPackage::updateObjects(bool isFullUpdate, int indexRow)
{       
    this->setProperty("isProgramInput", true);

    this->handlerToPackage->tableModels["objects"]->setFilter(this->objectsFilter);
    this->handlerToPackage->tableModels["objects"]->select();
    this->handlerToPackage->tableModels["objects"]->setHeaderData(0, Qt::Horizontal, "ID");
    this->handlerToPackage->tableModels["objects"]->setHeaderData(1, Qt::Horizontal, "Название объекта");
    this->handlerToPackage->tableModels["objects"]->setHeaderData(2, Qt::Horizontal, "Тип объекта");
    this->handlerToPackage->tableModels["objects"]->setHeaderData(3, Qt::Horizontal, "Путь к объекту");
    this->handlerToPackage->tableModels["objects"]->setHeaderData(4, Qt::Horizontal, "ID темы");
    this->handlerToPackage->tableModels["objects"]->setHeaderData(5, Qt::Horizontal, "Порядковый номер");

    this->ui->tableObjects->setColumnHidden(0, true);
    this->ui->tableObjects->setColumnHidden(2, true);
    this->ui->tableObjects->setColumnHidden(3, true);
    this->ui->tableObjects->setColumnHidden(4, true);
    this->ui->tableObjects->setColumnHidden(5, true);
    this->ui->tableObjects->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    this->ui->buttonObjectsAdd->setEnabled(true);

    if (isFullUpdate)
    {
        this->currentObject.id = -1;
        this->currentObject.name = "";
        this->currentObject.type = "";
        this->currentObject.pathToFile = "";
        this->currentObject.topicId = 0;
        this->currentObject.indexNumber = 1;

        this->ui->lineObjectsName->setText("");
        this->ui->lineObjectsName->setProperty("backup", "");

        this->ui->comboObjectsTopic->setCurrentIndex(0);
        this->ui->comboObjectsTopic->setProperty("backup", 0);

        this->ui->comboObjectsType->setCurrentIndex(0);
        this->ui->comboObjectsType->setProperty("backup", 0);

        this->ui->lineObjectsPathToFile->setText("");
        this->ui->lineObjectsPathToFile->setProperty("backup", "");

        this->ui->spinObjectsIndexNumber->setValue(1);
        this->ui->spinObjectsIndexNumber->setProperty("backup", 1);
        int countObjects = this->getCountItems("objects");
        this->ui->spinObjectsIndexNumber->setMaximum(countObjects);
        this->ui->spinObjectsIndexNumber->setMinimum(1);

        ui->lineObjectsName->setEnabled(false);
        ui->comboObjectsTopic->setEnabled(false);
        ui->comboObjectsType->setEnabled(false);
        ui->spinObjectsIndexNumber->setEnabled(false);
        ui->buttonObjectsPathToFile->setEnabled(false);
        ui->buttonObjectsRemove->setEnabled(false);
    }

    this->setProperty("isProgramInput", false);
}

void MainPackage::generateFilterObjects()
{
    this->setProperty("isProgramInput", true);
    this->objectsFilter = "";
    if (this->ui->checkObjectsFilterTopics->isChecked())
    {
        this->objectsFilter += "objectTopicID='";
        this->ui->comboObjectsFiltersTopic->setModelColumn(0);
        this->objectsFilter += this->ui->comboObjectsFiltersTopic->currentText();
        this->objectsFilter += "'";
        this->ui->comboObjectsFiltersTopic->setModelColumn(1);
    }

    if (this->ui->checkObjectsFilterTopics->isChecked() && this->ui->checkObjectsFilterTypes->isChecked())
    {
        this->objectsFilter += " AND ";
    }

    if (this->ui->checkObjectsFilterTypes->isChecked())
    {
        this->objectsFilter += "objectType='";
        this->objectsFilter += this->ui->comboObjectsFiltersType->currentText();
        this->objectsFilter += "'";
    }

    updateObjects();
}

void MainPackage::on_tableObjects_clicked(const QModelIndex &index)
{
    this->setProperty("isProgramInput", true);

    this->objectsCurrentRow = index.row();

    this->currentObject.id = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 0)).toInt();
    this->currentObject.name = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 1)).toString();
    this->currentObject.type = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 2)).toString();
    this->currentObject.pathToFile = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 3)).toString();;
    this->currentObject.topicId = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 4)).toInt();;
    this->currentObject.indexNumber = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 5)).toInt();;

    this->ui->lineObjectsName->setText(this->currentObject.name);
    this->ui->lineObjectsName->setProperty("backup", this->currentObject.name);

    this->ui->comboObjectsTopic->setModelColumn(0);
    this->ui->comboObjectsTopic->setCurrentText(QString::number(currentObject.topicId));
    this->ui->comboObjectsTopic->setProperty("backup", currentObject.topicId);
    this->ui->comboObjectsTopic->setModelColumn(1);

    this->ui->comboObjectsType->setCurrentText(currentObject.type);
    this->ui->comboObjectsType->setProperty("backup", currentObject.type);
    if (currentObject.type == "Видео")
        this->ui->labelObjectsFormat->setText("Файл должен быть в формате *.avi или *.wmv");
    else if (currentObject.type == "Тест")
        this->ui->labelObjectsFormat->setText("Файл должен быть в формате *.test");
    else
        this->ui->labelObjectsFormat->setText("Файл должен быть в формате *.pdf");

    this->ui->lineObjectsPathToFile->setText(currentObject.pathToFile);
    this->ui->lineObjectsPathToFile->setProperty("backup", currentObject.pathToFile);

    this->ui->spinObjectsIndexNumber->setValue(currentObject.indexNumber);
    this->ui->spinObjectsIndexNumber->setProperty("backup", currentObject.indexNumber);

    ui->lineObjectsName->setEnabled(true);
    ui->comboObjectsTopic->setEnabled(true);
    ui->comboObjectsType->setEnabled(true);
    ui->spinObjectsIndexNumber->setEnabled(true);
    ui->buttonObjectsPathToFile->setEnabled(true);
    ui->buttonObjectsRemove->setEnabled(true);

    this->setProperty("isProgramInput", false);
}

void MainPackage::temporarySaveObject()
{    
    this->setProperty("isProgramInput", true);

    if(this->ui->lineObjectsName->text() != this->ui->lineObjectsName->property("backup").toString())
    {
        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("UPDATE objects SET objectName=:name WHERE objectID=:id");
        query.bindValue(":name", this->ui->lineObjectsName->text());
        query.bindValue(":id", this->currentObject.id);
        query.exec();
        this->currentObject.name = ui->lineObjectsName->text();
        ui->lineObjectsName->setProperty("backup", this->currentObject.name);
    }

    this->ui->comboObjectsTopic->setModelColumn(0);
    if (this->ui->comboObjectsTopic->currentText() !=
            this->ui->comboObjectsTopic->property("backup").toString())
    {
        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("UPDATE objects SET objectTopicID=:id1 WHERE objectID=:id2");
        query.bindValue(":id1", this->ui->comboObjectsTopic->currentText());
        query.bindValue(":id2", this->currentObject.id);
        query.exec();
        this->currentObject.topicId = this->ui->comboObjectsTopic->currentText().toInt();
        ui->comboObjectsTopic->setProperty("backup", this->currentObject.topicId);
    }
    this->ui->comboObjectsTopic->setModelColumn(1);

    if (this->ui->comboObjectsType->currentText() !=
            this->ui->comboObjectsType->property("backup").toString())
    {
        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("UPDATE objects SET objectType=:type WHERE objectID=:id");
        query.bindValue(":type", this->ui->comboObjectsType->currentText());
        query.bindValue(":id", this->currentObject.id);
        query.exec();
        this->currentObject.type = this->ui->comboObjectsType->currentText();
        ui->comboObjectsType->setProperty("backup", this->currentObject.type);
    }

    if(this->ui->lineObjectsPathToFile->text() !=
            this->ui->lineObjectsPathToFile->property("backup").toString())
    {
        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("UPDATE objects SET objectPathToFile=:pathToFile WHERE objectID=:id");
        query.bindValue(":pathToFile", this->ui->lineObjectsPathToFile->text());
        query.bindValue(":id", this->currentObject.id);
        query.exec();
        this->currentObject.pathToFile = this->ui->lineObjectsPathToFile->text();
        ui->lineObjectsPathToFile->setProperty("backup", this->currentObject.pathToFile);
    }

    if (this->ui->spinObjectsIndexNumber->value() !=
            this->ui->spinObjectsIndexNumber->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("UPDATE objects SET objectIndexNumber=:index WHERE objectID=:id");
        query.bindValue(":index", ui->spinObjectsIndexNumber->value());
        query.bindValue(":id", this->currentObject.id);
        query.exec();

        query.prepare("UPDATE objects SET objectIndexNumber=:index1 "
                      "WHERE objectIndexNumber=:index2 AND objectID!=:id");
        query.bindValue(":index1", ui->spinObjectsIndexNumber->property("backup").toInt());
        query.bindValue(":index2", ui->spinObjectsIndexNumber->value());
        query.bindValue(":id", this->currentObject.id);
        query.exec();

        this->currentObject.indexNumber = this->ui->spinObjectsIndexNumber->value();
        ui->spinObjectsIndexNumber->setProperty("backup", this->currentObject.indexNumber);
        this->objectsCurrentRow = this->ui->spinObjectsIndexNumber->value() - 1;
    }

    this->hasChange();

    this->setProperty("isProgramInput", false);
}

void MainPackage::generatePackage()
{
    const int countTopic = 11;

    for (int i = 1; i <= countTopic; ++i)
    {
        QString currentPath = this->handlerToPackage->pathToFile + "/Тема " + QString::number(i);

//      QList<QString> typesObject = {"Лекция", "Задание", "Презентация", "Тест"};
        QList<QString> typesObject = {"Лекция", "Задание", "Презентация"};

        QDir currentDir;
        currentDir.setCurrent(currentPath);
        QStringList arrayFiles = currentDir.entryList();

        for (int k = 0; k < arrayFiles.size(); ++k)
        {
            if (arrayFiles[k] == "." || arrayFiles[k] == "..")
                continue;

            QString name = arrayFiles[k];
            name = name.replace(".pdf", "");

            QString type = arrayFiles[k];

            int arg1 = type.indexOf(" ");
            int arg2 = type.length() - arg1;
            type.remove(arg1, arg2);

            QSqlQuery query(this->handlerToPackage->databaseTemporary);
            query.prepare("INSERT INTO objects(objectID, objectName, objectType, "
                       "objectPathToFile, objectTopicID, objectIndexNumber) "
                       "VALUES (NULL, :name, :type, :path, :topic, (SELECT count(*) FROM  objects)+1)");
            query.bindValue(":name", name);
            query.bindValue(":type", type);

            query.bindValue(":path", "Тема " + QString::number(i) + "/" + arrayFiles[k]);
            query.bindValue(":topic", i);
            query.exec();
        }
    }

    QDir dir(this->handlerToPackage->pathToFile + "/Видео");
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString name = fileInfo.baseName();
        QString fileName = fileInfo.fileName();
        if (name == "") continue;
        QString path = "Видео/" + fileName;

        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("INSERT INTO objects(objectID, objectName, objectType, "
                   "objectPathToFile, objectTopicID, objectIndexNumber) "
                   "VALUES (NULL, :name, 'Видео', :path, 0, (SELECT count(*) FROM  objects)+1)");
        query.bindValue(":name", name);
        query.bindValue(":path", path);
        query.exec();
    }

    QDir dir2(this->handlerToPackage->pathToFile + "/Дополнительный материал");
    QFileInfoList list2 = dir2.entryInfoList();
    for (int i = 0; i < list2.size(); ++i)
    {
        QFileInfo fileInfo = list2.at(i);
        QString name = fileInfo.baseName();
        QString fileName = fileInfo.fileName();
        if (name == "") continue;
        QString path = "Дополнительный материал/" + fileName;

        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("INSERT INTO objects(objectID, objectName, objectType, "
                   "objectPathToFile, objectTopicID, objectIndexNumber) "
                   "VALUES (NULL, :name, 'Дополнительные материалы', :path, 0, (SELECT count(*) FROM  objects)+1)");
        query.bindValue(":name", name);
        query.bindValue(":path", path);
        query.exec();
    }

    this->updateObjects();
    ui->buttonObjectsRemove->setEnabled(true);
    ui->buttonObjectsSave->setEnabled(true);
    ui->buttonObjectsUndo->setEnabled(true);
    this->hasChange();
}

// Отображение панели фильтрации
void MainPackage::on_buttonObjectsShowFilter_clicked(bool checked)
{
    if (checked) ui->buttonObjectsShowFilter->setText("Скрыть фильтры");
    else ui->buttonObjectsShowFilter->setText("Отобразить фильтры");
    ui->groupFilters->setVisible(checked);
}

// Включение и отключение фильтра по теме
void MainPackage::on_checkObjectsFilterTopics_clicked(bool checked)
{
    this->ui->comboObjectsFiltersTopic->setEnabled(checked);
    this->generateFilterObjects();
}

// Включение и отключение фильтра по типу
void MainPackage::on_checkObjectsFilterTypes_clicked(bool checked)
{
    this->ui->comboObjectsFiltersType->setEnabled(checked);
    this->generateFilterObjects();
}

// Фильтр по теме
void MainPackage::on_comboObjectsFiltersTopic_currentIndexChanged(int index)
{
    if (this->property("isProgramInput").toBool()) return;
    generateFilterObjects();
}

// Фильтр по типу
void MainPackage::on_comboObjectsFiltersType_currentIndexChanged(int index)
{
    if (this->property("isProgramInput").toBool()) return;
    generateFilterObjects();
}

// Добавление нового объекта
void MainPackage::on_buttonObjectsAdd_clicked()
{
    this->setProperty("isProgramInput", true);

    QSqlQuery query(this->handlerToPackage->databaseTemporary);
    query.exec("INSERT INTO objects(objectID, objectName, objectType, "
               "objectPathToFile, objectTopicID, objectIndexNumber) "
               "VALUES (NULL, '', 'Лекции', '', 0, (SELECT count(*) FROM  objects)+1)");

    this->updateObjects();
    this->setProperty("isProgramInput", true);

    this->objectsCurrentRow = this->ui->tableObjects->model()->rowCount();
    --this->objectsCurrentRow;
    this->ui->tableObjects->selectRow(this->objectsCurrentRow);

    this->currentObject.id = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 0)).toInt();
    this->currentObject.name = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 1)).toString();
    this->currentObject.type = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 2)).toString();
    this->currentObject.pathToFile = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 3)).toString();;
    this->currentObject.topicId = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 4)).toInt();;
    this->currentObject.indexNumber = this->ui->tableObjects->model()->
            data(this->ui->tableObjects->model()->index(this->objectsCurrentRow, 5)).toInt();;

    this->ui->lineObjectsName->setText(this->currentObject.name);
    this->ui->lineObjectsName->setProperty("backup", this->currentObject.name);

    this->ui->comboObjectsTopic->setModelColumn(0);
    this->ui->comboObjectsTopic->setCurrentText(QString::number(currentObject.topicId));
    this->ui->comboObjectsTopic->setProperty("backup", currentObject.topicId);
    this->ui->comboObjectsTopic->setModelColumn(1);

    this->ui->comboObjectsType->setCurrentText(currentObject.type);
    this->ui->comboObjectsType->setProperty("backup", currentObject.type);

    this->ui->lineObjectsPathToFile->setText(currentObject.pathToFile);
    this->ui->lineObjectsPathToFile->setProperty("backup", currentObject.pathToFile);

    this->ui->spinObjectsIndexNumber->setValue(currentObject.indexNumber);
    this->ui->spinObjectsIndexNumber->setProperty("backup", currentObject.indexNumber);

    ui->lineObjectsName->setEnabled(true);
    ui->comboObjectsTopic->setEnabled(true);
    ui->comboObjectsType->setEnabled(true);
    ui->spinObjectsIndexNumber->setEnabled(true);
    ui->buttonObjectsPathToFile->setEnabled(true);
    ui->buttonObjectsRemove->setEnabled(true);      
    this->ui->lineObjectsName->setFocus();

    this->hasChange();

    this->setProperty("isProgramInput", false);
}

// Удаление объекта
void MainPackage::on_buttonObjectsRemove_clicked()
{
    this->setProperty("isProgramInput", true);

    QSqlQuery query(this->handlerToPackage->databaseTemporary);

    query.prepare("DELETE FROM objects WHERE objectID=:id");
    query.bindValue(":id", this->currentObject.id);
    query.exec();

    query.prepare("UPDATE objects SET objectIndexNumber=objectIndexNumber-1 WHERE objectIndexNumber>:indexNumber");
    query.bindValue(":indexNumber", this->currentObject.indexNumber);
    query.exec();

    this->currentObject.id = -1;
    this->updateObjects();
    this->hasChange();

    this->ui->tableObjects->setFocus();

    this->setProperty("isProgramInput", false);
}

// Сохранение изменений
void MainPackage::on_buttonObjectsSave_clicked()
{
    QSqlQuery queryCore(this->handlerToPackage->databaseCore);
    QSqlQuery queryTemporary(this->handlerToPackage->databaseTemporary);

    queryCore.prepare("DELETE FROM objects");
    queryCore.exec();
    queryTemporary.prepare("SELECT * FROM objects");
    queryTemporary.exec();

    while(queryTemporary.next())
    {
        Object tmpObject;
        tmpObject.id = queryTemporary.value(0).toInt();
        tmpObject.name = queryTemporary.value(1).toString();
        tmpObject.type = queryTemporary.value(2).toString();
        tmpObject.pathToFile = queryTemporary.value(3).toString();
        tmpObject.topicId = queryTemporary.value(4).toInt();
        tmpObject.indexNumber = queryTemporary.value(5).toInt();
        queryCore.prepare("INSERT INTO objects(objectID, objectName, objectType, "
                          "objectPathToFile, objectTopicID, objectIndexNumber) "
                          "VALUES (:id, :name, :type, :pathToFile, "
                          ":topic, :index)");
        queryCore.bindValue(":id", tmpObject.id);
        queryCore.bindValue(":name", tmpObject.name);
        queryCore.bindValue(":type", tmpObject.type);
        queryCore.bindValue(":pathToFile", tmpObject.pathToFile);
        queryCore.bindValue(":topic", tmpObject.topicId);
        queryCore.bindValue(":index", tmpObject.indexNumber);
        queryCore.exec();
    }

    queryTemporary.prepare("DELETE FROM backupObjects");
    queryTemporary.exec();
    queryTemporary.prepare("INSERT INTO backupObjects SELECT * FROM objects");
    queryTemporary.exec();

    this->hasChange();

    this->ui->tableObjects->setFocus();

    this->ui->statusBar->setStyleSheet("color: green");
    this->ui->statusBar->showMessage("Изменения объектов сохранено", 5000);
}

// Отмена изменений
void MainPackage::on_buttonObjectsUndo_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Оболочка");
    msgBox.setText("<b>Вы уверены, что хотите отменить изменения объектов?</b>");
    msgBox.setInformativeText("<p style=\"color: grey;\">Будут отменены все "
                              "изменения с последнего сохранения</p>");

    msgBox.addButton("Отменить", QMessageBox::YesRole);
    msgBox.addButton(" Не отменять ", QMessageBox::NoRole);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QDialogButtonBox::NoRole) return;

    QSqlQuery queryTemporary(this->handlerToPackage->databaseTemporary);
    queryTemporary.exec("DELETE FROM objects");
    queryTemporary.exec("INSERT INTO objects SELECT * FROM backupObjects;");


    this->updateObjects();
    this->hasChange();
    this->ui->tableObjects->setFocus();
}

void MainPackage::on_lineObjectsName_textEdited(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveObject();
    bool isFullUpdate = false;
    this->updateObjects(isFullUpdate);
    this->ui->tableObjects->selectRow(this->objectsCurrentRow);
}

void MainPackage::on_spinObjectsIndexNumber_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveObject();
    bool isFullUpdate = false;
    this->updateObjects(isFullUpdate);
    this->ui->tableObjects->selectRow(this->objectsCurrentRow);
}

void MainPackage::on_comboObjectsTopic_currentIndexChanged(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveObject();
    bool isFullUpdate = false;
    this->updateObjects(isFullUpdate);
    this->ui->tableObjects->selectRow(this->objectsCurrentRow);
}

void MainPackage::on_comboObjectsType_currentTextChanged(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;

    QString typeNew = arg1;
    QString typeOld = this->ui->comboObjectsType->property("backup").toString();
    if ((typeNew == "Видео" && typeOld != "Видео") ||
        (typeNew == "Тест" && typeOld != "Тест") ||
        (typeNew != "Видео" && typeOld == "Видео") ||
        (typeNew != "Тест" && typeOld == "Тест"))
        this->ui->lineObjectsPathToFile->setText("");

    if (typeNew == "Видео")
        this->ui->labelObjectsFormat->setText("Файл должен быть в формате *.avi или *.wmv");
    else if (typeNew == "Тест")
        this->ui->labelObjectsFormat->setText("Файл должен быть в формате *.test");
    else
        this->ui->labelObjectsFormat->setText("Файл должен быть в формате *.pdf");

    this->temporarySaveObject();
    bool isFullUpdate = false;
    this->updateObjects(isFullUpdate);
    this->ui->tableObjects->selectRow(this->objectsCurrentRow);
}

void MainPackage::on_buttonObjectsPathToFile_clicked()
{
    QMap<QString, QVariant> argument;
    argument["titleDialog"] = "Выберите файл объекта";

    if (this->ui->comboObjectsType->currentText() == "Тест")
        argument["fileFormat"] = "*.test";
    else if(this->ui->comboObjectsType->currentText() == "Видео")
        argument["fileFormat"] = tr("Video (*.avi *.wmv)");
    else
        argument["fileFormat"] = "*.pdf";

    argument["pathToPackage"] = this->handlerToPackage->pathToFile;

    this->getFilename(argument, this->ui->linePathToAnnotation);

    this->temporarySaveObject();
    this->ui->tableObjects->selectRow(this->objectsCurrentRow);
}
