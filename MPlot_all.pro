TEMPLATE = subdirs

SUBDIRS = MPlotLib \
    Examples


Examples.depends = MPlotLib
MPlotLib.file = MPlotLib.pro
MPlotTest.file = MPlotTest.pro
