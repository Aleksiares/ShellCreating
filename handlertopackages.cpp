#include "handlertopackages.h"

HandlerToPackages::HandlerToPackages(QObject *parent) : QObject(parent)
{
    this->databaseCore = QSqlDatabase::addDatabase("QSQLITE", "core");
    this->databaseTemporary = QSqlDatabase::addDatabase("QSQLITE", "tmp");
}

bool HandlerToPackages::createPackage()
{
    this->pathToFile = QFileDialog::getSaveFileName(0, "Создать пакет", "", "");

    if (this->pathToFile == "")
    {
        this->lastError = "Файл не выбран";
        return false;
    }

    if (!QDir().mkdir(this->pathToFile))
    {
        this->lastError = "Не удалось создать каталог пакета";
        return false;
    }

    QList<QString> objectKeys = OBJECT_TYPES_NAME.keys();
    for (int i = 0; i < objectKeys.size(); ++i)
        QDir().mkdir(this->pathToFile + "\\" + objectKeys[i]);

    bool isCopiedCore = QFile::copy(":/databases/sourcePackage",
                                    this->pathToFile + "\\core.pckg");

    if (!isCopiedCore)
    {
        this->lastError = "Не удалось создать ядро пакета";
        return false;
    }

    QFile core(this->pathToFile + "\\core.pckg");
    bool isModifiedCore = core.setPermissions(QFile::WriteUser);
    if(!isModifiedCore)
    {
        this->lastError = "Не удалось модифицировать ядро пакета";
        return false;
    }

    this->databaseCore.setDatabaseName(this->pathToFile + "\\core.pckg");

    return true;
}

bool HandlerToPackages::openPackage()
{
    this->pathToFile = QFileDialog::getExistingDirectory(0, "Открыть пакет", "");
    if (this->pathToFile == "") return false;

    if (!QFile::exists(this->pathToFile + "\\core.pckg"))
    {
        lastError = "Отсутствует ядро пакета";
        return false;
    }

    this->databaseCore.setDatabaseName(this->pathToFile + "\\core.pckg");
    this->databaseTemporary.setDatabaseName(this->pathToFile + "\\tmp.pckg");

    return true;
}

bool HandlerToPackages::createBuffer()
{
    if (QFile::exists(this->pathToFile + "\\tmp.pckg"))
        QFile::remove(this->pathToFile + "\\tmp.pckg");

    if (!QFile::exists(this->pathToFile + "\\core.pckg"))
    {
        this->lastError = "Отсутствует ядро пакета";
        return false;
    }

    QFile::copy(this->pathToFile + "\\core.pckg",
                this->pathToFile + "\\tmp.pckg");

    if (!QFile::exists(this->pathToFile + "\\tmp.pckg"))
    {
        this->lastError = "Не удалось создать буферное ядро";
        return false;
    }

    QFile temporary(this->pathToFile + "\\tmp.pckg");

    bool isModifiedCore = temporary.setPermissions(QFile::WriteUser);
    if (!isModifiedCore)
    {
        this->lastError = "Не удалось модифицировать буферное ядро";
        return false;
    }

    this->databaseTemporary.setDatabaseName(this->pathToFile + "\\tmp.pckg");       
}

bool HandlerToPackages::destroyBuffer()
{
    this->databaseTemporary.close();
    QFile::remove(pathToFile + "\\tmp.pckg");
    if (QFile::exists(pathToFile + "\\tmp.pckg"))
    {
        this->lastError = "Не удалось удалить буферное ядро";
        return false;
    }
    return true;
}

bool HandlerToPackages::connectToDatabase()
{
    if(!this->databaseCore.open())
    {
        this->lastError = "Не удалось открыть ядро";
        return false;
    }

    if(!this->databaseTemporary.open())
    {
        this->lastError = "Не удалось открыть временное ядро";
        return false;
    }

    QSqlQuery query(this->databaseTemporary);
    query.exec("CREATE TABLE backupTopics AS SELECT * FROM topics");
    query.exec("CREATE TABLE backupObjects AS SELECT * FROM objects");

    return true;
}

void HandlerToPackages::uploadProperties()
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

void HandlerToPackages::unloadProperties()
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
