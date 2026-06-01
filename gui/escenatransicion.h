#ifndef ESCENATRANSICION_H
#define ESCENATRANSICION_H

#include <QGraphicsScene>
#include <QTimer>

class MainWindow;

enum class TipoTransicion {
    InicioNivel1,
    Victoria,
    Derrota
};

class EscenaTransicion : public QGraphicsScene {
    Q_OBJECT

public:
    EscenaTransicion(MainWindow* ventana, bool esDificil);
    EscenaTransicion(MainWindow* ventana, bool victoria, bool dummy);
    ~EscenaTransicion() override;

private slots:
    void avanzar();

private:
    MainWindow*    ventana;
    QTimer*        timer;
    TipoTransicion tipo;
    bool           dificil;

    void construir(const QString& rutaImagen);
};

#endif
