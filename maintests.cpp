#include "maintests.h"
#include "ui_maintests.h"

#include <QStyleOptionViewItem>

MainTests::MainTests(HandlerToTests *inpHandlerToTests, QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainTests)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/icon"));
    this->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    this->handlerToTest = inpHandlerToTests;
    this->handlerToTest->connectToDatabase();

    this->handlerToTest->tableModels["questions"] = new QSqlTableModel(0, this->handlerToTest->databaseCore);
    this->handlerToTest->tableModels["questions"]->setTable("backupQuestions");
    this->handlerToTest->tableModels["questions"]->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->handlerToTest->tableModels["questions"]->setSort(6, Qt::AscendingOrder);
    this->ui->tableQuestions->setModel(this->handlerToTest->tableModels["questions"]);
    this->ui->tableQuestions->horizontalHeader()->setStretchLastSection(true);
    this->ui->tableQuestions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tableQuestions->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableQuestions->verticalHeader()->hide();
    this->ui->tableQuestions->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->updateProperties();
    this->updateQuestions();   
}

MainTests::~MainTests()
{
    delete ui;
}

void MainTests::updateProperties()
{
    this->handlerToTest->uploadProperties();

    ui->lineTestName->setText(this->handlerToTest->properties["name"]);
    ui->lineTestName->setProperty("backup", this->handlerToTest->properties["name"]);

    ui->spinFive->setValue(this->handlerToTest->properties["five"].toInt());
    ui->spinFive->setProperty("backup", this->handlerToTest->properties["five"].toInt());

    ui->spinFour->setValue(this->handlerToTest->properties["four"].toInt());
    ui->spinFour->setProperty("backup", this->handlerToTest->properties["four"].toInt());

    ui->spinThree->setValue(this->handlerToTest->properties["three"].toInt());
    ui->spinThree->setProperty("backup", this->handlerToTest->properties["three"].toInt());

    ui->spinMinutes->setValue(this->handlerToTest->properties["minutes"].toInt());
    ui->spinMinutes->setProperty("backup", this->handlerToTest->properties["minutes"].toInt());

    ui->spinSeconds->setValue(this->handlerToTest->properties["seconds"].toInt());
    ui->spinSeconds->setProperty("backup", this->handlerToTest->properties["seconds"].toInt());

    ui->checkMix->setChecked(this->handlerToTest->properties["mix"].toInt());
    ui->checkMix->setProperty("backup", this->handlerToTest->properties["mix"].toInt());

    ui->checkUnlimitedTime->setChecked(this->handlerToTest->properties["unlimited_time"].toInt());
    ui->checkUnlimitedTime->setProperty("backup", this->handlerToTest->properties["unlimited_time"].toInt());

    ui->checkShowResult->setChecked(this->handlerToTest->properties["show_result"].toInt());
    ui->checkShowResult->setProperty("backup", this->handlerToTest->properties["show_result"].toInt());
}

void MainTests::updateQuestions(bool isFullUpdate, int indexRow)
{
    this->setProperty("isProgramInput", true);

    this->handlerToTest->tableModels["questions"]->select();
    this->handlerToTest->tableModels["questions"]->setHeaderData(6, Qt::Horizontal, "Порядковый номер");

    this->ui->tableQuestions->setColumnHidden(0, true);
    this->ui->tableQuestions->setColumnHidden(1, true);
    this->ui->tableQuestions->setColumnHidden(2, true);
    this->ui->tableQuestions->setColumnHidden(3, true);
    this->ui->tableQuestions->setColumnHidden(4, true);
    this->ui->tableQuestions->setColumnHidden(5, true);
    this->ui->tableQuestions->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    if (isFullUpdate)
    {
        this->currentQuestion.id = -1;
        this->currentQuestion.text = "";
        this->currentQuestion.correctAnswer = "";
        this->currentQuestion.incorrectAnswer1 = "";
        this->currentQuestion.incorrectAnswer2 = "";
        this->currentQuestion.incorrectAnswer3 = "";
        this->currentQuestion.indexNumber = -1;

        this->ui->textQuestion->setText("");
        this->ui->textQuestion->setProperty("backup", "");
        this->ui->lineIncorrectAnswer1->setText("");
        this->ui->lineIncorrectAnswer1->setProperty("backup", "");
        this->ui->lineIncorrectAnswer1->setText("");
        this->ui->lineIncorrectAnswer1->setProperty("backup", "");
        this->ui->lineIncorrectAnswer2->setText("");
        this->ui->lineIncorrectAnswer2->setProperty("backup", "");
        this->ui->lineIncorrectAnswer3->setText("");
        this->ui->lineIncorrectAnswer3->setProperty("backup", "");

        this->ui->spinIndexNumber->setValue(1);
        this->ui->spinIndexNumber->setProperty("backup", 1);
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("SELECT COUNT(*) FROM backupQuestions");
        query.exec();
        query.next();
        this->setProperty("isProgramInput", false);
        int countQuestions = query.value(0).toInt();
        this->ui->spinIndexNumber->setMaximum(countQuestions);
        this->ui->spinFive->setMaximum(countQuestions);
        this->setProperty("isProgramInput", true);
        ui->labelCountQuestions->setText("Количество вопросов: " + QString::number(countQuestions));

        this->ui->textQuestion->setEnabled(false);
        this->ui->spinIndexNumber->setEnabled(false);
        this->ui->buttonAddQuestion->setEnabled(true);
        this->ui->buttonRemoveQuestion->setEnabled(false);
        this->ui->lineCorrectAnswer->setEnabled(false);
        this->ui->lineIncorrectAnswer1->setEnabled(false);
        this->ui->lineIncorrectAnswer2->setEnabled(false);
        this->ui->lineIncorrectAnswer3->setEnabled(false);
        this->ui->tableQuestions->setFocus();
    }

    this->setProperty("isProgramInput", false);
}

void MainTests::temporarySaveProperties()
{
    this->setProperty("isProgramInput", true);

    if (ui->lineTestName->text() != ui->lineTestName->property("backup").toString())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->lineTestName->text());
        query.bindValue(":name", "name");
        query.exec();
        ui->lineTestName->setProperty("backup", ui->lineTestName->text());
    }

    if (ui->spinFive->value() != ui->spinFive->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->spinFive->value());
        query.bindValue(":name", "five");
        query.exec();
        ui->spinFive->setProperty("backup", ui->spinFive->value());
    }

    if (ui->spinFour->value() != ui->spinFour->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->spinFour->value());
        query.bindValue(":name", "four");
        query.exec();
        ui->spinFour->setProperty("backup", ui->spinFour->value());
    }

    if (ui->spinThree->value() != ui->spinThree->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->spinThree->value());
        query.bindValue(":name", "three");
        query.exec();
        ui->spinThree->setProperty("backup", ui->spinThree->value());
    }

    if (ui->spinMinutes->value() != ui->spinMinutes->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->spinMinutes->value());
        query.bindValue(":name", "minutes");
        query.exec();
        ui->spinMinutes->setProperty("backup", ui->spinMinutes->value());
    }

    if (ui->spinSeconds->value() != ui->spinSeconds->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->spinSeconds->value());
        query.bindValue(":name", "seconds");
        query.exec();
        ui->spinSeconds->setProperty("backup", ui->spinSeconds->value());
    }

    if (ui->checkMix->isChecked() != ui->checkMix->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->checkMix->isChecked());
        query.bindValue(":name", "mix");
        query.exec();
        ui->checkMix->setProperty("backup", ui->checkMix->isChecked());
    }

    if (ui->checkUnlimitedTime->isChecked() != ui->checkUnlimitedTime->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->checkUnlimitedTime->isChecked());
        query.bindValue(":name", "unlimited_time");
        query.exec();
        ui->checkUnlimitedTime->setProperty("backup", ui->checkUnlimitedTime->isChecked());
    }

    if (ui->checkShowResult->isChecked() != ui->checkShowResult->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupProperties SET value=:value WHERE name=:name");
        query.bindValue(":value", ui->checkShowResult->isChecked());
        query.bindValue(":name", "show_result");
        query.exec();
        ui->checkShowResult->setProperty("backup", ui->checkShowResult->isChecked());
    }

    this->hasChange();
    this->setProperty("isProgramInput", false);
}

void MainTests::temporarySaveQuestions()
{
    this->setProperty("isProgramInput", true);

    if (ui->textQuestion->toPlainText() != ui->textQuestion->property("backup").toString())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupQuestions SET text=:value WHERE id=:id");
        query.bindValue(":value", ui->textQuestion->toPlainText());
        query.bindValue(":id", this->currentQuestion.id);
        query.exec();
        ui->textQuestion->setProperty("backup", ui->textQuestion->toPlainText());
    }

    if (ui->lineCorrectAnswer->text() != ui->lineCorrectAnswer->property("backup").toString())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupQuestions SET correctAnswer=:value WHERE id=:id");
        query.bindValue(":value", ui->lineCorrectAnswer->text());
        query.bindValue(":id", this->currentQuestion.id);
        query.exec();
        ui->lineCorrectAnswer->setProperty("backup", ui->lineCorrectAnswer->text());
    }

    if (ui->lineIncorrectAnswer1->text() != ui->lineIncorrectAnswer1->property("backup").toString())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupQuestions SET incorrectAnswer1=:value WHERE id=:id");
        query.bindValue(":value", ui->lineIncorrectAnswer1->text());
        query.bindValue(":id", this->currentQuestion.id);
        query.exec();
        ui->lineIncorrectAnswer1->setProperty("backup", ui->lineIncorrectAnswer1->text());
    }

    if (ui->lineIncorrectAnswer2->text() != ui->lineIncorrectAnswer2->property("backup").toString())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupQuestions SET incorrectAnswer2=:value WHERE id=:id");
        query.bindValue(":value", ui->lineIncorrectAnswer2->text());
        query.bindValue(":id", this->currentQuestion.id);
        query.exec();
        ui->lineIncorrectAnswer2->setProperty("backup", ui->lineIncorrectAnswer2->text());
    }

    if (ui->lineIncorrectAnswer3->text() != ui->lineIncorrectAnswer3->property("backup").toString())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupQuestions SET incorrectAnswer3=:value WHERE id=:id");
        query.bindValue(":value", ui->lineIncorrectAnswer3->text());
        query.bindValue(":id", this->currentQuestion.id);
        query.exec();
        ui->lineIncorrectAnswer3->setProperty("backup", ui->lineIncorrectAnswer3->text());
    }

    if (ui->spinIndexNumber->value() != ui->spinIndexNumber->property("backup").toInt())
    {
        QSqlQuery query(this->handlerToTest->databaseCore);
        query.prepare("UPDATE backupQuestions SET indexNumber=:index WHERE id=:id");
        query.bindValue(":index", ui->spinIndexNumber->value());
        query.bindValue(":id", this->currentQuestion.id);
        query.exec();

        query.prepare("UPDATE backupQuestions SET indexNumber=:index1 "
                      "WHERE indexNumber=:index2 AND id!=:id");
        query.bindValue(":index1", ui->spinIndexNumber->property("backup").toInt());
        query.bindValue(":index2", ui->spinIndexNumber->value());
        query.bindValue(":id", this->currentQuestion.id);
        query.exec();

        ui->spinIndexNumber->setProperty("backup", ui->spinIndexNumber->value());
        this->questionCurrentRow = ui->spinIndexNumber->value() - 1;
    }

    this->hasChange();
    this->setProperty("isProgramInput", false);
}

bool MainTests::hasChange()
{
    QSqlQuery query(this->handlerToTest->databaseCore);
    query.exec("SELECT * FROM backupProperties EXCEPT SELECT * FROM properties");
    int count = 0;
    while(query.next())
        ++count;

    query.exec("SELECT * FROM backupQuestions EXCEPT SELECT * FROM questions");
    while(query.next())
        ++count;

    if (count == 0)
    {
        this->ui->buttonSave->setEnabled(false);
        this->ui->buttonUndo->setEnabled(false);
        return false;
    }

    this->ui->buttonSave->setEnabled(true);
    this->ui->buttonUndo->setEnabled(true);
    return true;
}

void MainTests::on_buttonSave_clicked()
{
    QSqlQuery query(this->handlerToTest->databaseCore);
    query.exec("DELETE FROM properties");
    query.exec("INSERT INTO properties SELECT * FROM backupProperties");
    query.exec("DELETE FROM questions");
    query.exec("INSERT INTO questionsSELECT * FROM backupQuestions");
    this->hasChange();
}

void MainTests::on_buttonUndo_clicked()
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

    QSqlQuery query(this->handlerToTest->databaseCore);
    query.exec("DELETE FROM backupProperties");
    query.exec("INSERT INTO backupProperties SELECT * FROM properties");
    query.exec("DELETE FROM backupQuestions");
    query.exec("INSERT INTO backupQuestions SELECT * FROM questions");
    this->hasChange();
    this->updateProperties();
    this->updateQuestions();
}

void MainTests::on_buttonExit_clicked()
{
    bool isCheck = false;
    if (hasChange())
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
            this->on_buttonSave_clicked();
    }

    this->handlerToTest->databaseCore.close();
    this->close();
}

void MainTests::on_buttonExitToMenu_clicked()
{
    bool isCheck = false;
    if (hasChange())
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
        if (resultMsgBox == 2) return;
        if (resultMsgBox == 0)
            this->on_buttonSave_clicked();
    }

    MainMenu *mainMenu = new MainMenu();
    mainMenu->show();
    this->close();
}

void MainTests::on_buttonAbout_clicked()
{
    QMessageBox::information(0, "О программе", "    Учебная оболочка<br>"
                                               "    Версия: 1.0<br>");
}

// Вкладка свойств

void MainTests::on_lineTestName_textEdited(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveProperties();
}

void MainTests::on_spinFive_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->property("isProgramInput").toBool() == true;
    ui->spinFour->setMaximum(ui->spinFive->value() - 1);
    ui->spinThree->setMaximum(ui->spinFour->value() - 1);
    this->property("isProgramInput").toBool() == false;
    this->temporarySaveProperties();

}

void MainTests::on_spinFour_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->property("isProgramInput").toBool() == true;
    ui->spinFive->setMinimum(ui->spinFour->value() + 1);
    ui->spinThree->setMaximum(ui->spinFour->value() - 1);
    this->property("isProgramInput").toBool() == false;
    this->temporarySaveProperties();
}

void MainTests::on_spinThree_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->property("isProgramInput").toBool() == true;
    ui->spinFour->setMinimum(ui->spinThree->value() + 1);
    ui->spinFive->setMinimum(ui->spinFour->value() + 1);
    this->property("isProgramInput").toBool() == false;
    this->temporarySaveProperties();
}

void MainTests::on_spinMinutes_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveProperties();
}

void MainTests::on_spinSeconds_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveProperties();
}

void MainTests::on_checkMix_stateChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveProperties();
}

void MainTests::on_checkUnlimitedTime_stateChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveProperties();
}

void MainTests::on_checkShowResult_stateChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveProperties();
}

// Вкладка вопросов

void MainTests::on_tableQuestions_clicked(const QModelIndex &index)
{
    this->setProperty("isProgramInput", true);

    this->questionCurrentRow = index.row();

    this->currentQuestion.id = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 0)).toInt();

    this->currentQuestion.text = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 1)).toString();

    this->currentQuestion.correctAnswer = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 2)).toString();

    this->currentQuestion.incorrectAnswer1 = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 3)).toString();

    this->currentQuestion.incorrectAnswer2 = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 4)).toString();

    this->currentQuestion.incorrectAnswer3 = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 5)).toString();

    this->currentQuestion.indexNumber = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 6)).toInt();

    this->ui->textQuestion->setText(this->currentQuestion.text);
    this->ui->textQuestion->setProperty("backup", this->currentQuestion.text);

    this->ui->spinIndexNumber->setValue(this->currentQuestion.indexNumber);
    this->ui->spinIndexNumber->setProperty("backup", this->currentQuestion.indexNumber);

    this->ui->lineCorrectAnswer->setText(this->currentQuestion.correctAnswer);
    this->ui->lineCorrectAnswer->setProperty("backup", this->currentQuestion.correctAnswer);

    this->ui->lineIncorrectAnswer1->setText(this->currentQuestion.incorrectAnswer1);
    this->ui->lineIncorrectAnswer1->setProperty("backup", this->currentQuestion.incorrectAnswer1);

    this->ui->lineIncorrectAnswer2->setText(this->currentQuestion.incorrectAnswer2);
    this->ui->lineIncorrectAnswer2->setProperty("backup", this->currentQuestion.incorrectAnswer2);

    this->ui->lineIncorrectAnswer3->setText(this->currentQuestion.incorrectAnswer3);
    this->ui->lineIncorrectAnswer3->setProperty("backup", this->currentQuestion.incorrectAnswer3);

    ui->textQuestion->setEnabled(true);
    ui->spinIndexNumber->setEnabled(true);
    ui->lineIncorrectAnswer1->setEnabled(true);
    ui->lineIncorrectAnswer1->setEnabled(true);
    ui->lineIncorrectAnswer2->setEnabled(true);
    ui->lineIncorrectAnswer3->setEnabled(true);

    this->ui->buttonAddQuestion->setEnabled(true);
    this->ui->buttonRemoveQuestion->setEnabled(false);
    if(ui->spinIndexNumber->maximum() > 5)
        this->ui->buttonRemoveQuestion->setEnabled(true);
    this->setProperty("isProgramInput", false);
}

void MainTests::on_textQuestion_textChanged()
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveQuestions();
}

void MainTests::on_spinIndexNumber_valueChanged(int arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveQuestions();
    bool isFullUpdate = false;
    this->updateQuestions(isFullUpdate);
    this->ui->tableQuestions->selectRow(this->questionCurrentRow);
}

void MainTests::on_lineCorrectAnswer_textEdited(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveQuestions();
}

void MainTests::on_lineIncorrectAnswer1_textEdited(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveQuestions();
}

void MainTests::on_lineIncorrectAnswer2_textEdited(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveQuestions();
}

void MainTests::on_lineIncorrectAnswer3_textEdited(const QString &arg1)
{
    if(this->property("isProgramInput").toBool() == true) return;
    this->temporarySaveQuestions();
}

void MainTests::on_buttonAddQuestion_clicked()
{
    this->setProperty("isProgramInput", true);

    QSqlQuery query(this->handlerToTest->databaseCore);
    query.exec("INSERT INTO backupQuestions(indexNumber) VALUES ((SELECT COUNT(*) FROM backupQuestions) + 1)");

    this->updateQuestions();
    this->setProperty("isProgramInput", true);

    qDebug() << "dsadsa";

    this->questionCurrentRow = this->ui->tableQuestions->model()->rowCount();
    --this->questionCurrentRow;
    this->ui->tableQuestions->selectRow(this->questionCurrentRow);

    this->currentQuestion.id = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 0)).toInt();

    this->currentQuestion.text = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 1)).toString();

    this->currentQuestion.correctAnswer = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 2)).toString();

    this->currentQuestion.incorrectAnswer1 = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 3)).toString();

    this->currentQuestion.incorrectAnswer2 = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 4)).toString();

    this->currentQuestion.incorrectAnswer3 = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 5)).toString();

    this->currentQuestion.indexNumber = this->ui->tableQuestions->model()->
            data(this->ui->tableQuestions->model()->index(this->questionCurrentRow, 6)).toInt();

    this->ui->textQuestion->setText(this->currentQuestion.text);
    this->ui->textQuestion->setProperty("backup", this->currentQuestion.text);

    this->ui->spinIndexNumber->setValue(this->currentQuestion.indexNumber);
    this->ui->spinIndexNumber->setProperty("backup", this->currentQuestion.indexNumber);

    this->ui->lineCorrectAnswer->setText(this->currentQuestion.correctAnswer);
    this->ui->lineCorrectAnswer->setProperty("backup", this->currentQuestion.correctAnswer);

    this->ui->lineIncorrectAnswer1->setText(this->currentQuestion.incorrectAnswer1);
    this->ui->lineIncorrectAnswer1->setProperty("backup", this->currentQuestion.incorrectAnswer1);

    this->ui->lineIncorrectAnswer2->setText(this->currentQuestion.incorrectAnswer2);
    this->ui->lineIncorrectAnswer2->setProperty("backup", this->currentQuestion.incorrectAnswer2);

    this->ui->lineIncorrectAnswer3->setText(this->currentQuestion.incorrectAnswer3);
    this->ui->lineIncorrectAnswer3->setProperty("backup", this->currentQuestion.incorrectAnswer3);

    ui->textQuestion->setEnabled(true);
    ui->spinIndexNumber->setEnabled(true);
    ui->lineIncorrectAnswer1->setEnabled(true);
    ui->lineIncorrectAnswer1->setEnabled(true);
    ui->lineIncorrectAnswer2->setEnabled(true);
    ui->lineIncorrectAnswer3->setEnabled(true);

    this->ui->buttonAddQuestion->setEnabled(true);
    this->ui->buttonRemoveQuestion->setEnabled(false);
    if(ui->spinIndexNumber->maximum() > 5)
        this->ui->buttonRemoveQuestion->setEnabled(true);

    this->hasChange();

    this->setProperty("isProgramInput", false);
}

void MainTests::on_buttonRemoveQuestion_clicked()
{
    this->setProperty("isProgramInput", true);

    QSqlQuery query(this->handlerToTest->databaseCore);

    query.prepare("DELETE FROM backupQuestions WHERE id=:id");
    query.bindValue(":id", this->currentQuestion.id);
    query.exec();

    query.prepare("UPDATE backupQuestions SET indexNumber=indexNumber-1 WHERE indexNumber>:indexNumber");
    query.bindValue(":indexNumber", this->ui->spinIndexNumber->value());
    query.exec();

    this->updateQuestions();
    this->hasChange();

    this->ui->tableQuestions->setFocus();

    this->setProperty("isProgramInput", false);
}
