//
// Created by jackzhous on 2019/8/26.
//

#include "MediaSync.h"


MediaSync::MediaSync() {
    abort = false;
}


MediaSync::~MediaSync() {}


void MediaSync::run() {

}

void MediaSync::updateExternalClock(double pts) {}

void MediaSync::stop() {
    abort = true;
}


void MediaSync::updateAudioColock(double pts, double time) {
    audioClock->setClock(pts, time);
    extClock->syncToSlave(audioClock);
}

double MediaSync::getAudioDffClock() {
    return audioClock->getClock() - getMasterClock();
}

double MediaSync::getMasterClock() {
    double val = 0;
    switch (playerStatus->syncType){
        case AV_SYNC_VIDEO:
            val = videoClock->getClock();
            break

        case AV_SYNC_AUDIO:
            val = audioClock->getClock();
            break;

        case AV_SYNC_EXTERNAL:
            val = extClock->getClock();
            break;
    }

    return val;
}