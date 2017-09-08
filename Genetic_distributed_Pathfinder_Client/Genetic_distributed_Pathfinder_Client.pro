#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T14:24:07
#
#-------------------------------------------------

QT       += core
QT       -= gui
QT       += xml network
INCLUDEPATH += . $$PWD/libs/
LIBS += $$PWD/libs/maia.lib

DEPENDPATH += .
INCLUDEPATH += .

TARGET = ProjetSD_Client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    client.cpp \
    grille.cpp \
    humain.cpp \
    obstacle.cpp \
    village.cpp

HEADERS += \
    client.h \
    grille.h \
    humain.h \
    include.h \
    obstacle.h \
    village.h
