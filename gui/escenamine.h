#ifndef ESCENAMINE_H
#define ESCENAMINE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>

class MainWindow;

class EscenaMenu : public QGraphicsScene {
    Q_OBJECT

public:
    explicit EscenaMenu(MainWindow* ventana);
    ~EscenaMenu() override;

private slots:
    void irNivel1();
    void irNivel2();
    void salir();

private:
    MainWindow*          ventana;
    QGraphicsPixmapItem* fondo;
    QPushButton*         btnNivel1;
    QPushButton*         btnNivel2;
    QPushButton*         btnSalir;

    void configurarBoton(QPushButton* btn, const QString& texto,
                         int x, int y);
};

#endif
