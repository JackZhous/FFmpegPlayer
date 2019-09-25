//
// Created by jackzhous on 2019/8/13.
//
#include "PlayerStatus.h"

PlayerStatus::PlayerStatus() {
    reset();
    queue = new MessageQueue();
}

PlayerStatus::~PlayerStatus() {
    if(queue){
        delete queue;
        queue = NULL;
    }
}

void PlayerStatus::setVideoPath(char *path) {
    this->vedioPath = path;
}

void PlayerStatus::setVideoUrl(char *url) {
    this->url = url;
}

void PlayerStatus::reset() {
    url = NULL;
    vedioPath = NULL;
    pauseRequest = 0;
    abortRequest = 0;
    mDuration = -1;
    reorderVideoPts = -1;
    seekRequest = 0;
    isLiveStream = false;
    playDuration = AV_NOPTS_VALUE;
    start_time = AV_NOPTS_VALUE;
    mute = 0;
}