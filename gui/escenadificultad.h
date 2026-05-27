#ifndef ESCENADIFICULTAD_H
#define ESCENADIFICULTAD_H

#include <QGraphicsScene>
#include <QPushButton>

class MainWindow;

class EscenaDificultad : public QGraphicsScene {
    Q_OBJECT

public:
    explicit EscenaDificultad(MainWindow* ventana);
    ~EscenaDificultad() override;

private slots:
    void elegirFacil();
    void elegirDificil();
    void volver();

private:
    MainWindow*  ventana;
    QPushButton* btnFacil;
    QPushButton* btnDificil;
    QPushButton* btnVolver;

    void configurarBoton(QPushButton* btn, int x, int y);
};

#endif
