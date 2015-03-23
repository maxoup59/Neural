#-------------------------------------------------
#
# Project created by QtCreator 2015-03-22T12:44:07
#
#-------------------------------------------------

QT += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Neural
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    neuron.cpp \
    brain.cpp \
    survey.cpp \
    poney.cpp \
    price.cpp

HEADERS  += mainwindow.h \
    neuron.h \
    brain.h \
    survey.h \
    poney.h \
    price.h

FORMS    += mainwindow.ui
