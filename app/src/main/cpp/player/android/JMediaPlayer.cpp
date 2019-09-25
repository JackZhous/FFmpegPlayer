//
// Created by jackzhous on 2019/8/9.
//

#include "JMediaPlayer.h"

JMediaPlayer::JMediaPlayer() {
    listener = NULL;
    player = new Player();
    abort = false;
    seeking = false;
}

JMediaPlayer::~JMediaPlayer() {

}


void JMediaPlayer::setSurface(ANativeWindow *window) {
    if(player != NULL){
        player->setSurface(window);
    }
}


void JMediaPlayer::setJNIListener(JNIListener *listener) {
    this->listener = listener;
    Thread* thread = new Thread(this);
    thread->start();
}

void JMediaPlayer::notifyJava(int what) {
    if(listener != NULL){
        listener->notify(what);
    }
}


void JMediaPlayer::setVideoUrl(char* url) {
    player->setVideoUrl(url);
}

void JMediaPlayer::setVideoPath(char *path) {
    player->setVideoPath(path);
}

void JMediaPlayer::run() {
    int ret;
    for(;;){
        if(abort){
            LOGI("handle message abort!");
            break;
        }

        JNIMessage msg;
        ret = player->getMessageQueue()->getMessage(&msg);
        if(ret < 0){
            LOGE("get handle message failed!");
        }
        LOGI("progress %s", msg.message);
        switch (msg.what){
            case SEEK_END:
                seeking = false;
                break;
        }
        notifyJava(msg.what);
        freeJNIMessage(&msg);
    }

}


void JMediaPlayer::freeJNIMessage(JNIMessage *msg) {
    if(msg == NULL){
        return;
    }
    if(msg->message != NULL){
        av_free(msg->message);
    }
}

void JMediaPlayer::prepareAsyn() {
    if(player != NULL){
        player->prepare();
    }
}

int JMediaPlayer::getHeight() {
    if(player != NULL){
        player->getHeight();
    }
    return 0;
}

int JMediaPlayer::getWidth() {
    if(player != NULL){
        player->getWidth();
    }
    return 0;
}

void JMediaPlayer::seekVideo(float time) {
    if(player != NULL){
        if(seeking){
            return;
        } else{
            player->seekVideo(time);
            seeking = true;
        }
    }
}

void JMediaPlayer::setLoop(int loop) {
    if(player != NULL){
        player->setLoop(loop);
    }
}
