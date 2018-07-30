#-------------------------------------------------
#
# Project created by QtCreator 2018-07-26T14:19:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = roster
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    downloader.cpp \
    config.cpp \
    rosterloader.cpp \
    rostermodel.cpp

HEADERS += \
    mainwindow.h \
    downloader.h \
    config.h \
    rosterloader.h \
    globals.h \
    rostermodel.h

FORMS += \
    mainwindow.ui

CONFIG_FILE = config.json

DISTFILES += \
    $$CONFIG_FILE

#copy config file to build directory
DEFINES += CONFIG_FILE=\\\"$$CONFIG_FILE\\\"
copydata.commands = $(COPY_DIR) $$PWD/$$CONFIG_FILE $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
