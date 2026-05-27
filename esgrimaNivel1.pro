QT += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET   = esgrimaNivel1
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    logica/entidad.cpp \
    logica/personaje.cpp \
    logica/jugador.cpp \
    logica/roca.cpp \
    logica/fisicatemblor.cpp \
    logica/nivel1.cpp \
    gui/escenamine.cpp \
    gui/escenadificultad.cpp \
    gui/escenatransicion.cpp \
    gui/escenanivel1.cpp \
    gui/escenanivel2.cpp

HEADERS += \
    mainwindow.h \
    logica/entidad.h \
    logica/personaje.h \
    logica/jugador.h \
    logica/roca.h \
    logica/fisicatemblor.h \
    logica/dificultad.h \
    logica/dificultadfacil.h \
    logica/dificultaddificil.h \
    logica/nivel1.h \
    gui/escenamine.h \
    gui/escenadificultad.h \
    gui/escenatransicion.h \
    gui/escenanivel1.h \
    gui/escenanivel2.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += logica gui

RESOURCES += recursos.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
