#ifndef COMMON_H
#define COMMON_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QLineEdit>

#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QQueue>

#include <QDebug>

const QMap<QString, QString> OBJECT_TYPES_NAME = {
    {"lecture", "Лекция"},
    {"exercise", "Задание"},
    {"additional_material", "Дополнительные материалы"},
    {"presentation", "Презентация"},
    {"test", "Тест"},
    {"video", "Видео"},};

struct Topic {
    int id;
    QString name;
    int indexNumber;
};

struct Object {
    int id;
    QString type;
    QString name;
    QString pathToFile;
    int topicId;
    int indexNumber;
};

struct Question {
    int id;
    QString text;
    QString correctAnswer;
    QString incorrectAnswer1;
    QString incorrectAnswer2;
    QString incorrectAnswer3;
    int indexNumber;
};

#endif // СOMMON_H
