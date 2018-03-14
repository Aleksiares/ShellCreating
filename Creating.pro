#-------------------------------------------------
#
# Project created by QtCreator 2018-03-01T10:56:33
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Creating
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainmenu.cpp \
    handlertopackages.cpp \
    mainpackage1properties.cpp \
    mainpackage2topics.cpp \
    mainpackage3objects.cpp \
    mainpackage4change.cpp \
    mainpackage.cpp \
    maintests.cpp \
    handlertotests.cpp

HEADERS += \
        mainmenu.h \
    common.h \
    mainpackage.h \
    handlertopackages.h \
    maintests.h \
    handlertotests.h

FORMS += \
        mainmenu.ui \
    mainpackage.ui \
    maintests.ui

RESOURCES += \
    resourses.qrc

RC_FILE += icon.qrc

DISTFILES += \
    sourceTest.test
