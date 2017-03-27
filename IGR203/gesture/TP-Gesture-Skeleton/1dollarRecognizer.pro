#-------------------------------------------------
#
# Project created by QtCreator 2015-02-25T16:31:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 1dollarRecognizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gesturedrawer.cpp \
    gesturetest.cpp \
    gesturetraining.cpp \
#    onedollargesture.cpp \
    onedollarrecognizer.cpp \
    gesturetemplate.cpp \
    onedollarutil.cpp

HEADERS  += mainwindow.h \
    gesturedrawer.h \
    gesturetest.h \
    gesturetraining.h \
    onedollargesture.h \
    onedollarrecognizer.h \
    gesturetemplate.h \
    onedollarutil.h
