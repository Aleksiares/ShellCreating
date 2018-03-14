#include "handlertotests.h"

HandlerToTests::HandlerToTests(QObject *parent) : QObject(parent)
{
    this->databaseCore = QSqlDatabase::addDatabase("QSQLITE", "test");
}

bool HandlerToTests::createTest()
{
    this->pathToFile = QFileDialog::getSaveFileName(0, "Создать пакет", "", "*.test");

    if (this->pathToFile == "")
    {
        this->lastError = "Файл не выбран";
        return false;
    }

    bool isCopiedCore = QFile::copy(":/databases/sourceTest", this->pathToFile);

    if (!isCopiedCore)
    {
        this->lastError = "Не удалось создать тест";
        return false;
    }

    QFile test(this->pathToFile);

    bool isModifiedCore = test.setPermissions(QFile::WriteUser);
    if(!isModifiedCore)
    {
        this->lastError = "Не удалось модифицировать тест для открытия";
        return false;
    }

    this->databaseCore.setDatabaseName(this->pathToFile);

    return true;
}

bool HandlerToTests::openTest()
{
    this->pathToFile = QFileDialog::getOpenFileName(0, "Открыть пакет", "", "*.test");

    if (this->pathToFile == "")
    {
        this->lastError = "Файл не выбран";
        return false;
    }

    this->databaseCore.setDatabaseName(this->pathToFile);

    return true;
}

bool HandlerToTests::connectToDatabase()
{
    if(!this->databaseCore.open())
    {
        this->lastError = "Не удалось открыть тест";
        return false;
    }

    QSqlQuery query(this->databaseCore);
    query.exec("DELETE FROM backupProperties");
    query.exec("INSERT INTO backupProperties SELECT * FROM properties");
    query.exec("DELETE FROM backupQuestions");
    query.exec("INSERT INTO backupQuestions SELECT * FROM questions");
    return true;
}

void HandlerToTests::uploadProperties()
{
    this->properties.clear();
    QSqlQuery query = QSqlQuery(this->databaseCore);
    query.exec("SELECT * FROM properties");
    while (query.next())
    {
        QString name = query.value(0).toString();
        QString value = "";
        value = query.value(1).toString();
        this->properties[name] = value;
    }
}

void HandlerToTests::unloadProperties()
{
    QList<QString> keysOfProperties = this->properties.keys();
    for(int i = 0; i < keysOfProperties.size(); ++i)
    {
        QSqlQuery query = QSqlQuery(this->databaseCore);
        QString name = keysOfProperties[i];
        query.prepare("UPDATE properties SET propertyValue=:value WHERE propertyName=:name");
        query.bindValue(":value", this->properties[name]);
        query.bindValue(":name", name);
        query.exec();
    }
}
