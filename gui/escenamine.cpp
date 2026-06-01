#include "escenamine.h"
#include "mainwindow.h"
#include "escenadificultad.h"
#include "escenanivel2.h"
#include <QGraphicsProxyWidget>
#include <QApplication>

using namespace std;

EscenaMenu::EscenaMenu(MainWindow* ventana)
    : QGraphicsScene(0, 0, MainWindow::ANCHO, MainWindow::ALTO),
    ventana(ventana)
{
    QPixmap pixFondo(":/img/recursos/fondo_menu.png");
    fondo = addPixmap(pixFondo.scaled(MainWindow::ANCHO, MainWindow::ALTO,
                                      Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation));
    fondo->setZValue(0);

    btnNivel1 = new QPushButton("Nivel 1");
    btnNivel2 = new QPushButton("Nivel 2");
    btnSalir  = new QPushButton("Salir");

    configurarBoton(btnNivel1, "Nivel 1", 350, 280);
    configurarBoton(btnNivel2, "Nivel 2", 350, 360);
    configurarBoton(btnSalir,  "Salir",   350, 440);

    connect(btnNivel1, &QPushButton::clicked, this, &EscenaMenu::irNivel1);
    connect(btnNivel2, &QPushButton::clicked, this, &EscenaMenu::irNivel2);
    connect(btnSalir,  &QPushButton::clicked, this, &EscenaMenu::salir);
}

EscenaMenu::~EscenaMenu() {}

void EscenaMenu::configurarBoton(QPushButton* btn, const QString& texto,
                                 int x, int y) {
    btn->setText(texto);
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
    proxy->setZValue(1);
}

void EscenaMenu::irNivel1() {
    ventana->cambiarEscena(new EscenaDificultad(ventana));
}

void EscenaMenu::irNivel2() {
    ventana->cambiarEscena(new EscenaNivel2(ventana));
}

void EscenaMenu::salir() {
    QApplication::quit();
}
