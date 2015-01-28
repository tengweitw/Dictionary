#-------------------------------------------------
#
# Project created by QtCreator 2014-08-03T15:26:37
#
#-------------------------------------------------
QT +=core gui network webkit multimedia
QT +=webkit
QT +=network
QT += webkitwidgets
CONFIG += qaxcontainer


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dictionary
TEMPLATE = app


SOURCES += main.cpp\
        searchwindow.cpp \
    basicwindow.cpp \
    logindialog.cpp \
    network.cpp \
    word.cpp \
    localdict.cpp \
    wordbook.cpp \
    qmenubutton.cpp \
    completelineedit.cpp \
    libmanager.cpp \
    wordmodify.cpp

HEADERS  += searchwindow.h \
    basicwindow.h \
    logindialog.h \
    network.h \
    word.h \
    localdict.h \
    wordbook.h \
    qmenubutton.h \
    completelineedit.h \
    libmanager.h \
    wordmodify.h

FORMS    += searchwindow.ui \
    basicwindow.ui \
    logindialog.ui \
    wordbook.ui \
    libmanager.ui \
    wordmodify.ui

RESOURCES += \
    Images.qrc
RC_FILE +=myico.rc
