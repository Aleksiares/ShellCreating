#ifndef MAINTESTS_H
#define MAINTESTS_H

#include <QMainWindow>
#include <common.h>
#include <handlertotests.h>
#include <mainmenu.h>

namespace Ui {
class MainTests;
}

class MainTests : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainTests(HandlerToTests *inpHandlerToTests, QWidget *parent = 0);
    ~MainTests();

private:
    Ui::MainTests *ui;
    HandlerToTests* handlerToTest;
    Question currentQuestion;
    int questionCurrentRow;

private slots:

    void updateProperties();
    void updateQuestions(bool isFullUpdate = true, int indexRow = -1);

    void temporarySaveProperties();
    void temporarySaveQuestions();

    bool hasChange();

    void on_buttonSave_clicked();
    void on_buttonUndo_clicked();
    void on_buttonExit_clicked();
    void on_buttonExitToMenu_clicked();
    void on_buttonAbout_clicked();

    void on_buttonAddQuestion_clicked();
    void on_buttonRemoveQuestion_clicked();

    void on_lineTestName_textEdited(const QString &arg1);
    void on_spinFive_valueChanged(int arg1);
    void on_spinFour_valueChanged(int arg1);
    void on_spinThree_valueChanged(int arg1);
    void on_spinMinutes_valueChanged(int arg1);
    void on_spinSeconds_valueChanged(int arg1);
    void on_checkMix_stateChanged(int arg1);
    void on_checkUnlimitedTime_stateChanged(int arg1);
    void on_checkShowResult_stateChanged(int arg1);

    void on_tableQuestions_clicked(const QModelIndex &index);
    void on_textQuestion_textChanged();
    void on_spinIndexNumber_valueChanged(int arg1);
    void on_lineCorrectAnswer_textEdited(const QString &arg1);
    void on_lineIncorrectAnswer1_textEdited(const QString &arg1);
    void on_lineIncorrectAnswer2_textEdited(const QString &arg1);
    void on_lineIncorrectAnswer3_textEdited(const QString &arg1);
};

#endif // MAINTESTS_H
