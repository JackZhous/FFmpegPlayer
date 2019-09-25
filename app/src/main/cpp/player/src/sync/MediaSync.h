//
// Created by jackzhous on 2019/8/26.
//

#ifndef MYPLAYER_MEDIASYNC_H
#define MYPLAYER_MEDIASYNC_H

#include <common/PlayerStatus.h>
#include "Thread.h"
#include "MediaClock.h"

class MediaSync : public Runnable{
    public:
        MediaSync();
        ~MediaSync();

        void updateExternalClock(double pts);

        virtual void run() override ;

        void stop();

        void updateAudioColock(double pts, double time);

        //获取音频时钟漂移
        double getAudioDffClock();

        double getMasterClock();


private:
    Mutex mMutex;
    Condition mCond;
    bool abort;

    MediaClock *audioClock;
    MediaClock *videoClock;
    MediaClock *extClock;


    PlayerStatus* playerStatus;
};

#endif //MYPLAYER_MEDIASYNC_H
