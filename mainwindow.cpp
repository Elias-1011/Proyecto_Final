#include "mainwindow.h"
#include "escenamine.h"
#include "gamewidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    escenaActual(nullptr)
{
    setFixedSize(ANCHO, ALTO);
    setWindowTitle("Igneos: Duelo Primordial");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    view = new QGraphicsView(this);
    view->setFixedSize(ANCHO, ALTO);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameStyle(0);
    stack->addWidget(view);       // indice 0: menus y Nivel 1

    gameWidget = new GameWidget(this);
    stack->addWidget(gameWidget); // indice 1: Nivel 2

    connect(gameWidget, &GameWidget::juegoTerminado,
            this, &MainWindow::volverAlMenu);

    stack->setCurrentIndex(0);
    cambiarEscena(new EscenaMenu(this));
}

MainWindow::~MainWindow() {
    delete escenaActual;
}

void MainWindow::cambiarEscena(QGraphicsScene* nuevaEscena) {
    if (escenaActual) {
        view->setScene(nullptr);
        escenaActual->deleteLater();
    }
    escenaActual = nuevaEscena;
    view->setScene(escenaActual);
    stack->setCurrentIndex(0);
}

void MainWindow::irNivel2() {
    gameWidget->reiniciar();
    stack->setCurrentIndex(1);
    gameWidget->setFocus();
}

void MainWindow::volverAlMenu() {
    stack->setCurrentIndex(0);
    cambiarEscena(new EscenaMenu(this));
}
