#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T16:14:50
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = qtdrive
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp\
           oauth2.cpp\
           logindialog.cpp \
           driveengine.cpp \
           xmlparser.cpp \
    treeiteminfo.cpp \
    resmanager.cpp \
    xmlhandler.cpp \
    downloadmanager.cpp\
    settingsdialog.cpp \
    commontools.cpp \
    uploadmanager.cpp \
    networkmanager.cpp \
    progressbardialog.cpp \
    filesmanager.cpp \
    foldersmanager.cpp

HEADERS  += mainwindow.h\
            oauth2.h\
            logindialog.h \
            driveengine.h \
            AppRegData.h \
            xmlparser.h \
    treeiteminfo.h \
    Def.h \
    resmanager.h \
    downloadmanager.h\
    settingsdialog.h \
    commontools.h \
    uploadmanager.h \
    progressbardialog.h \
    filesmanager.h \
    foldersmanager.h

HEADERS  +=  networkmanager.h
HEADERS  +=  xmlhandler.h \

FORMS    += mainwindow.ui\
            logindialog.ui\
             settingsdialog.ui \
    progressbardialog.ui

RESOURCES += \
    resource.qrc
