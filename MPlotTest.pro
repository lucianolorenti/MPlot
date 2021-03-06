#-------------------------------------------------
#
# QMake Project for building an MPlot demo app
#
#-------------------------------------------------

TEMPLATE = app
QT+=widgets printsupport
TARGET = MPlotTest
DEPENDPATH += . \
	src \
	src/MPlot

INCLUDEPATH += include
QMAKE_CXXFLAGS+=-std=c++11
MPLOTLIBPATH = $${PWD}/lib
LIBS += -L$${MPLOTLIBPATH} -lMPlot

# Input
HEADERS +=

SOURCES += src/main.cpp
