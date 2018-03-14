#include "mainpackage.h"
#include "ui_mainpackage.h"

// Обновление таблицы тем
void MainPackage::updateTopics(bool isFullUpdate, int indexRow)
{
    this->setProperty("isProgramInput", true);

    this->handlerToPackage->tableModels["topics"]->select();
    this->handlerToPackage->tableModels["topics"]->setHeaderData(0, Qt::Horizontal, "ID");
    this->handlerToPackage->tableModels["topics"]->setHeaderData(1, Qt::Horizontal, "Название темы");
    this->handlerToPackage->tableModels["topics"]->setHeaderData(2, Qt::Horizontal, "Порядковый номер");

    this->ui->tableTopics->setColumnHidden(0, true);
    this->ui->tableTopics->setColumnHidden(2, true);    
    this->ui->tableTopics->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    if (isFullUpdate)
    {
        this->currentTopic.id = -1;
        this->currentTopic.name = "";
        this->currentTopic.indexNumber = -1;

        this->ui->lineTopicsName->setText("");
        this->ui->lineTopicsName->setProperty("backup", "");

        this->ui->spinTopicsIndexNumber->setValue(1);
        this->ui->spinTopicsIndexNumber->setProperty("backup", 1);
        int countTopics = this->getCountItems("topics");
        this->ui->spinTopicsIndexNumber->setMaximum(countTopics);

        this->ui->lineTopicsName->setEnabled(false);
        this->ui->spinTopicsIndexNumber->setEnabled(false);
        this->ui->buttonTopicsAdd->setEnabled(true);
        this->ui->buttonTopicsRemove->setEnabled(false);

        this->handlerToPackage->tableModels["filterTopics"] = new QSqlTableModel(0, this->handlerToPackage->databaseTemporary);
        this->handlerToPackage->tableModels["filterTopics"]->setTable("topics");
        this->handlerToPackage->tableModels["filterTopics"]->setSort(2, Qt::AscendingOrder);
        this->handlerToPackage->tableModels["filterTopics"]->select();

        this->ui->comboObjectsFiltersTopic->setModel(this->handlerToPackage->tableModels["filterTopics"]);
        this->ui->comboObjectsFiltersTopic->setModelColumn(1);

        this->ui->comboObjectsTopic->setModel(this->handlerToPackage->tableModels["filterTopics"]);
        this->ui->comboObjectsTopic->setModelColumn(1);
    }
    this->handlerToPackage->tableModels["filterTopics"] = new QSqlTableModel(0, this->handlerToPackage->databaseTemporary);
    this->handlerToPackage->tableModels["filterTopics"]->setTable("topics");
    this->handlerToPackage->tableModels["filterTopics"]->setSort(2, Qt::AscendingOrder);
    this->handlerToPackage->tableModels["filterTopics"]->select();

    this->ui->comboObjectsFiltersTopic->setModel(this->handlerToPackage->tableModels["filterTopics"]);
    this->ui->comboObjectsFiltersTopic->setModelColumn(1);

    this->ui->comboObjectsTopic->setModel(this->handlerToPackage->tableModels["filterTopics"]);
    this->ui->comboObjectsTopic->setModelColumn(1);

    this->setProperty("isProgramInput", false);
}

// Выбор темы
void MainPackage::on_tableTopics_clicked(const QModelIndex &index)
{    
    this->setProperty("isProgramInput", true);

    this->topicsCurrentRow = index.row();

    this->currentTopic.id = this->ui->tableTopics->model()->
            data(this->ui->tableTopics->model()->index(this->topicsCurrentRow, 0)).toInt();

    this->currentTopic.name = this->ui->tableTopics->model()->
            data(this->ui->tableTopics->model()->index(this->topicsCurrentRow, 1)).toString();

    this->currentTopic.indexNumber = this->ui->tableTopics->model()->
            data(this->ui->tableTopics->model()->index(this->topicsCurrentRow, 2)).toInt();

    this->ui->lineTopicsName->setText(this->currentTopic.name);
    this->ui->lineTopicsName->setProperty("backup", this->currentTopic.name);
    this->ui->spinTopicsIndexNumber->setValue(this->currentTopic.indexNumber);
    this->ui->spinTopicsIndexNumber->setProperty("backup", this->currentTopic.indexNumber);

    this->ui->lineTopicsName->setEnabled(true);
    this->ui->spinTopicsIndexNumber->setEnabled(true);
    this->ui->buttonTopicsAdd->setEnabled(true);
    this->ui->buttonTopicsRemove->setEnabled(true);

    this->setProperty("isProgramInput", false);
}

// Сохранение в временную базу
void MainPackage::temporarySaveTopic()
{
    this->setProperty("isProgramInput", true);

    this->currentTopic.name = ui->lineTopicsName->text();
    this->currentTopic.indexNumber = ui->spinTopicsIndexNumber->value();

    if (ui->lineTopicsName->text() != ui->lineTopicsName->property("backup").toString())
    {
        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("UPDATE topics SET topicName=:name WHERE topicID=:id");
        query.bindValue(":name", this->currentTopic.name);
        query.bindValue(":id", this->currentTopic.id);
        query.exec();
        ui->lineTopicsName->setProperty("backup", this->currentTopic.name);
    }

    if (ui->spinTopicsIndexNumber->value() != ui->spinTopicsIndexNumber->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToPackage->databaseTemporary);
        query.prepare("UPDATE topics SET topicIndexNumber=:index WHERE topicID=:id");
        query.bindValue(":index", this->currentTopic.indexNumber);
        query.bindValue(":id", this->currentTopic.id);
        query.exec();

        query.prepare("UPDATE topics SET topicIndexNumber=:index1 "
                      "WHERE topicIndexNumber=:index2 AND topicID!=:id");
        query.bindValue(":index1", ui->spinTopicsIndexNumber->property("backup").toInt());
        query.bindValue(":index2", this->currentTopic.indexNumber);
        query.bindValue(":id", this->currentTopic.id);
        query.exec();

        ui->spinTopicsIndexNumber->setProperty("backup", this->currentTopic.indexNumber);
        this->topicsCurrentRow = this->currentTopic.indexNumber - 1;
    }

    this->hasChange();
    this->setProperty("isProgramInput", false);
}

// Добавление новой темы
void MainPackage::on_buttonTopicsAdd_clicked()
{
    this->setProperty("isProgramInput", true);

//    Delete if all will work without this (#Delete)
//    if (this->currentTopic.id != -1)
//        this->temporarySaveTopic();

    QSqlQuery query(this->handlerToPackage->databaseTemporary);
    query.exec("INSERT INTO topics(topicID, topicName, topicIndexNumber) "
               "VALUES (NULL, '', (SELECT max(topicIndexNumber) FROM  topics)+1)");

    this->updateTopics();
    this->setProperty("isProgramInput", true);
    this->topicsCurrentRow = this->ui->tableTopics->model()->rowCount();
    --this->topicsCurrentRow;
    this->ui->tableTopics->selectRow(this->topicsCurrentRow);

    this->currentTopic.id = this->ui->tableTopics->model()->
            data(this->ui->tableTopics->model()->index(this->topicsCurrentRow, 0)).toInt();

    this->currentTopic.name = this->ui->tableTopics->model()->
            data(this->ui->tableTopics->model()->index(this->topicsCurrentRow, 1)).toString();

    this->currentTopic.indexNumber = this->ui->tableTopics->model()->
            data(this->ui->tableTopics->model()->index(this->topicsCurrentRow, 2)).toInt();

    this->ui->lineTopicsName->setText(this->currentTopic.name);
    this->ui->lineTopicsName->setProperty("backup", this->currentTopic.name);
    this->ui->spinTopicsIndexNumber->setValue(this->currentTopic.indexNumber);
    this->ui->spinTopicsIndexNumber->setProperty("backup", this->currentTopic.indexNumber);

    this->hasChange();

    this->ui->lineTopicsName->setEnabled(true);
    this->ui->spinTopicsIndexNumber->setEnabled(true);
    this->ui->buttonTopicsAdd->setEnabled(true);
    this->ui->buttonTopicsRemove->setEnabled(true);
    this->ui->lineTopicsName->setFocus();

    this->setProperty("isProgramInput", false);
}

// Удаление темы
void MainPackage::on_buttonTopicsRemove_clicked()
{
    this->setProperty("isProgramInput", true);

    QSqlQuery query(this->handlerToPackage->databaseTemporary);

    query.prepare("UPDATE objects SET objectTopicID=0 WHERE objectTopicID=:id");
    query.bindValue(":id", this->currentTopic.id);
    query.exec();

    query.prepare("DELETE FROM topics WHERE topicID=:id");
    query.bindValue(":id", this->currentTopic.id);
    query.exec();

    query.prepare("UPDATE topics SET topicIndexNumber=topicIndexNumber-1 WHERE topicIndexNumber>:indexNumber");
    query.bindValue(":indexNumber", this->currentTopic.indexNumber);
    query.exec();

    this->updateTopics();
    this->hasChange();

    this->ui->tableTopics->setFocus();

    this->setProperty("isProgramInput", false);
}

// Сохранение изменений в основную базу
void MainPackage::on_buttonTopicsSave_clicked()
{

    QSqlQuery queryCore(this->handlerToPackage->databaseCore);
    QSqlQuery queryTemporary(this->handlerToPackage->databaseTemporary);

    queryCore.prepare("DELETE FROM topics");
    queryCore.exec();
    queryTemporary.prepare("SELECT * FROM topics");
    queryTemporary.exec();

    while(queryTemporary.next())
    {
        Topic tmpTopic;
        tmpTopic.id = queryTemporary.value(0).toInt();
        tmpTopic.name = queryTemporary.value(1).toString();
        tmpTopic.indexNumber = queryTemporary.value(2).toInt();

        queryCore.prepare("INSERT INTO topics(topicID, topicName, topicIndexNumber) "
                          "VALUES (:id, :name, :index)");
        queryCore.bindValue(":id", tmpTopic.id);
        queryCore.bindValue(":name", tmpTopic.name);
        queryCore.bindValue(":index", tmpTopic.indexNumber);
        queryCore.exec();
    }

    queryTemporary.prepare("DELETE FROM backupTopics");
    queryTemporary.exec();
    queryTemporary.prepare("INSERT INTO backupTopics SELECT * FROM topics");
    queryTemporary.exec();

    this->hasChange();

    this->ui->tableTopics->setFocus();

    this->ui->statusBar->setStyleSheet("color: green");
    this->ui->statusBar->showMessage("Изменения тем сохранено", 5000);
}

// Удаление изменений из временной базы
void MainPackage::on_buttonTopicsUndo_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Оболочка");
    msgBox.setText("<b>Вы уверены, что хотите отменить изменения тем?</b>");
    msgBox.setInformativeText("<p style=\"color: grey;\">Будут отменены все "
                              "изменения с последнего сохранения</p>");

    msgBox.addButton("Отменить", QMessageBox::YesRole);
    msgBox.addButton(" Не отменять ", QMessageBox::NoRole);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QDialogButtonBox::NoRole) return;

    this->setProperty("isProgramInput", true);

    QSqlQuery queryTemporary(this->handlerToPackage->databaseTemporary);
    queryTemporary.exec("DELETE FROM topics");
    queryTemporary.exec("INSERT INTO topics SELECT * FROM backupTopics;");

    this->updateTopics();
    this->hasChange();

    this->ui->tableTopics->setFocus();

    this->setProperty("isProgramInput", false);
}

// Реагирование на ввод имени и порядкового номера темы

void MainPackage::on_spinTopicsIndexNumber_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveTopic();
    bool isFullUpdate = false;
    this->updateTopics(isFullUpdate);
    this->ui->tableTopics->selectRow(this->topicsCurrentRow);
}

void MainPackage::on_lineTopicsName_textChanged(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveTopic();
    bool isFullUpdate = false;
    this->updateTopics(isFullUpdate);
    this->ui->tableTopics->selectRow(this->topicsCurrentRow);
}
