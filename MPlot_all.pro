TEMPLATE = subdirs

SUBDIRS = MPlotLib \
            MPlotTest \
    Examples


Examples.depends = MPlotLib
MPlotLib.file = MPlotLib.pro
MPlotTest.file = MPlotTest.pro
