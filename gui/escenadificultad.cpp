#include "escenadificultad.h"
#include "mainwindow.h"
#include "escenatransicion.h"
#include "escenamine.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QFont>

using namespace std;

EscenaDificultad::EscenaDificultad(MainWindow* ventana)
    : QGraphicsScene(0, 0, MainWindow::ANCHO, MainWindow::ALTO),
    ventana(ventana)
{
    QGraphicsPixmapItem* fondo = addPixmap(
        QPixmap(":/n1img/fondo_menu.png").scaled(
            MainWindow::ANCHO, MainWindow::ALTO,
            Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
        );
    fondo->setZValue(0);

    QGraphicsTextItem* titulo = addText("Selecciona la dificultad");
    titulo->setDefaultTextColor(QColor(255, 102, 0));
    titulo->setFont(QFont("Arial", 26, QFont::Bold));
    titulo->setPos(200, 180);
    titulo->setZValue(1);

    btnFacil   = new QPushButton("Facil");
    btnDificil = new QPushButton("Dificil");
    btnVolver  = new QPushButton("Volver");

    configurarBoton(btnFacil,   350, 300);
    configurarBoton(btnDificil, 350, 380);
    configurarBoton(btnVolver,  350, 480);

    connect(btnFacil,   &QPushButton::clicked, this, &EscenaDificultad::elegirFacil);
    connect(btnDificil, &QPushButton::clicked, this, &EscenaDificultad::elegirDificil);
    connect(btnVolver,  &QPushButton::clicked, this, &EscenaDificultad::volver);

    musicaDificultad = new QMediaPlayer(this);
    audioDificultad  = new QAudioOutput(this);
    musicaDificultad->setAudioOutput(audioDificultad);
    musicaDificultad->setSource(QUrl("qrc:/n1snd/sonido_menu.mp3"));
    audioDificultad->setVolume(0.6f);
    musicaDificultad->setLoops(QMediaPlayer::Infinite);
    musicaDificultad->play();
}

EscenaDificultad::~EscenaDificultad() {
    musicaDificultad->stop();
}

void EscenaDificultad::configurarBoton(QPushButton* btn, int x, int y) {
    btn->setFixedSize(200, 60);
    btn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(30, 15, 5, 200);"
        "  color: #FF6600;"
        "  border: 2px solid #FF6600;"
        "  border-radius: 8px;"
        "  font-size: 18px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(255, 102, 0, 180);"
        "  color: white;"
        "}"
        );
    QGraphicsProxyWidget* proxy = addWidget(btn);
    proxy->setPos(x, y);
    proxy->setZValue(2);
}

void EscenaDificultad::elegirFacil()   { ventana->cambiarEscena(new EscenaTransicion(ventana, false)); }
void EscenaDificultad::elegirDificil() { ventana->cambiarEscena(new EscenaTransicion(ventana, true));  }
void EscenaDificultad::volver()        { ventana->cambiarEscena(new EscenaMenu(ventana));              }
