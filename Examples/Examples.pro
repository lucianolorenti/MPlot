#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T23:23:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Examples
TEMPLATE = app

INCLUDEPATH+= ../src/MPlot/
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
