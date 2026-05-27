#include "escenatransicion.h"
#include "mainwindow.h"
#include "escenanivel1.h"
#include "escenamine.h"
#include <QGraphicsPixmapItem>

using namespace std;

EscenaTransicion::EscenaTransicion(MainWindow* ventana, bool esDificil)
    : QGraphicsScene(0, 0, MainWindow::ANCHO, MainWindow::ALTO),
    ventana(ventana),
    tipo(TipoTransicion::InicioNivel1),
    dificil(esDificil)
{
    timer = new QTimer(this);
    construir(":/img/recursos/transicion_inicio.png");
}

EscenaTransicion::EscenaTransicion(MainWindow* ventana, bool victoria, bool)
    : QGraphicsScene(0, 0, MainWindow::ANCHO, MainWindow::ALTO),
    ventana(ventana),
    tipo(victoria ? TipoTransicion::Victoria : TipoTransicion::Derrota),
    dificil(false)
{
    timer = new QTimer(this);
    QString ruta = victoria ? ":/img/recursos/transicion_fin_victoria.png"
                            : ":/img/recursos/transicion_fin_derrota.png";
    construir(ruta);
}

EscenaTransicion::~EscenaTransicion() {}

void EscenaTransicion::construir(const QString& rutaImagen) {
    QPixmap pix(rutaImagen);
    QGraphicsPixmapItem* img = addPixmap(
        pix.scaled(MainWindow::ANCHO, MainWindow::ALTO,
                   Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
        );
    img->setZValue(0);

    connect(timer, &QTimer::timeout, this, &EscenaTransicion::avanzar);
    timer->setSingleShot(true);
    timer->start(2000);
}

void EscenaTransicion::avanzar() {
    if (tipo == TipoTransicion::InicioNivel1) {
        ventana->cambiarEscena(new EscenaNivel1(ventana, dificil));
    } else {
        ventana->cambiarEscena(new EscenaMenu(ventana));
    }
}
