QT -= gui
QT  = core

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET   = esgrimaNivel1
TEMPLATE = app

SOURCES += \
    main.cpp \
    logica/entidad.cpp \
    logica/personaje.cpp \
    logica/jugador.cpp \
    logica/roca.cpp \
    logica/fisicatemblor.cpp \
    logica/nivel1.cpp

HEADERS += \
    logica/entidad.h \
    logica/personaje.h \
    logica/jugador.h \
    logica/roca.h \
    logica/fisicatemblor.h \
    logica/dificultad.h \
    logica/dificultadfacil.h \
    logica/dificultaddificil.h \
    logica/nivel1.h

INCLUDEPATH += logica
