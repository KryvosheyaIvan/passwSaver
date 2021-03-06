#-------------------------------------------------
#
# Project created by QtCreator 2019-03-10T22:57:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pswdSaver
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
        modules/deletePassw/deletepassw.cpp \
        modules/createPassw/createpassw.cpp \
        modules/userProfiles/userprofiles.cpp \
        modules/passwWindow/passw.cpp

HEADERS += \
        mainwindow.h \
        modules/deletePassw/deletepassw.h \
        modules/createPassw/createpassw.h \
        modules/userProfiles/userprofiles.h \
        modules/passwWindow/passw.h

FORMS += \
        mainwindow.ui \
        modules/deletePassw/deletepassw.ui \
        modules/createPassw/createpassw.ui \
        modules/passwWindow/passw.ui

TRANSLATIONS = \
        translations/QtLanguage_Ukr.ts \
        translations/QtLanguage_Ger.ts \
        translations/QtLanguage_Eng.ts \
        translations/QtLanguage_Rus.ts

RC_ICONS = icon/keys.ico

RESOURCES += \
        resource.qrc

DISTFILES +=
