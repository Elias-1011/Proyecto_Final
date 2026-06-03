#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include <QVector>
#include "nivel2.h"
#include "soundmanager.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    QSize sizeHint() const override;
protected:
    void paintEvent(QPaintEvent *event)    override;
    void keyPressEvent(QKeyEvent *event)   override;
    void keyReleaseEvent(QKeyEvent *event) override;
private slots:
    void gameLoop();
private:
    Nivel_2*      m_nivel;
    SoundManager  m_sonidos;
    QTimer        m_timer;

    // ── Estados anteriores para detectar cambios ──────────────────────────────
    bool m_jugadorEmbistioAntes;
    bool m_jugadorSaltoAntes;
    bool m_jugadorEsquivoAntes;
    bool m_enemigoEmbistioAntes;
    int  m_puntosJugadorAntes;
    int  m_puntosEnemigoAntes;
    bool m_nivelFinalizadoAntes;

    // ── Fondo ────────────────────────────────────────────────────────────────
    QPixmap m_fondo;
    QPixmap m_imgVictoria;
    QPixmap m_imgDerrota;

    // ── Jugador ───────────────────────────────────────────────────────────────
    QVector<QPixmap> m_walkDerecha;
    QVector<QPixmap> m_jumpSprites;
    QVector<QPixmap> m_attackSprites;
    QVector<QPixmap> m_idleSprites;
    int   m_frameActual;
    float m_tiempoAnimacion;

    // ── Enemigo ───────────────────────────────────────────────────────────────
    QVector<QPixmap> m_enemyWalk;
    QVector<QPixmap> m_enemyAttack;
    QVector<QPixmap> m_enemyIdle;
    int   m_enemyFrameActual;
    float m_enemyTimer;
    bool  m_enemyAtacando;
    int   m_enemyDireccion;

    // ── Helpers ───────────────────────────────────────────────────────────────
    void dibujarHUD(QPainter& p);
    void dibujarBarraToques(QPainter& p, int toques, int x, int y, bool esJugador);
    QPixmap fallback(int w, int h, QColor color) const;
};
#endif // GAMEWIDGET_H
