//
// Created by jackzhous on 2019/8/14.
//

#ifndef MYPLAYER_FFMPEGUTIL_H
#define MYPLAYER_FFMPEGUTIL_H


#include <AndroidLog.h>
#include <string.h>

extern "C"{
#include <libavutil/error.h>
#include <libavformat/avformat.h>
};


void printError(const char* name, int error);

bool isLiveStream(AVFormatContext *s);

#endif //MYPLAYER_FFMPEGUTIL_H
