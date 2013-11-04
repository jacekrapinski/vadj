#-------------------------------------------------
#
# Project created by QtCreator 2013-10-07T11:30:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bogusz
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wykres.cpp \
    qcustomplot.cpp \
    wykres_obserwacje.cpp \
    aux.cpp

HEADERS  += mainwindow.h \
    wykres.h \
    qcustomplot.h \
    wykres_obserwacje.h \
    aux.h

FORMS    += mainwindow.ui \
    wykres.ui \
    wykres_obserwacje.ui


#unix|win32: LIBS += -larmadillo


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/

INCLUDEPATH += $$PWD/../../../../../usr/include/qwt
DEPENDPATH += $$PWD/../../../../../usr/include/qwt

QT += printsupport

OTHER_FILES += \
    TODO.txt
