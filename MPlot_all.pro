TEMPLATE = subdirs

SUBDIRS = MPlotLib \
            MPlotTest \
    Examples

CONFIG += ordered

MPlotLib.file = MPlotLib.pro
MPlotTest.file = MPlotTest.pro
