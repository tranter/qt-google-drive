#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T16:14:50
#
#-------------------------------------------------

QT       += core gui webkit network

TARGET = qt-google-drive
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        oauth2.cpp\
        logindialog.cpp \
    driveengine.cpp

HEADERS  += mainwindow.h\
            oauth2.h\
            logindialog.h \
    driveengine.h

FORMS    += mainwindow.ui\
            logindialog.ui
