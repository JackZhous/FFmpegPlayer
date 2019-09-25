//
// Created by jackzhous on 2019/8/15.
//

#ifndef MYPLAYER_VIDEODECODER_H
#define MYPLAYER_VIDEODECODER_H

#include <common/FrameQueue.h>
#include "MediaDecoder.h"
#include <sync/MediaClock.h>

class VideoDecoder : public MediaDecoder{

    public:
        VideoDecoder(AVCodecContext* codecCtx, AVStream* stream, int streamIndex, PlayerStatus* status, AVFormatContext* Ctx);
        ~VideoDecoder();
        void run() override ;
        virtual void flush();
        int getFrameSize();
        FrameQueue* getFrameQueue();
        void setMasterClock(MediaClock* clock);

    private:
        void decode();

    private:
        FrameQueue* frameQueue;
        MediaClock* masterClock;
        AVFormatContext* pForamtCtx;
};

#endif //MYPLAYER_VIDEODECODER_H
