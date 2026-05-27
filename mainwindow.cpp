#include "mainwindow.h"
#include "escenamine.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    escenaActual(nullptr)
{
    setFixedSize(ANCHO, ALTO);
    setWindowTitle("Igneos: Duelo Primordial");

    view = new QGraphicsView(this);
    view->setFixedSize(ANCHO, ALTO);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameStyle(0);
    setCentralWidget(view);
    EscenaMenu* menu = new EscenaMenu(this);
    cambiarEscena(menu);
}

MainWindow::~MainWindow() {
    delete escenaActual;
}

void MainWindow::cambiarEscena(QGraphicsScene* nuevaEscena) {
    if (escenaActual) {
        view->setScene(nullptr);
        delete escenaActual;
    }
    escenaActual = nuevaEscena;
    view->setScene(escenaActual);
}
