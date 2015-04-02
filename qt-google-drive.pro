#-------------------------------------------------
#
# Project created by QtCreator 2012-04-25T16:14:50
#
#-------------------------------------------------

QT += core gui webkit network xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets


CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

TARGET = gdfm
TEMPLATE = app

SOURCES += core/main.cpp
SOURCES += network/auth.cpp
SOURCES += gui/forms/authdialog.cpp
SOURCES += settings/settingsmanager.cpp
SOURCES += settings/accountinfo.cpp
SOURCES += gui/accountsmanager.cpp
SOURCES += network/queries.cpp
SOURCES += gui/forms/filepanel.cpp
SOURCES += gui/forms/drivetree.cpp
SOURCES += gui/forms/additionalfolders.cpp
SOURCES += gui/forms/sharedialog.cpp
SOURCES += gui/forms/createfolderdialog.cpp
SOURCES += gui/forms/mainwindow.cpp
SOURCES += core/driveengine.cpp
SOURCES += parsers/xmlparser.cpp
SOURCES += gui/iteminfo.cpp
SOURCES += network/resmanager.cpp
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
SOURCES += gui/controls/toolbar.cpp
SOURCES += gui/controls/combobox.cpp

HEADERS += gui/forms/mainwindow.h
HEADERS += network/auth.h
HEADERS += gui/forms/authdialog.h
HEADERS += settings/settingsmanager.h
HEADERS += settings/accountinfo.h
HEADERS += gui/accountsmanager.h
HEADERS += network/queries.h
HEADERS += gui/forms/filepanel.h
HEADERS += gui/forms/drivetree.h
HEADERS += gui/forms/additionalfolders.h
HEADERS += gui/forms/sharedialog.h
HEADERS += share/registration.h
HEADERS += share/defs.h
HEADERS += share/debug.h
HEADERS += share/enums.h
HEADERS += gui/forms/createfolderdialog.h
HEADERS += core/driveengine.h
HEADERS += parsers/xmlparser.h
HEADERS += gui/iteminfo.h
HEADERS += network/resmanager.h
HEADERS += network/downloadmanager.h
HEADERS += gui/forms/settingsdialog.h
HEADERS += share/commontools.h
HEADERS += network/uploadmanager.h
HEADERS += gui/forms/progressbardialog.h
HEADERS += network/filesmanager.h
HEADERS += network/foldersmanager.h
HEADERS += network/networkmanager.h
HEADERS += parsers/xmlhandler.h
HEADERS += network/contentmanager.h
HEADERS += gui/additionalfoldesmanager.h
HEADERS += network/operationsmanager.h
HEADERS += share/singleton.h
HEADERS += core/eventhandler.h
HEADERS += parsers/jsonparser.h
HEADERS += gui/filestransferui.h
HEADERS += gui/operationsui.h
HEADERS += gui/filesui.h
HEADERS += gui/foldersui.h
HEADERS += gui/checkui.h
HEADERS += gui/controls/toolbar.h
HEADERS += gui/controls/combobox.h

FORMS += gui/forms/mainwindow.ui
FORMS += gui/forms/authdialog.ui
FORMS += gui/forms/filepanel.ui
FORMS += gui/forms/drivetree.ui
FORMS += gui/forms/additionalfolders.ui
FORMS += gui/forms/sharedialog.ui
FORMS += gui/forms/createfolderdialog.ui
FORMS += gui/forms/settingsdialog.ui
FORMS += gui/forms/progressbardialog.ui

RESOURCES += resource.qrc
