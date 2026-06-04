#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void playGolpe();
    void playEmbestida();
    void playSalto();
    void playEsquive();
    void playPunto();
    void playVictoria();
    void playDerrota();

    void playMusica();
    void stopMusica();

    void setVolumenEfectos(float v);
    void setVolumenMusica(float v);

private:
    QMediaPlayer*  m_golpe;
    QMediaPlayer*  m_embestida;
    QMediaPlayer*  m_salto;
    QMediaPlayer*  m_esquive;
    QMediaPlayer*  m_punto;
    QMediaPlayer*  m_victoria;
    QMediaPlayer*  m_derrota;
    QMediaPlayer*  m_musica;

    QAudioOutput*  m_outGolpe;
    QAudioOutput*  m_outEmbestida;
    QAudioOutput*  m_outSalto;
    QAudioOutput*  m_outEsquive;
    QAudioOutput*  m_outPunto;
    QAudioOutput*  m_outVictoria;
    QAudioOutput*  m_outDerrota;
    QAudioOutput*  m_outMusica;

    void cargarEfecto(QMediaPlayer* player, QAudioOutput* output,
                      const QString& ruta, float volumen = 0.8f);
};

#endif // SOUNDMANAGER_H
