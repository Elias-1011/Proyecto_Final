#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QStackedWidget>

class GameWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void cambiarEscena(QGraphicsScene* nuevaEscena);
    void irNivel2();

    QGraphicsView* getView() const { return view; }

    static constexpr int ANCHO = 900;
    static constexpr int ALTO  = 700;

public slots:
    void volverAlMenu();

private:
    QStackedWidget* stack;
    QGraphicsView*  view;
    QGraphicsScene* escenaActual;
    GameWidget*     gameWidget;
};

#endif