#ifndef ESCENANIVEL2_H
#define ESCENANIVEL2_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>

class MainWindow;

// Espacio reservado para el Nivel 2 (trabajo del companiero).
// Por ahora muestra un texto y regresa al menu.

class EscenaNivel2 : public QGraphicsScene {
    Q_OBJECT

public:
    explicit EscenaNivel2(MainWindow* ventana);
    ~EscenaNivel2() override;

private slots:
    void volver();

private:
    MainWindow* ventana;
};

#endif
