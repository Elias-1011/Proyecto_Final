#include "escenanivel2.h"
#include "mainwindow.h"
#include "escenamine.h"
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QFont>

using namespace std;

EscenaNivel2::EscenaNivel2(MainWindow* ventana)
    : QGraphicsScene(0, 0, MainWindow::ANCHO, MainWindow::ALTO),
    ventana(ventana)
{
    setBackgroundBrush(QColor(10, 10, 20));

    QGraphicsTextItem* txt = addText("Nivel 2 — Proximamente");
    txt->setDefaultTextColor(Qt::white);
    txt->setFont(QFont("Arial", 24, QFont::Bold));
    txt->setPos(230, 300);

    QPushButton* btnVolver = new QPushButton("Volver al menu");
    btnVolver->setFixedSize(200, 55);
    btnVolver->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; color: white;"
        "border: 2px solid white; border-radius: 8px; font-size: 16px; }"
        "QPushButton:hover { background-color: white; color: black; }"
        );
    QGraphicsProxyWidget* proxy = addWidget(btnVolver);
    proxy->setPos(350, 390);

    connect(btnVolver, &QPushButton::clicked, this, &EscenaNivel2::volver);
}

EscenaNivel2::~EscenaNivel2() {}

void EscenaNivel2::volver() {
    ventana->cambiarEscena(new EscenaMenu(ventana));
}
