#-------------------------------------------------
#
# Project created by QtCreator 2013-01-07T18:15:02
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TARGET = MovieMonitor
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    connection.cpp \
    inotify-cxx/inotify-cxx.cpp \
    mmonitor.cpp \
    notifyworker.cpp \
    QsLog/QsLogDest.cpp \
    QsLog/QsLog.cpp \
    QsLog/QsDebugOutput.cpp

HEADERS += \
    connection.h \
    inotify-cxx/inotify-cxx.h \
    mmonitor.h \
    notifyworker.h \
    QsLog/QsLogDest.h \
    QsLog/QsLog.h \
    QsLog/QsDebugOutput.h

QTPLUGIN += qsqlmysql
