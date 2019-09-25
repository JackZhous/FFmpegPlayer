//
// Created by jackzhous on 2019/8/15.
//

#include "FFmpegUtil.h"

/**
 * 打印错误日志
 */
void printError(const char* name, int error) {
    char buf[100];
    const char* pBuf = buf;
    if(av_strerror(error, buf, sizeof(buf)) < 0){
        pBuf = strerror(AVUNERROR(error));
    }
    LOGE("%s : %s", name, pBuf);
}

bool isLiveStream(AVFormatContext *s){
    const char* name = s->iformat->name;
    if(!strcmp(name, "rtp") || !strcmp(name, "rtsp") || !strcmp(name, "sdp")){
        return true;
    }

    if (s->pb && (!strncmp(s->filename, "rtp:", 4) || !strncmp(s->filename, "udp:", 4))) {
        return true;
    }
    return false;

}