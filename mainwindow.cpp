#include "mainwindow.h"
#include "escenamine.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    escenaActual(nullptr)
{
    setFixedSize(ANCHO, ALTO);
    setWindowTitle("ProyectoFinal Esgrima");

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
}

void MainWindow::cambiarEscena(QGraphicsScene* nuevaEscena) {
    if (escenaActual) {
        view->setScene(nullptr);
        escenaActual->deleteLater();
    }
    escenaActual = nuevaEscena;
    view->setScene(escenaActual);
}
