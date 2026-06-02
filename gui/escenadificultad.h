#ifndef ESCENADIFICULTAD_H
#define ESCENADIFICULTAD_H

#include <QGraphicsScene>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>

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

    QMediaPlayer* musicaDificultad;
    QAudioOutput* audioDificultad;

    void configurarBoton(QPushButton* btn, int x, int y);
};

#endif
