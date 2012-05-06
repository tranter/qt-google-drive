#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T16:14:50
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = qt-google-drive
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp\
           oauth2.cpp\
           logindialog.cpp \
           driveengine.cpp \
           xmlparser.cpp \
           treeitem.cpp \
           treemodel.cpp \
    treeiteminfo.cpp \
    resmanager.cpp \
    xmlhandler.cpp

HEADERS  += mainwindow.h\
            oauth2.h\
            logindialog.h \
            driveengine.h \
            AppRegData.h \
            xmlparser.h \
            treeitem.h \
            treemodel.h \
    treeiteminfo.h \
    Def.h \
    resmanager.h \
    xmlhandler.h

FORMS    += mainwindow.ui\
            logindialog.ui

RESOURCES += \
    resource.qrc
