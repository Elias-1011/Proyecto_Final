#ifndef ESCENATRANSICION_H
#define ESCENATRANSICION_H

#include <QGraphicsScene>
#include <QTimer>

class MainWindow;

// Tipos de transicion posibles
enum class TipoTransicion {
    InicioNivel1,
    Victoria,
    Derrota
};

class EscenaTransicion : public QGraphicsScene {
    Q_OBJECT

public:
    // Constructor para inicio de nivel (viene de seleccion de dificultad)
    EscenaTransicion(MainWindow* ventana, bool esDificil);

    // Constructor para fin de nivel
    EscenaTransicion(MainWindow* ventana, bool victoria, bool dummy);

    ~EscenaTransicion() override;

private slots:
    void avanzar();

private:
    MainWindow*     ventana;
    QTimer*         timer;
    TipoTransicion  tipo;
    bool            dificil;

    void construir(const QString& rutaImagen);
};

#endif
