#include "escenanivel1.h"
#include "mainwindow.h"
#include "escenatransicion.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QFont>
#include <algorithm>
#include <cmath>

using namespace std;

EscenaNivel1::EscenaNivel1(MainWindow* ventana, bool esDificil)
    : QGraphicsScene(0, 0, MainWindow::ANCHO, MainWindow::ALTO),
      ventana(ventana),
      dirActual(Direccion::Abajo),
      frameActual(0),
      ticksAnimacion(0),
      ticksAtaqueVisible(0)
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

    QFont fHUD("Arial", 14, QFont::Bold);

    textoTiempo = addText("Tiempo: 60s", fHUD);
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setPos(10, 10);
    textoTiempo->setZValue(4);

    textoRocas = addText("Rocas: 0", fHUD);
    textoRocas->setDefaultTextColor(Qt::white);
    textoRocas->setPos(10, 36);
    textoRocas->setZValue(4);

    textoVidas = addText("Vidas: 3", fHUD);
    textoVidas->setDefaultTextColor(QColor(255, 80, 80));
    textoVidas->setPos(10, 62);
    textoVidas->setZValue(4);

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
        sprJugadorArriba[i] = QPixmap(
            QString(":/img/recursos/jugador_arriba_%1.png").arg(i))
            .scaled(TAM_JUGADOR, TAM_JUGADOR,
                    Qt::KeepAspectRatio, Qt::SmoothTransformation);
        sprJugadorAbajo[i] = QPixmap(
            QString(":/img/recursos/jugador_abajo_%1.png").arg(i))
            .scaled(TAM_JUGADOR, TAM_JUGADOR,
                    Qt::KeepAspectRatio, Qt::SmoothTransformation);
        sprJugadorIzq[i] = QPixmap(
            QString(":/img/recursos/jugador_izq_%1.png").arg(i))
            .scaled(TAM_JUGADOR, TAM_JUGADOR,
                    Qt::KeepAspectRatio, Qt::SmoothTransformation);
        sprJugadorDer[i] = QPixmap(
            QString(":/img/recursos/jugador_der_%1.png").arg(i))
            .scaled(TAM_JUGADOR, TAM_JUGADOR,
                    Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    sprAtaque[0] = QPixmap(":/img/recursos/jugador_ataque_arriba.png")
                   .scaled(TAM_ATAQUE, TAM_ATAQUE,
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    sprAtaque[1] = QPixmap(":/img/recursos/jugador_ataque_abajo.png")
                   .scaled(TAM_ATAQUE, TAM_ATAQUE,
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    sprAtaque[2] = QPixmap(":/img/recursos/jugador_ataque_izq.png")
                   .scaled(TAM_ATAQUE, TAM_ATAQUE,
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
    sprAtaque[3] = QPixmap(":/img/recursos/jugador_ataque_der.png")
                   .scaled(TAM_ATAQUE, TAM_ATAQUE,
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

    sprImpacto = QPixmap(":/img/recursos/impacto.png")
                 .scaled(TAM_ROCA_MAX, TAM_ROCA_MAX,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation);

    for (short i = 0; i < 5; i++) {
        short tam = TAM_ROCA_BASE +
                    (TAM_ROCA_MAX - TAM_ROCA_BASE) * i / 4;
        sprRocas[i] = QPixmap(
            QString(":/img/recursos/roca_%1.png").arg(i))
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
        if (!nivel->estaAtacando()) {
            nivel->iniciarAtaque();
            ticksAtaqueVisible = TICKS_ATAQUE;
            verificarColisionAtaqueRoca();
        }
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

    sincronizarRocas();
    actualizarJugadorGrafico();
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
    if (!nivel->nivelTerminado()) {
        nivel->tickSegundo();
        if (nivel->nivelTerminado()) {
            timerLoop->stop();
            timerSpawn->stop();
            timerTemblor->stop();
            timerSegundo->stop();
            bool exitoso     = nivel->fueExitoso();
            MainWindow* vent = ventana;
            QTimer::singleShot(100, vent, [vent, exitoso]() {
                vent->cambiarEscena(
                    new EscenaTransicion(vent, exitoso, true)
                );
            });
        }
    }
}

short EscenaNivel1::getIdxDireccion() const {
    switch (dirActual) {
    case Direccion::Arriba:    return 0;
    case Direccion::Abajo:     return 1;
    case Direccion::Izquierda: return 2;
    case Direccion::Derecha:   return 3;
    default:                   return 1;
    }
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

void EscenaNivel1::actualizarJugadorGrafico() {
    const Jugador& jug = nivel->getJugador();
    float ox = nivel->getOffsetX();
    float oy = nivel->getOffsetY();

    if (ticksAtaqueVisible > 0) {
        ticksAtaqueVisible--;
        itemJugador->setPixmap(sprAtaque[getIdxDireccion()]);
        itemJugador->setPos(jug.getX() - TAM_ATAQUE / 2 + ox,
                            jug.getY() - TAM_ATAQUE / 2 + oy);
        if (ticksAtaqueVisible == 0)
            nivel->finalizarAtaque();
    } else {
        bool moviendose = (jug.getDirX() != 0.0f ||
                           jug.getDirY() != 0.0f);
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
        itemJugador->setPixmap(getSpriteSetActual()[frameActual]);
        itemJugador->setPos(jug.getX() - TAM_JUGADOR / 2 + ox,
                            jug.getY() - TAM_JUGADOR / 2 + oy);
    }
}

void EscenaNivel1::verificarColisionAtaqueRoca() {
    const Jugador&       jug   = nivel->getJugador();
    const vector<Roca*>& rocas = nivel->getRocas();

    for (short i = 0; i < (short)rocas.size(); i++) {
        if (!rocas[i]->estaActivo()) continue;

        float dx   = rocas[i]->getX() - jug.getX();
        float dy   = rocas[i]->getY() - jug.getY();
        float dist = sqrt(dx*dx + dy*dy);

        if (dist <= rocas[i]->getRadio() + RANGO_ATAQUE) {
            float dot = dx * jug.getDirX() + dy * jug.getDirY();
            if (dot > 0.0f) {
                nivel->registrarImpacto(i);
                return;
            }
        }
    }
    nivel->finalizarAtaque();
}

short EscenaNivel1::getFrameRoca(const Roca* r) const {
    float pct = r->getTamano() / r->getTamanoMax();
    short f   = (short)(pct * 4.0f);
    if (f > 4) f = 4;
    return f;
}

void EscenaNivel1::sincronizarRocas() {
    if (nivel->getHayImpacto()) {
        QGraphicsPixmapItem* itemImp = addPixmap(sprImpacto);
        itemImp->setPos(nivel->getImpactoX() - TAM_ROCA_MAX / 2,
                        nivel->getImpactoY() - TAM_ROCA_MAX / 2);
        itemImp->setZValue(3);
        impactos.append({itemImp, TICKS_IMPACTO});
        nivel->resetImpacto();
    }

    const vector<Roca*>& rocas = nivel->getRocas();

    while (itemsRocas.size() < (int)rocas.size()) {
        QGraphicsPixmapItem* item = addPixmap(QPixmap());
        item->setZValue(1);
        itemsRocas.append(item);
    }

    for (int i = 0; i < (int)rocas.size(); i++) {
        const Roca* r = rocas[i];
        short       f = getFrameRoca(r);
        short       t = (short)r->getTamano();

        if (itemsRocas[i]->pixmap().width() != sprRocas[f].width())
            itemsRocas[i]->setPixmap(sprRocas[f]);

        itemsRocas[i]->setPos(r->getX() - t / 2,
                               r->getY() - t / 2);
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
        "Rocas: "  + QString::number(nivel->getRocasDestr()) +
        " / "      + QString::number(nivel->getRocasObj()));
    textoVidas->setPlainText(
        "Vidas: "  + QString::number(nivel->getJugador().getVidas()));
    textoTemblor->setPlainText(
        nivel->getOffsetX() != 0.0f ? "TEMBLOR SISMICO!" : "");
}

void EscenaNivel1::verificarFin() {
    if (!nivel->nivelTerminado()) return;

    timerLoop->stop();
    timerSpawn->stop();
    timerTemblor->stop();
    timerSegundo->stop();

    bool exitoso     = nivel->fueExitoso();
    MainWindow* vent = ventana;
    QTimer::singleShot(100, vent, [vent, exitoso]() {
        vent->cambiarEscena(
            new EscenaTransicion(vent, exitoso, true)
        );
    });
}
