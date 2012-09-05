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
SOURCES += iteminfo.cpp
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
SOURCES += jsonparser.cpp
SOURCES += gui/filestransferui.cpp
SOURCES += gui/operationsui.cpp
SOURCES += gui/filesui.cpp
SOURCES += gui/foldersui.cpp
SOURCES += gui/checkui.cpp

HEADERS  += mainwindow.h
HEADERS  += enums.h
HEADERS  += createfolderdialog.h
HEADERS  += oauth2.h
HEADERS  += logindialog.h
HEADERS  += driveengine.h
HEADERS  += AppRegData.h
HEADERS  += xmlparser.h
HEADERS  += iteminfo.h
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
HEADERS  += singleton.h
HEADERS  += eventhandler.h
HEADERS  += jsonparser.h
HEADERS  += gui/filestransferui.h
HEADERS  += gui/operationsui.h
HEADERS  += gui/filesui.h
HEADERS  += gui/foldersui.h
HEADERS  += gui/checkui.h

FORMS    += gui/forms/mainwindow.ui
FORMS    += gui/forms/createfolderdialog.ui
FORMS    += gui/forms/logindialog.ui
FORMS    += gui/forms/settingsdialog.ui
FORMS    += gui/forms/progressbardialog.ui

RESOURCES += resource.qrc
