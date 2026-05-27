#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    void cambiarEscena(QGraphicsScene* nuevaEscena);

    QGraphicsView* getView() const { return view; }

    static constexpr int ANCHO = 900;
    static constexpr int ALTO  = 700;

private:
    QGraphicsView*  view;
    QGraphicsScene* escenaActual;
};

#endif
