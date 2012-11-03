#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T16:14:50
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = qtdrive
TEMPLATE = app

SOURCES += core/main.cpp
SOURCES += gui/forms/createfolderdialog.cpp
SOURCES += gui/forms/mainwindow.cpp
SOURCES += network/oauth2.cpp
SOURCES += gui/forms/logindialog.cpp
SOURCES += core/driveengine.cpp
SOURCES += parsers/xmlparser.cpp
SOURCES += gui/iteminfo.cpp
SOURCES += gui/resmanager.cpp
SOURCES += parsers/xmlhandler.cpp
SOURCES += network/downloadmanager.cpp
SOURCES += gui/forms/settingsdialog.cpp
SOURCES += share/commontools.cpp
SOURCES += network/uploadmanager.cpp
SOURCES += network/networkmanager.cpp
SOURCES += gui/forms/progressbardialog.cpp
SOURCES += network/filesmanager.cpp
SOURCES += network/foldersmanager.cpp
SOURCES += gui/additionalfoldesmanager.cpp
SOURCES += network/contentmanager.cpp
SOURCES += network/operationsmanager.cpp
SOURCES += parsers/jsonparser.cpp
SOURCES += gui/filestransferui.cpp
SOURCES += gui/operationsui.cpp
SOURCES += gui/filesui.cpp
SOURCES += gui/foldersui.cpp
SOURCES += gui/checkui.cpp

HEADERS  += gui/forms/mainwindow.h
HEADERS  += share/debug.h
HEADERS  += share/enums.h
HEADERS  += gui/forms/createfolderdialog.h
HEADERS  += network/oauth2.h
HEADERS  += gui/forms/logindialog.h
HEADERS  += core/driveengine.h
HEADERS  += share/appregdata.h
HEADERS  += parsers/xmlparser.h
HEADERS  += gui/iteminfo.h
HEADERS  += share/def.h
HEADERS  += gui/resmanager.h
HEADERS  += network/downloadmanager.h
HEADERS  += gui/forms/settingsdialog.h
HEADERS  += share/commontools.h
HEADERS  += network/uploadmanager.h
HEADERS  += gui/forms/progressbardialog.h
HEADERS  += network/filesmanager.h
HEADERS  += network/foldersmanager.h
HEADERS  += network/networkmanager.h
HEADERS  += parsers/xmlhandler.h
HEADERS  += network/contentmanager.h
HEADERS  += gui/additionalfoldesmanager.h
HEADERS  += network/operationsmanager.h
HEADERS  += share/singleton.h
HEADERS  += core/eventhandler.h
HEADERS  += parsers/jsonparser.h
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
