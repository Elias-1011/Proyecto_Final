#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPixmap>
#include <QVector>
#include "nivel2.h"
#include "soundmanager.h"

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget() override;

    QSize sizeHint() const override;
    void  reiniciar();

signals:
    void juegoTerminado();

protected:
    void paintEvent(QPaintEvent* event)    override;
    void keyPressEvent(QKeyEvent* event)   override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void gameLoop();

private:
    Nivel_2*     m_nivel;
    SoundManager m_sonidos;
    QTimer       m_timer;

    bool  m_enTransicion;
    float m_tiempoTransicion;
    float m_duracionTransicion;
    int   m_rondaActual;
    QVector<QPixmap> m_imgRondas;

    bool m_jugadorEmbistioAntes;
    bool m_jugadorSaltoAntes;
    bool m_jugadorEsquivoAntes;
    bool m_enemigoEmbistioAntes;
    int  m_puntosJugadorAntes;
    int  m_puntosEnemigoAntes;
    bool m_nivelFinalizadoAntes;

    QPixmap m_fondo;
    QPixmap m_imgVictoria;
    QPixmap m_imgDerrota;

    QVector<QPixmap> m_walkDerecha;
    QVector<QPixmap> m_jumpSprites;
    QVector<QPixmap> m_attackSprites;
    QVector<QPixmap> m_idleSprites;
    int   m_frameActual;
    float m_tiempoAnimacion;

    QVector<QPixmap> m_enemyWalk;
    QVector<QPixmap> m_enemyAttack;
    QVector<QPixmap> m_enemyIdle;
    QVector<QPixmap> m_enemyJump;
    int   m_enemyFrameActual;
    float m_enemyTimer;
    bool  m_enemyAtacando;
    int   m_enemyDireccion;

    void cargarSprites();
    void fixNullPixmaps(QVector<QPixmap>& sprites, QColor color);
    void dibujarHUD(QPainter& p);
    void dibujarBarraToques(QPainter& p, int toques, int x, int y, bool esJugador);
    QPixmap fallback(int w, int h, QColor color) const;
    void iniciarTransicion();
};

#endif