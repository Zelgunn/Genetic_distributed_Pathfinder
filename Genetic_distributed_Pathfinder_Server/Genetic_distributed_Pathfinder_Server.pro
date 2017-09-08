#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T09:57:47
#
#-------------------------------------------------

QT       += core gui
QT       += xml network
INCLUDEPATH += . $$PWD/libs/
LIBS += $$PWD/libs/maia.lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Genetic_distributed_Pathfinder_Server
TEMPLATE = app

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp\
        mainwindow.cpp \
    grille.cpp \
    humain.cpp \
    obstacle.cpp \
    village.cpp \
    world.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    grille.h \
    humain.h \
    include.h \
    obstacle.h \
    village.h \
    world.h \
    server.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
