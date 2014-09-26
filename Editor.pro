#-------------------------------------------------
#
# Project created by QtCreator 2014-09-19T20:11:07
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp

HEADERS  += mainwindow.h \
    cell.h

FORMS    += mainwindow.ui \
    cell.ui
QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += \
    tile.qrc

OTHER_FILES +=
