#include "soundmanager.h"

SoundManager::SoundManager() {
    m_golpe      = new QMediaPlayer();  m_outGolpe      = new QAudioOutput();
    m_embestida  = new QMediaPlayer();  m_outEmbestida  = new QAudioOutput();
    m_salto      = new QMediaPlayer();  m_outSalto      = new QAudioOutput();
    m_esquive    = new QMediaPlayer();  m_outEsquive    = new QAudioOutput();
    m_punto      = new QMediaPlayer();  m_outPunto      = new QAudioOutput();
    m_victoria   = new QMediaPlayer();  m_outVictoria   = new QAudioOutput();
    m_derrota    = new QMediaPlayer();  m_outDerrota    = new QAudioOutput();
    m_musica     = new QMediaPlayer();  m_outMusica     = new QAudioOutput();

    cargarEfecto(m_golpe,     m_outGolpe,     "Sounds/golpe.mp3");
    cargarEfecto(m_embestida, m_outEmbestida, "Sounds/embestida.mp3");
    cargarEfecto(m_salto,     m_outSalto,     "Sounds/salto.mp3");
    cargarEfecto(m_esquive,   m_outEsquive,   "Sounds/esquive.mp3");
    cargarEfecto(m_punto,     m_outPunto,     "Sounds/punto.mp3");
    cargarEfecto(m_victoria,  m_outVictoria,  "Sounds/victoria.mp3");
    cargarEfecto(m_derrota,   m_outDerrota,   "Sounds/derrota.mp3");
    cargarEfecto(m_musica,    m_outMusica,    "Sounds/musica_fondo.mp3", 0.5f);

    m_musica->setLoops(QMediaPlayer::Infinite);
}

SoundManager::~SoundManager() {
    delete m_golpe;     delete m_outGolpe;
    delete m_embestida; delete m_outEmbestida;
    delete m_salto;     delete m_outSalto;
    delete m_esquive;   delete m_outEsquive;
    delete m_punto;     delete m_outPunto;
    delete m_victoria;  delete m_outVictoria;
    delete m_derrota;   delete m_outDerrota;
    delete m_musica;    delete m_outMusica;
}

void SoundManager::cargarEfecto(QMediaPlayer* player, QAudioOutput* output,
                                const QString& ruta, float volumen) {
    player->setAudioOutput(output);
    player->setSource(QUrl::fromLocalFile(ruta));
    output->setVolume(volumen);
}

void SoundManager::playGolpe() {
    m_golpe->setPosition(0);
    m_golpe->play();
}
void SoundManager::playEmbestida() {
    m_embestida->setPosition(0);
    m_embestida->play();
}
void SoundManager::playSalto() {
    m_salto->setPosition(0);
    m_salto->play();
}
void SoundManager::playEsquive() {
    m_esquive->setPosition(0);
    m_esquive->play();
}
void SoundManager::playPunto() {
    m_punto->setPosition(0);
    m_punto->play();
}
void SoundManager::playVictoria() {
    m_victoria->setPosition(0);
    m_victoria->play();
}
void SoundManager::playDerrota() {
    m_derrota->setPosition(0);
    m_derrota->play();
}
void SoundManager::playMusica()  { m_musica->play();  }
void SoundManager::stopMusica()  { m_musica->stop();  }

void SoundManager::setVolumenEfectos(float v) {
    m_outGolpe->setVolume(v);
    m_outEmbestida->setVolume(v);
    m_outSalto->setVolume(v);
    m_outEsquive->setVolume(v);
    m_outPunto->setVolume(v);
    m_outVictoria->setVolume(v);
    m_outDerrota->setVolume(v);
}

void SoundManager::setVolumenMusica(float v) {
    m_outMusica->setVolume(v);
}
