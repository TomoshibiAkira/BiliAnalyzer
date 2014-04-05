#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T09:54:05
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BiliAnalyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainframe.cpp \
    videoinfo.cpp \
    videoinfomodel.cpp \
    aboutwindow.cpp

HEADERS  += mainframe.h \
    videoinfo.h \
    status.h \
    videoinfomodel.h \
    vptr.h \
    aboutwindow.h

FORMS    += mainframe.ui \
    aboutwindow.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    icon.rc

RC_FILE += \
    icon.rc
