//
// Created by jackzhous on 2019/8/16.
//
//FrameQueue 存放解码后的视频帧
#ifndef MYPLAYER_FRAMEQUEUE_H
#define MYPLAYER_FRAMEQUEUE_H

#include <Mutex.h>
#include "Config.h"
#include "Thread.h"

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
};

#define MAX 10

/**
 * 封装的数据
 */
typedef struct {
    AVFrame* frame;
    int frameWidth;
    int frameHeight;
    double pts;
    double duration;
    AVSubtitle sub;
    int format;
    int upload;
}JFrame;

class FrameQueue{
public:
    FrameQueue(int max);
    ~FrameQueue();
    void flush();
    int getFrameLen() const ;
    void setAbort(short abort);
    JFrame* getpushFrame();            //返回地址让外部填入
    JFrame* getPopFrame();             //获取弹出地址
    void push();                    //pushFrame后调用该方法让栈加1
    void pop();                     //出栈，这里为什么要这么设计？ 1. 考虑AVFrame是一副二维图形，故直接地址引用效率比较高 2. 直接返回地址给外部使用，确认外部使用完后才能往这个地址继续写入内容，否则还没使用完就覆盖会出错
    JFrame* getLastFrame();
    JFrame* getCurrentFrame();
    JFrame* getNextFrame();

private:
    int queueMax;                   //总容量
    int inputIndex,outputIndex;     //进出序号
    int size;                       //当前size
    JFrame frame[MAX];
    short abort;
    Mutex mMutex;
    Condition mCond;
};

#endif //MYPLAYER_FRAMEQUEUE_H
