#include "escenanivel1.h"
#include "mainwindow.h"
#include "escenatransicion.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QFont>
#include <cmath>

using namespace std;

static constexpr short TICKS_POR_FRAME = 8;
static constexpr short TICKS_IMPACTO   = 18;
static constexpr short TAM_JUGADOR     = 64;
static constexpr short TAM_ROCA_BASE   = 20;
static constexpr short TAM_ROCA_MAX    = 80;

EscenaNivel1::EscenaNivel1(MainWindow* ventana, bool esDificil)
    : QGraphicsScene(0, 0, MainWindow::ANCHO, MainWindow::ALTO),
      ventana(ventana),
      dirActual(Direccion::Abajo),
      frameActual(0),
      ticksAnimacion(0),
      estabaAtacando(false),
      cantRocasAntes(0)
{
    dif   = esDificil ? (Dificultad*) new DificultadDificil()
                      : (Dificultad*) new DificultadFacil();
    nivel = new Nivel1(dif, 60);

    cargarSprites();

    itemFondo = addPixmap(
        QPixmap(":/img/recursos/fondo_escenario.png").scaled(
            MainWindow::ANCHO, MainWindow::ALTO,
            Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
    );
    itemFondo->setZValue(0);

    itemJugador = addPixmap(sprJugadorAbajo[0]);
    itemJugador->setZValue(2);
    itemJugador->setTransformOriginPoint(TAM_JUGADOR / 2, TAM_JUGADOR / 2);

    QFont fHUD("Arial", 14, QFont::Bold);

    textoTiempo = addText("Tiempo: 60s", fHUD);
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setPos(10, 10);
    textoTiempo->setZValue(4);

    textoRocas = addText("Rocas: 0", fHUD);
    textoRocas->setDefaultTextColor(Qt::white);
    textoRocas->setPos(10, 36);
    textoRocas->setZValue(4);

    textoTemblor = addText("");
    textoTemblor->setDefaultTextColor(QColor(255, 80, 0));
    textoTemblor->setFont(QFont("Arial", 13, QFont::Bold));
    textoTemblor->setPos(300, 10);
    textoTemblor->setZValue(4);

    ultimoMs = QDateTime::currentMSecsSinceEpoch();

    timerLoop = new QTimer(this);
    connect(timerLoop, &QTimer::timeout,
            this, &EscenaNivel1::actualizarFrame);
    timerLoop->start(16);

    timerSpawn = new QTimer(this);
    connect(timerSpawn, &QTimer::timeout,
            this, &EscenaNivel1::spawnRocaAleatoria);
    timerSpawn->start(dif->getIntervaloSpawnMs());

    timerTemblor = new QTimer(this);
    connect(timerTemblor, &QTimer::timeout,
            this, &EscenaNivel1::dispararTemblor);
    timerTemblor->start(dif->getIntervaloTemblorMs());

    timerSegundo = new QTimer(this);
    connect(timerSegundo, &QTimer::timeout,
            this, &EscenaNivel1::tickSegundo);
    timerSegundo->start(1000);
}

EscenaNivel1::~EscenaNivel1() {
    delete nivel;
    delete dif;
}

void EscenaNivel1::cargarSprites() {
    for (short i = 0; i < 3; i++) {
        sprJugadorArriba[i] = QPixmap(QString(":/img/recursos/jugador_arriba_%1.png").arg(i))
                              .scaled(TAM_JUGADOR, TAM_JUGADOR,
                                      Qt::KeepAspectRatio, Qt::SmoothTransformation);
        sprJugadorAbajo[i]  = QPixmap(QString(":/img/recursos/jugador_abajo_%1.png").arg(i))
                              .scaled(TAM_JUGADOR, TAM_JUGADOR,
                                      Qt::KeepAspectRatio, Qt::SmoothTransformation);
        sprJugadorIzq[i]    = QPixmap(QString(":/img/recursos/jugador_izq_%1.png").arg(i))
                              .scaled(TAM_JUGADOR, TAM_JUGADOR,
                                      Qt::KeepAspectRatio, Qt::SmoothTransformation);
        sprJugadorDer[i]    = QPixmap(QString(":/img/recursos/jugador_der_%1.png").arg(i))
                              .scaled(TAM_JUGADOR, TAM_JUGADOR,
                                      Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    sprAtaque  = QPixmap(":/img/recursos/jugador_ataque.png")
                 .scaled(TAM_JUGADOR, TAM_JUGADOR,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    sprImpacto = QPixmap(":/img/recursos/impacto.png")
                 .scaled(TAM_ROCA_MAX, TAM_ROCA_MAX,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation);

    for (short i = 0; i < 5; i++) {
        short tam = TAM_ROCA_BASE + (TAM_ROCA_MAX - TAM_ROCA_BASE) * i / 4;
        sprRocas[i] = QPixmap(QString(":/img/recursos/roca_%1.png").arg(i))
                      .scaled(tam, tam,
                              Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

void EscenaNivel1::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    case Qt::Key_W:
        nivel->moverJugador(true, false, false, false);
        dirActual = Direccion::Arriba;
        break;
    case Qt::Key_S:
        nivel->moverJugador(false, true, false, false);
        dirActual = Direccion::Abajo;
        break;
    case Qt::Key_A:
        nivel->moverJugador(false, false, true, false);
        dirActual = Direccion::Izquierda;
        break;
    case Qt::Key_D:
        nivel->moverJugador(false, false, false, true);
        dirActual = Direccion::Derecha;
        break;
    case Qt::Key_Space:
        nivel->accionAtacar();
        break;
    default:
        break;
    }
}

void EscenaNivel1::keyReleaseEvent(QKeyEvent* e) {
    switch (e->key()) {
    case Qt::Key_W: nivel->moverJugador(false, false, false, false); break;
    case Qt::Key_S: nivel->moverJugador(false, false, false, false); break;
    case Qt::Key_A: nivel->moverJugador(false, false, false, false); break;
    case Qt::Key_D: nivel->moverJugador(false, false, false, false); break;
    default: break;
    }
}

void EscenaNivel1::actualizarFrame() {
    if (nivel->nivelTerminado()) return;

    qint64 ahora = QDateTime::currentMSecsSinceEpoch();
    float  dt    = (ahora - ultimoMs) / 1000.0f;
    ultimoMs     = ahora;
    if (dt > 0.05f) dt = 0.05f;

    nivel->actualizar(dt);

    actualizarJugadorGrafico();
    sincronizarRocas();
    actualizarImpactos();
    actualizarHUD();
    verificarFin();
}

void EscenaNivel1::spawnRocaAleatoria() {
    if (nivel->nivelTerminado()) return;
    float rx = 80.0f + QRandomGenerator::global()->bounded(
                           MainWindow::ANCHO - 160);
    float ry = 80.0f + QRandomGenerator::global()->bounded(
                           MainWindow::ALTO  - 160);
    nivel->spawnRoca(rx, ry);
}

void EscenaNivel1::dispararTemblor() {
    if (!nivel->nivelTerminado())
        nivel->activarTemblor();
}

void EscenaNivel1::tickSegundo() {
    if (!nivel->nivelTerminado())
        nivel->tickSegundo();
}

void EscenaNivel1::actualizarJugadorGrafico() {
    const Jugador& jug = nivel->getJugador();
    float ox = nivel->getOffsetX();
    float oy = nivel->getOffsetY();

    bool atacando = nivel->getLanza().activa;

    if (atacando) {
        itemJugador->setPixmap(sprAtaque);
        ticksAnimacion = 0;
        frameActual    = 0;
    } else {
        bool moviendose = jug.estaAtacando() == false &&
                          (jug.getDirX() != 0.0f || jug.getDirY() != 0.0f);

        if (moviendose) {
            ticksAnimacion++;
            if (ticksAnimacion >= TICKS_POR_FRAME) {
                ticksAnimacion = 0;
                frameActual    = (frameActual + 1) % 3;
            }
        } else {
            frameActual    = 0;
            ticksAnimacion = 0;
        }

        QPixmap* set = getSpriteSetActual();
        itemJugador->setPixmap(set[frameActual]);
    }
    itemJugador->setPos(jug.getX() - TAM_JUGADOR / 2 + ox,
                        jug.getY() - TAM_JUGADOR / 2 + oy);

    estabaAtacando = atacando;
}

QPixmap* EscenaNivel1::getSpriteSetActual() {
    switch (dirActual) {
    case Direccion::Arriba:    return sprJugadorArriba;
    case Direccion::Abajo:     return sprJugadorAbajo;
    case Direccion::Izquierda: return sprJugadorIzq;
    case Direccion::Derecha:   return sprJugadorDer;
    default:                   return sprJugadorAbajo;
    }
}

short EscenaNivel1::getFrameRoca(const Roca* r) const {
    float pct = r->getTamano() / r->getTamanoMax();
    short f   = (short)(pct * 4.0f);
    if (f > 4) f = 4;
    return f;
}

void EscenaNivel1::sincronizarRocas() {
    const vector<Roca*>& rocas = nivel->getRocas();

    short cantAhora = nivel->getRocasDestr();
    if (cantAhora > cantRocasAntes) {
        for (Roca* r : rocas) {
            if (r->fueDestruida() && !r->estaActivo()) {
                QGraphicsPixmapItem* itemImp = addPixmap(sprImpacto);
                itemImp->setPos(r->getX() - TAM_ROCA_MAX / 2,
                                r->getY() - TAM_ROCA_MAX / 2);
                itemImp->setZValue(3);
                impactos.append({itemImp, TICKS_IMPACTO});
                break;
            }
        }
        cantRocasAntes = cantAhora;
    }

    while (itemsRocas.size() < (int)rocas.size()) {
        QGraphicsPixmapItem* item = addPixmap(QPixmap());
        item->setZValue(1);
        itemsRocas.append(item);
    }
    for (int i = 0; i < (int)rocas.size(); i++) {
        Roca* r     = rocas[i];
        short frame = getFrameRoca(r);
        itemsRocas[i]->setPixmap(sprRocas[frame]);
        itemsRocas[i]->setPos(r->getX() - r->getTamano() / 2,
                               r->getY() - r->getTamano() / 2);
        itemsRocas[i]->setVisible(r->estaActivo());
    }

    for (int i = (int)rocas.size(); i < itemsRocas.size(); i++)
        itemsRocas[i]->setVisible(false);
}


void EscenaNivel1::actualizarImpactos() {
    for (EfectoImpacto& ef : impactos)
        ef.framesTick--;
    impactos.erase(
        remove_if(impactos.begin(), impactos.end(),
                  [this](EfectoImpacto& ef) {
                      if (ef.framesTick <= 0) {
                          removeItem(ef.item);
                          delete ef.item;
                          return true;
                      }
                      return false;
                  }),
        impactos.end()
    );
}

void EscenaNivel1::actualizarHUD() {
    textoTiempo->setPlainText(
        "Tiempo: " + QString::number(nivel->getTiempo()) + "s");
    textoRocas->setPlainText(
        "Rocas: " + QString::number(nivel->getRocasDestr()) +
        " / "     + QString::number(nivel->getRocasObj()));
    textoTemblor->setPlainText(
        nivel->getOffsetX() != 0.0f ? "TEMBLOR SISMICO!" : "");
}

void EscenaNivel1::verificarFin() {
    if (!nivel->nivelTerminado()) return;

    timerLoop->stop();
    timerSpawn->stop();
    timerTemblor->stop();
    timerSegundo->stop();

    ventana->cambiarEscena(
        new EscenaTransicion(ventana, nivel->fueExitoso(), true)
    );
}
