#ifndef HANDLERTOTESTS_H
#define HANDLERTOTESTS_H

#include <QObject>
#include <common.h>

class HandlerToTests : public QObject
{
    Q_OBJECT
public:
    QString pathToFile = "";
    QString lastError = "";

    QSqlDatabase databaseCore;

    QMap<QString, QString> properties;
    QMap<QString, QSqlTableModel*> tableModels;

    explicit HandlerToTests(QObject *parent = nullptr);

    bool createTest();
    bool openTest();

    bool connectToDatabase();

    void uploadProperties();
    void unloadProperties();

public slots:
};

#endif // HANDLERTOTESTS_H
