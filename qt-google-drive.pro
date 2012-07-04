#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T16:14:50
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = qtdrive
TEMPLATE = app

SOURCES += main.cpp
SOURCES += createfolderdialog.cpp
SOURCES += mainwindow.cpp
SOURCES += oauth2.cpp
SOURCES += logindialog.cpp
SOURCES += driveengine.cpp
SOURCES += xmlparser.cpp
SOURCES += treeiteminfo.cpp
SOURCES += resmanager.cpp
SOURCES += xmlhandler.cpp
SOURCES += downloadmanager.cpp
SOURCES += settingsdialog.cpp
SOURCES += commontools.cpp
SOURCES += uploadmanager.cpp
SOURCES += networkmanager.cpp
SOURCES += progressbardialog.cpp
SOURCES += filesmanager.cpp
SOURCES += foldersmanager.cpp
SOURCES += additionalfoldesmanager.cpp
SOURCES += contentmanager.cpp
SOURCES += operationsmanager.cpp
SOURCES += filestransfer.cpp

HEADERS  += mainwindow.h
HEADERS  += createfolderdialog.h
HEADERS  += oauth2.h
HEADERS  += logindialog.h
HEADERS  += driveengine.h
HEADERS  += AppRegData.h
HEADERS  += xmlparser.h
HEADERS  += treeiteminfo.h
HEADERS  += Def.h
HEADERS  += resmanager.h
HEADERS  += downloadmanager.h
HEADERS  += settingsdialog.h
HEADERS  += commontools.h
HEADERS  += uploadmanager.h
HEADERS  += progressbardialog.h
HEADERS  += filesmanager.h
HEADERS  += foldersmanager.h
HEADERS  += networkmanager.h
HEADERS  += xmlhandler.h
HEADERS  += contentmanager.h
HEADERS  += additionalfoldesmanager.h
HEADERS  += operationsmanager.h
HEADERS  += filestransfer.h
HEADERS  += singleton.h

FORMS    += mainwindow.ui
FORMS    += createfolderdialog.ui
FORMS    += logindialog.ui
FORMS    += settingsdialog.ui
FORMS    += progressbardialog.ui

RESOURCES += resource.qrc
