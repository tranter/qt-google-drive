#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T16:14:50
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = qtdrive
TEMPLATE = app

SOURCES += main.cpp
SOURCES += gui/forms/createfolderdialog.cpp
SOURCES += gui/forms/mainwindow.cpp
SOURCES += oauth2.cpp
SOURCES += gui/forms/logindialog.cpp
SOURCES += driveengine.cpp
SOURCES += xmlparser.cpp
SOURCES += iteminfo.cpp
SOURCES += resmanager.cpp
SOURCES += xmlhandler.cpp
SOURCES += network/downloadmanager.cpp
SOURCES += gui/forms/settingsdialog.cpp
SOURCES += commontools.cpp
SOURCES += network/uploadmanager.cpp
SOURCES += network/networkmanager.cpp
SOURCES += gui/forms/progressbardialog.cpp
SOURCES += network/filesmanager.cpp
SOURCES += network/foldersmanager.cpp
SOURCES += additionalfoldesmanager.cpp
SOURCES += network/contentmanager.cpp
SOURCES += operationsmanager.cpp
SOURCES += jsonparser.cpp
SOURCES += gui/filestransferui.cpp
SOURCES += gui/operationsui.cpp
SOURCES += gui/filesui.cpp
SOURCES += gui/foldersui.cpp
SOURCES += gui/checkui.cpp

HEADERS  += gui/forms/mainwindow.h
HEADERS  += enums.h
HEADERS  += gui/forms/createfolderdialog.h
HEADERS  += oauth2.h
HEADERS  += gui/forms/logindialog.h
HEADERS  += driveengine.h
HEADERS  += AppRegData.h
HEADERS  += xmlparser.h
HEADERS  += iteminfo.h
HEADERS  += Def.h
HEADERS  += resmanager.h
HEADERS  += network/downloadmanager.h
HEADERS  += gui/forms/settingsdialog.h
HEADERS  += commontools.h
HEADERS  += network/uploadmanager.h
HEADERS  += gui/forms/progressbardialog.h
HEADERS  += network/filesmanager.h
HEADERS  += network/foldersmanager.h
HEADERS  += network/networkmanager.h
HEADERS  += xmlhandler.h
HEADERS  += network/contentmanager.h
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
