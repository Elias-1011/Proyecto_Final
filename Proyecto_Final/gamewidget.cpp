#include "gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , m_nivel(new Nivel_2())
    , m_frameActual(0)
    , m_tiempoAnimacion(0.0f)
    , m_enemyFrameActual(0)
    , m_enemyTimer(0.0f)
    , m_enemyAtacando(false)
    , m_enemyDireccion(-1)
    , m_jugadorEmbistioAntes(false)
    , m_jugadorSaltoAntes(true)
    , m_jugadorEsquivoAntes(false)
    , m_enemigoEmbistioAntes(false)
    , m_puntosJugadorAntes(0)
    , m_puntosEnemigoAntes(0)
    , m_nivelFinalizadoAntes(false)
{
    setFocusPolicy(Qt::StrongFocus);
    m_nivel->iniciar();

    // ── Fondo ────────────────────────────────────────────────────────────────
    m_fondo.load("Sprites/Fondo/fondo.png");
    m_imgVictoria.load("Sprites/Fondo/victoria.png");
    m_imgDerrota.load("Sprites/Fondo/derrota.png");

    // ── Jugador sprites ───────────────────────────────────────────────────────
    for (int i = 1; i <= 6; i++)
        m_walkDerecha.push_back(QPixmap(QString("Sprites/Jugador/walk%1.png").arg(i)));
    m_jumpSprites.push_back(QPixmap("Sprites/Jugador/jump1.png"));
    m_attackSprites.push_back(QPixmap("Sprites/Jugador/ataque1.png"));
    m_idleSprites.push_back(QPixmap("Sprites/Jugador/walk1.png"));

    // ── Enemigo sprites ───────────────────────────────────────────────────────
    for (int i = 1; i <= 6; i++)
        m_enemyWalk.push_back(QPixmap(QString("Sprites/Enemigo/walk%1.png").arg(i)));
    m_enemyAttack.push_back(QPixmap("Sprites/Enemigo/ataque1.png"));
    m_enemyIdle.push_back(QPixmap("Sprites/Enemigo/walk1.png"));

    // ── Música ────────────────────────────────────────────────────────────────
    m_sonidos.playMusica();

    // ── Timer ────────────────────────────────────────────────────────────────
    connect(&m_timer, &QTimer::timeout, this, &GameWidget::gameLoop);
    m_timer.start(16);
}

QSize GameWidget::sizeHint() const {
    return QSize(800, 600);
}

GameWidget::~GameWidget() {
    m_timer.stop();
    delete m_nivel;
}

QPixmap GameWidget::fallback(int w, int h, QColor color) const {
    QPixmap px(w, h);
    px.fill(color);
    return px;
}

void GameWidget::gameLoop() {
    const float dt = 0.016f;

    Principal* jugador = m_nivel->getJugador();
    Enemigo*   enemigo = m_nivel->getEnemigo();
    if (!jugador || !enemigo) return;

    // ── Capturar estado ANTES de actualizar ───────────────────────────────────
    bool jugadorEmbistiendo = jugador->estaEmbistiendo();
    bool jugadorEnSuelo     = jugador->estaEnSuelo();
    bool jugadorEsquivando  = jugador->estaEsquivando();
    bool enemigoEmbistiendo = enemigo->estaEmbistiendo();
    int  puntosJugador      = m_nivel->getPuntosJugador();
    int  puntosEnemigo      = m_nivel->getPuntosAgente();

    m_nivel->actualizar(dt);

    // ── Detectar cambios de estado para disparar sonidos ─────────────────────

    // Embestida jugador — sonido al iniciar
    if (jugador->estaEmbistiendo() && !m_jugadorEmbistioAntes)
        m_sonidos.playEmbestida();

    // Embestida enemigo — sonido al iniciar
    if (enemigo->estaEmbistiendo() && !m_enemigoEmbistioAntes)
        m_sonidos.playEmbestida();

    // Salto jugador — sonido al despegar
    if (!jugador->estaEnSuelo() && m_jugadorSaltoAntes)
        m_sonidos.playSalto();

    // Esquive jugador — sonido al iniciar
    if (jugador->estaEsquivando() && !m_jugadorEsquivoAntes)
        m_sonidos.playEsquive();

    // Golpe — sonido cuando cambia el marcador de toques
    // (significa que una embestida conectó)
    if (m_nivel->getPuntosJugador() != puntosJugador ||
        m_nivel->getPuntosAgente()  != puntosEnemigo) {
        m_sonidos.playPunto();
    } else if (m_nivel->getToquesJugador() > 0 &&
               jugadorEmbistiendo && !jugador->estaEmbistiendo()) {
        m_sonidos.playGolpe();
    } else if (m_nivel->getToquesAgente() > 0 &&
               enemigoEmbistiendo && !enemigo->estaEmbistiendo()) {
        m_sonidos.playGolpe();
    }

    // Victoria / derrota — sonido al finalizar el nivel
    if (m_nivel->nivelFinalizado() && !m_nivelFinalizadoAntes) {
        m_sonidos.stopMusica();
        if (m_nivel->jugadorGano())
            m_sonidos.playVictoria();
        else
            m_sonidos.playDerrota();
    }

    // ── Guardar estado actual para el próximo frame ───────────────────────────
    m_jugadorEmbistioAntes  = jugador->estaEmbistiendo();
    m_jugadorSaltoAntes     = jugador->estaEnSuelo();
    m_jugadorEsquivoAntes   = jugador->estaEsquivando();
    m_enemigoEmbistioAntes  = enemigo->estaEmbistiendo();
    m_puntosJugadorAntes    = m_nivel->getPuntosJugador();
    m_puntosEnemigoAntes    = m_nivel->getPuntosAgente();
    m_nivelFinalizadoAntes  = m_nivel->nivelFinalizado();

    // ── Animaciones ───────────────────────────────────────────────────────────
    if (m_walkDerecha.isEmpty())  m_walkDerecha.push_back(fallback(128,128,QColor(70,130,180)));
    if (m_jumpSprites.isEmpty())  m_jumpSprites.push_back(fallback(128,128,QColor(70,130,180)));
    if (m_attackSprites.isEmpty())m_attackSprites.push_back(fallback(128,128,QColor(70,130,180)));
    if (m_idleSprites.isEmpty())  m_idleSprites.push_back(fallback(128,128,QColor(70,130,180)));
    if (m_enemyWalk.isEmpty())    m_enemyWalk.push_back(fallback(128,128,QColor(180,60,60)));
    if (m_enemyAttack.isEmpty())  m_enemyAttack.push_back(fallback(128,128,QColor(180,60,60)));
    if (m_enemyIdle.isEmpty())    m_enemyIdle.push_back(fallback(128,128,QColor(180,60,60)));

    m_tiempoAnimacion += dt;
    m_enemyTimer      += dt;

    m_enemyDireccion = (enemigo->getX() < jugador->getX()) ? 1 : -1;
    m_enemyAtacando  = enemigo->estaEmbistiendo();

    float tiempoFrame = jugador->estaEmbistiendo() ? 0.08f :
                            !jugador->estaEnSuelo() ? 0.15f : 0.10f;

    if (m_tiempoAnimacion >= tiempoFrame) {
        m_tiempoAnimacion = 0.0f;
        if (jugador->estaEmbistiendo())
            m_frameActual = (m_frameActual + 1) % m_attackSprites.size();
        else if (!jugador->estaEnSuelo())
            m_frameActual = (m_frameActual + 1) % m_jumpSprites.size();
        else if (jugador->estaMoviendose())
            m_frameActual = (m_frameActual + 1) % m_walkDerecha.size();
        else
            m_frameActual = 0;
    }

    m_frameActual = qBound(0, m_frameActual, (int)m_walkDerecha.size() - 1);

    if (m_enemyTimer >= 0.12f) {
        m_enemyTimer = 0.0f;
        if (m_enemyAtacando)
            m_enemyFrameActual = (m_enemyFrameActual + 1) % m_enemyAttack.size();
        else if (enemigo->estaMoviendose())
            m_enemyFrameActual = (m_enemyFrameActual + 1) % m_enemyWalk.size();
        else
            m_enemyFrameActual = (m_enemyFrameActual + 1) % m_enemyIdle.size();
    }

    m_enemyFrameActual = qBound(0, m_enemyFrameActual, (int)m_enemyWalk.size() - 1);

    update();
}

void GameWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);

    if (!m_fondo.isNull())
        p.drawPixmap(rect(), m_fondo);
    else
        p.fillRect(rect(), QColor(30, 30, 30));

    p.setBrush(QColor(90, 70, 40));
    p.setPen(QPen(QColor(120, 90, 50), 2));
    p.drawRect(50, 520, 700, 20);

    Principal* jugador = m_nivel->getJugador();
    Enemigo*   enemigo = m_nivel->getEnemigo();
    if (!jugador || !enemigo) return;

    // ── Sprite jugador ────────────────────────────────────────────────────────
    QPixmap spriteJugador;
    if (jugador->estaEmbistiendo())
        spriteJugador = m_attackSprites.isEmpty() ? fallback(128,128,QColor(70,130,180))
                                                  : m_attackSprites[m_frameActual % m_attackSprites.size()];
    else if (!jugador->estaEnSuelo())
        spriteJugador = m_jumpSprites.isEmpty() ? fallback(128,128,QColor(70,130,180))
                                                : m_jumpSprites[m_frameActual % m_jumpSprites.size()];
    else if (jugador->estaMoviendose())
        spriteJugador = m_walkDerecha.isEmpty() ? fallback(128,128,QColor(70,130,180))
                                                : m_walkDerecha[m_frameActual % m_walkDerecha.size()];
    else
        spriteJugador = m_idleSprites.isEmpty() ? fallback(128,128,QColor(70,130,180))
                                                : m_idleSprites[0];

    if (jugador->getDireccion() < 0)
        spriteJugador = spriteJugador.transformed(QTransform().scale(-1, 1));

    p.drawPixmap(static_cast<int>(jugador->getX()) - 64,
                 static_cast<int>(jugador->getY()) - 128,
                 128, 128, spriteJugador);

    // ── Sprite enemigo ────────────────────────────────────────────────────────
    QPixmap spriteEnemigo;
    if (m_enemyAtacando)
        spriteEnemigo = m_enemyAttack.isEmpty() ? fallback(128,128,QColor(180,60,60))
                                                : m_enemyAttack[m_enemyFrameActual % m_enemyAttack.size()];
    else if (enemigo->estaMoviendose())
        spriteEnemigo = m_enemyWalk.isEmpty() ? fallback(128,128,QColor(180,60,60))
                                              : m_enemyWalk[m_enemyFrameActual % m_enemyWalk.size()];
    else
        spriteEnemigo = m_enemyIdle.isEmpty() ? fallback(128,128,QColor(180,60,60))
                                              : m_enemyIdle[m_enemyFrameActual % m_enemyIdle.size()];

    if (m_enemyDireccion == -1)
        spriteEnemigo = spriteEnemigo.transformed(QTransform().scale(-1, 1));

    p.drawPixmap(static_cast<int>(enemigo->getX()) - 64,
                 static_cast<int>(enemigo->getY()) - 128,
                 128, 128, spriteEnemigo);

    dibujarHUD(p);

    if (m_nivel->nivelFinalizado()) {
        bool gano = m_nivel->jugadorGano();

        // Mostrar imagen de victoria o derrota a pantalla completa
        QPixmap& imgFinal = gano ? m_imgVictoria : m_imgDerrota;

        if (!imgFinal.isNull()) {
            p.drawPixmap(rect(), imgFinal);
        } else {
            // Fallback si no carga la imagen
            p.setBrush(QColor(0, 0, 0, 160));
            p.setPen(Qt::NoPen);
            p.drawRect(rect());

            p.setFont(QFont("Arial", 32, QFont::Bold));
            p.setPen(gano ? QColor(100, 220, 100) : QColor(220, 80, 80));
            p.drawText(rect(), Qt::AlignCenter, gano ? "¡VICTORIA!" : "DERROTA");
        }

        // Marcador encima de la imagen
        p.setFont(QFont("Arial", 14, QFont::Bold));
        p.setPen(QColor(220, 200, 100));
        p.drawText(QRect(0, height()/2 + 50, width(), 30),
                   Qt::AlignCenter,
                   QString("Jugador %1  —  Enemigo %2")
                       .arg(m_nivel->getPuntosJugador())
                       .arg(m_nivel->getPuntosAgente()));
    }
}

void GameWidget::dibujarHUD(QPainter& p) {
    p.setBrush(QColor(0,0,0,130)); p.setPen(Qt::NoPen);
    p.drawRoundedRect(width()/2 - 60, 8, 120, 38, 8, 8);
    p.setFont(QFont("Arial", 16, QFont::Bold));
    p.setPen(QColor(255, 220, 80));
    p.drawText(QRect(width()/2 - 55, 10, 110, 34),
               Qt::AlignCenter,
               QString("%1  —  %2")
                   .arg(m_nivel->getPuntosJugador())
                   .arg(m_nivel->getPuntosAgente()));

    dibujarBarraToques(p, m_nivel->getToquesJugador(), 20,  12, true);
    dibujarBarraToques(p, m_nivel->getToquesAgente(),  width()-140, 12, false);

    if (m_nivel->getTemblorActivo()) {
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.setPen(QColor(255, 160, 0));
        p.drawText(QRect(0, height()-28, width(), 22),
                   Qt::AlignCenter, "⚠  TEMBLOR SÍSMICO  ⚠");
    }
}

void GameWidget::dibujarBarraToques(QPainter& p, int toques,
                                    int x, int y, bool esJugador) {
    QColor activo = esJugador ? QColor(70,130,180) : QColor(200,70,70);

    p.setFont(QFont("Arial", 8, QFont::Bold));
    p.setPen(QColor(220,220,220));
    p.drawText(x, y + 12, esJugador ? "JUGADOR" : "ENEMIGO");

    for (int i = 0; i < 3; i++) {
        int rx = x + i * 38;
        int ry = y + 16;
        p.setBrush(QColor(0,0,0,60)); p.setPen(Qt::NoPen);
        p.drawRoundedRect(rx+2, ry+2, 32, 14, 4, 4);
        p.setBrush(i < toques ? activo : QColor(50,50,50));
        p.setPen(QPen(QColor(200,200,200,100), 1));
        p.drawRoundedRect(rx, ry, 32, 14, 4, 4);
    }
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    InputManager& input = m_nivel->getInput();
    switch (event->key()) {
    case Qt::Key_A:     input.izquierda = true;  m_nivel->registrarAccionJugador(Accion::MOVER);    break;
    case Qt::Key_D:     input.derecha   = true;  m_nivel->registrarAccionJugador(Accion::MOVER);    break;
    case Qt::Key_W:     input.saltar    = true;  m_nivel->registrarAccionJugador(Accion::SALTAR);   break;
    case Qt::Key_Shift: input.esquivar  = true;  m_nivel->registrarAccionJugador(Accion::ESQUIVAR); break;
    case Qt::Key_Space: input.atacar    = true;  m_nivel->registrarAccionJugador(Accion::ATACAR);   break;
    default: break;
    }
    QWidget::keyPressEvent(event);
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
    InputManager& input = m_nivel->getInput();
    switch (event->key()) {
    case Qt::Key_A: input.izquierda = false; break;
    case Qt::Key_D: input.derecha   = false; break;
    default: break;
    }
    QWidget::keyReleaseEvent(event);
}
