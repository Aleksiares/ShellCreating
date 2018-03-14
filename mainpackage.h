#ifndef MAINPACKAGE_H
#define MAINPACKAGE_H

#include <QMainWindow>
#include <common.h>
#include <handlertopackages.h>
#include <mainmenu.h>

namespace Ui {
class MainPackage;
}

class MainPackage : public QMainWindow
{
    Q_OBJECT 
public:
    explicit MainPackage(HandlerToPackages *inpHandlerPackage, QWidget *parent = 0);
    ~MainPackage();

private:
    Ui::MainPackage *ui;

    HandlerToPackages *handlerToPackage;
    Topic currentTopic;
    Object currentObject;

    int topicsCurrentRow = -1;
    int objectsCurrentRow = -1;
    QString objectsFilter = "";

    void generatePackage();

private slots:
// Общее
    void on_buttonAllSave_clicked();
    void on_buttonAllUndo_clicked();
    void on_buttonExit_clicked();
    void on_buttonExitToMenu_clicked();
    void on_buttonAbout_clicked();

    void getFilename(QMap<QString, QVariant> arguments, QLineEdit* lineOutput);
    int getCountItems(QString tableName);
    void hasChange();

//  Свойства
    void on_linePackageName_textEdited(const QString &arg1);
    void on_buttonPathToAnnotation_clicked();
    void on_buttonPathToLiterature_clicked();
    void on_buttonPathToAuthor_clicked();
    void on_buttonPathToGlossary_clicked();

    void on_buttonPropertiesSave_clicked();
    void on_buttonPropertiesUndo_clicked();

    void updateToProperties();
    bool hasChangeToProperties();


//  Темы
    void on_tableTopics_clicked(const QModelIndex &index);

    void on_spinTopicsIndexNumber_valueChanged(int arg1);
    void on_lineTopicsName_textChanged(const QString &arg1);

    void on_buttonTopicsAdd_clicked();
    void on_buttonTopicsRemove_clicked();

    void on_buttonTopicsSave_clicked();
    void on_buttonTopicsUndo_clicked();

    void updateTopics(bool isFullUpdate = true, int indexRow = -1);
    void temporarySaveTopic();
    bool hasChangeToTopics();

//  Объекты
    void on_tableObjects_clicked(const QModelIndex &index);

    void on_buttonObjectsShowFilter_clicked(bool checked);
    void on_comboObjectsFiltersType_currentIndexChanged(int index);
    void on_comboObjectsFiltersTopic_currentIndexChanged(int index);
    void on_checkObjectsFilterTopics_clicked(bool checked);
    void on_checkObjectsFilterTypes_clicked(bool checked);

    void on_lineObjectsName_textEdited(const QString &arg1);
    void on_spinObjectsIndexNumber_valueChanged(int arg1);
    void on_comboObjectsTopic_currentIndexChanged(const QString &arg1);
    void on_comboObjectsType_currentTextChanged(const QString &arg1);
    void on_buttonObjectsPathToFile_clicked();

    void on_buttonObjectsAdd_clicked();
    void on_buttonObjectsRemove_clicked();

    void on_buttonObjectsSave_clicked();
    void on_buttonObjectsUndo_clicked();

    void updateObjects(bool isFullUpdate = true, int indexRow = -1);
    void generateFilterObjects();
    void temporarySaveObject();
    bool hasChangeToObjects();    
};

#endif // MAINPACKAGE_H
