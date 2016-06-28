#include "soundplayer.h"

SoundPlayer::SoundPlayer()
{
    m_alarmSound.setSource(QUrl::fromLocalFile(":/new/sound/perfect.mp3"));
    m_alarmSound.moveToThread(&m_thread);
    QObject::connect(this, SIGNAL(play()), &m_alarmSound, SLOT(play()));
    m_thread.start(); // QThread::exec() will be called for you, making the thread wait for events
}

