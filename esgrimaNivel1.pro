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
    logica/temblor.cpp \
    logica/nivel1.cpp \
    gui/escenamine.cpp \
    gui/escenadificultad.cpp \
    gui/escenatransicion.cpp \
    gui/escenanivel1.cpp \
    nivel2/combatiente.cpp \
    nivel2/principal.cpp \
    nivel2/enemigo.cpp \
    nivel2/nivel2.cpp \
    nivel2/gamewidget.cpp \
    nivel2/soundmanager.cpp

HEADERS += \
    mainwindow.h \
    logica/entidad.h \
    logica/personaje.h \
    logica/jugador.h \
    logica/roca.h \
    logica/temblor.h \
    logica/dificultad.h \
    logica/dificultadfacil.h \
    logica/dificultaddificil.h \
    logica/nivel1.h \
    gui/escenamine.h \
    gui/escenadificultad.h \
    gui/escenatransicion.h \
    gui/escenanivel1.h \
    nivel2/enums.h \
    nivel2/perfilJugador.h \
    nivel2/combatiente.h \
    nivel2/principal.h \
    nivel2/enemigo.h \
    nivel2/nivel2.h \
    nivel2/gamewidget.h \
    nivel2/soundmanager.h

FORMS += mainwindow.ui

# logica antes que nivel2 para que "temblor.h" resuelva a logica/temblor.h
INCLUDEPATH += logica gui nivel2

RESOURCES += recursos.qrc

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
