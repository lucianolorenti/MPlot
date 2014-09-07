#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T23:23:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Examples
TEMPLATE = app

INCLUDEPATH+= ../src/
QMAKE_CXXFLAGS=-std=c++11
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS+= -L$${PWD}/../lib -lMPlot
