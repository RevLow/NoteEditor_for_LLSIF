#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T13:40:27
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoteEditor_for_LLSIF
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    noteeditscene.cpp \
    note.cpp \
    notegraphicsview.cpp \
    newfiledialog.cpp \
    notebackgrid.cpp \
    settingdialog.cpp

macx{
    SOURCES += settingdialog-mac.cpp
}

HEADERS  += mainwindow.h \
    noteeditscene.h \
    note.h \
    notegraphicsview.h \
    newfiledialog.h \
    notebackgrid.h \
    settingdialog.h

FORMS    += mainwindow.ui \
    newfiledialog.ui \
    settingdialog.ui

CONFIG+=c++11

RESOURCES += \
    resources.qrc

DISTFILES += \
    setting.ini
