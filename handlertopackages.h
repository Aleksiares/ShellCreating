#ifndef HANDLERTOPACKAGE_H
#define HANDLERTOPACKAGE_H

#include <QObject>
#include <common.h>

class HandlerToPackages : public QObject
{
    Q_OBJECT
public:
    QString pathToFile = "";
    QString lastError = "";

    QSqlDatabase databaseCore;
    QSqlDatabase databaseTemporary;

    QMap<QString, QString> properties;
    QMap<QString, QSqlTableModel*> tableModels;

    explicit HandlerToPackages(QObject *parent = nullptr);

    /*
     * createPackage - создание пакета
     * openPackage - открытие пакета
     * createBuffer - создание файла временной базы (буфера)
     * destroyBuffer - удаление файла временной базы (буфера)
     * connectToDatabase - открытие базы данных
     * uploadProperties - выгрузка свойств из базы данных
     * unloadProperties - сохранение свойств в базу данных
     */

    bool createPackage();
    bool openPackage();

    bool createBuffer();
    bool destroyBuffer();

    bool connectToDatabase();

    void uploadProperties();
    void unloadProperties();
};

#endif // HANDLERTOPACKAGE_H
