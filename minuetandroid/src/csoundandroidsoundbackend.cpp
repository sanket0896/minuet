/****************************************************************************
**
** Copyright (C) 2016 by Sandro S. Andrade <sandroandrade@kde.org>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License as
** published by the Free Software Foundation; either version 2 of
** the License or (at your option) version 3 or any later version
** accepted by the membership of KDE e.V. (or its successor approved
** by the membership of KDE e.V.), which shall act as a proxy
** defined in Section 14 of version 3 of the license.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "csoundandroidsoundbackend.h"
#include "csengine.h"

#include <QtMath>
#include <QLoggingCategory>

#include <QtQml>

Q_DECLARE_LOGGING_CATEGORY(MINUETANDROID)


CsoundAndroidSoundBackend::CsoundAndroidSoundBackend(QObject *parent):
    Minuet::ISoundBackend(parent),
    m_csoundEngine(new CsEngine)
{
    qmlRegisterType<CsoundAndroidSoundBackend>("org.kde.minuet", 1, 0, "CsoundAndroidSoundBackend");
    setQuestionLabel("new question");
    connect(m_csoundEngine, &CsEngine::finished, this, [=]() {
        setQuestionLabel(QStringLiteral("play again"));
    });
}

void CsoundAndroidSoundBackend::clearExercise()
{
    QFile dfile("./test1.csd");
    if(dfile.exists())
        QFile::remove("./test1.csd");
    QFile sfile("assets:/share/test1.csd");
    if (sfile.exists())
    {
        sfile.copy("./test1.csd");
        QFile::setPermissions("./test1.csd",QFile::WriteOwner | QFile::ReadOwner);
    }
}

void CsoundAndroidSoundBackend::appendEvent(QList<unsigned int> midiNotes,QList<unsigned int> barStartInfo)
{
    //TODO : use grantlee processing or any other text template library
    QString content;
    QFile m_csdFileOpen("./test1.csd");
    if(!m_csdFileOpen.isOpen()){
        m_csdFileOpen.open(QIODevice::ReadWrite | QIODevice::Text);
    }
    QString lineData;
    QTextStream in(&m_csdFileOpen);
    while(!in.atEnd()){
        lineData = in.readLine();
        content= content + lineData + "\n";
        if(lineData.contains("<CsScore>")){
            for(int i=0 ; i<midiNotes.count() ; i++){
                QString initScore = "i 1 " + QString::number(barStartInfo.at(i)) + " " + QString::number(1) + " " + QString::number(midiNotes.at(i)) + " 100"+ "\n" ;
                content = content + initScore;
            }
            QString instrInit = "i 99 0 " + QString::number(barStartInfo.at(barStartInfo.count()-1)+1) + "\ne\n";//instrument will be active till the end of the notes +1 second
            content = content + instrInit;
        }
    }
    m_csdFileOpen.seek(0);
    QByteArray contentByte = content.toUtf8();
    m_csdFileOpen.write(contentByte);
}

CsoundAndroidSoundBackend::~CsoundAndroidSoundBackend()
{
    delete m_csoundEngine;
}

void CsoundAndroidSoundBackend::prepareFromExerciseOptions(QJsonArray selectedExerciseOptions,const QString &playMode){
    /*Song *song = new Song;
    song->setHeader(0, 1, 60);
    song->setInitialTempo(600000);
    m_song.reset(song);

    if (m_song->initialTempo() == 0)
        m_song->setInitialTempo(600000);
    appendEvent(new drumstick::TempoEvent(m_queueId, 600000), 0);
    */
    clearExercise();
    unsigned int barStart = 0;
    /*if (playMode == "rhythm") {
        appendEvent(new drumstick::NoteOnEvent(9, 80, 120), 0);
        appendEvent(new drumstick::NoteOnEvent(9, 80, 120), 60);
        appendEvent(new drumstick::NoteOnEvent(9, 80, 120), 120);
        appendEvent(new drumstick::NoteOnEvent(9, 80, 120), 180);
        barStart = 240;
    }*/
    QList<unsigned int> midiNotes;
    QList<unsigned int> barStartInfo;

    for (int i = 0; i < selectedExerciseOptions.size(); ++i) {
        QString sequence = selectedExerciseOptions[i].toObject()[QStringLiteral("sequence")].toString();

        unsigned int chosenRootNote = selectedExerciseOptions[i].toObject()[QStringLiteral("rootNote")].toString().toInt();
        if (playMode != "rhythm") {
            //appendEvent(new drumstick::NoteOnEvent(1, chosenRootNote, 120), barStart);
            //appendEvent(new drumstick::NoteOffEvent(1, chosenRootNote, 120), barStart + 60);
            midiNotes.append(chosenRootNote);
            barStartInfo.append(barStart);

            unsigned int j = 1;
            //drumstick::SequencerEvent *ev;
            foreach(const QString &additionalNote, sequence.split(' ')) {
                midiNotes.append(chosenRootNote+additionalNote.toInt());
                barStartInfo.append((m_playMode == ScalePlayMode) ? barStart+j:barStart);
                ++j;
            }
        }
        else {
            //TODO: Implement for rhythm
            /*appendEvent(new drumstick::NoteOnEvent(9, 80, 120), barStart);
            foreach(QString additionalNote, sequence.split(' ')) { // krazy:exclude=foreach
                appendEvent(new drumstick::NoteOnEvent(9, 37, 120), barStart);
                float dotted = 1;
                if (additionalNote.endsWith('.')) {
                    dotted = 1.5;
                    additionalNote.chop(1);
                }
                barStart += dotted*60*(4.0/additionalNote.toInt());
            }
        */}
    }
    appendEvent(midiNotes,barStartInfo);

    /*if (playMode == "rhythm")
        appendEvent(new drumstick::NoteOnEvent(9, 80, 120), barStart);*/

}

void CsoundAndroidSoundBackend::prepareFromMidiFile(const QString &fileName){
    Q_UNUSED(fileName)
}

void CsoundAndroidSoundBackend::play(){
    m_csoundEngine->start();
}

void CsoundAndroidSoundBackend::pause(){
    
}

void CsoundAndroidSoundBackend::stop(){
    m_csoundEngine->stop();
}

void CsoundAndroidSoundBackend::setPitch (qint8 pitch){
    Q_UNUSED(pitch)
}


void CsoundAndroidSoundBackend::setVolume (quint8 volume){
    Q_UNUSED(volume)
}


void CsoundAndroidSoundBackend::setTempo (quint8 tempo){
    Q_UNUSED(tempo)
}

//#include "moc_csoundandroidsoundbackend.cpp"

