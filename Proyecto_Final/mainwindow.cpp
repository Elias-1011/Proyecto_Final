#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(new GameWidget(this))
{
    ui->setupUi(this);

    setCentralWidget(m_game);

    setWindowTitle("El Duelo del Cráter");

    setFixedSize(m_game->sizeHint());

    m_game->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}
