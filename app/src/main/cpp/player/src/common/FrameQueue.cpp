//
// Created by jackzhous on 2019/8/16.
//

#include <AndroidLog.h>
#include "FrameQueue.h"

FrameQueue::FrameQueue(int max) {
    queueMax = MIN(max, MAX);
    size = 0;
    for(int i = 0; i < queueMax; i++){
        frame[i].frame = av_frame_alloc();
    }
    abort = 1;
    inputIndex = 0;
    outputIndex = 0;
    size = 0;
}

FrameQueue::~FrameQueue() {
    for(int i = 0; i < queueMax; i++){
        av_frame_unref(frame[i].frame);
//        avsubtitle_free(&(frame[i].sub));
    }
}

void FrameQueue::flush() {
    mMutex.lock();
    while (size > 0){
        av_frame_unref(frame[outputIndex].frame);
//        avsubtitle_free(&(frame[outputIndex].sub));
        size--;
        outputIndex++;
        if(outputIndex >= queueMax){
            outputIndex = outputIndex % queueMax;
        }
    }
    mCond.signal();
    mMutex.unlock();
}



int FrameQueue::getFrameLen() const {
    return size;
}

JFrame* FrameQueue::getCurrentFrame() {
    int index = outputIndex % queueMax;
    return &frame[index];
}


/**
 * 有待商榷，因为第一次调用时可能出错
 */
JFrame* FrameQueue::getLastFrame() {
//    LOGI("出兑index %d", outputIndex-1);
    return &frame[outputIndex-1];
}

JFrame* FrameQueue::getNextFrame() {
    return &frame[(outputIndex + 1) % queueMax];
}

void FrameQueue::setAbort(short abort) {
    this->abort = abort;
}

JFrame* FrameQueue::getpushFrame() {
    mMutex.lock();
    JFrame* pFrame;
    while (size >= queueMax || !abort){
        mCond.wait(mMutex);
    }
//    LOGI("getPushFrame %d", inputIndex % queueMax);
    pFrame = &frame[inputIndex % queueMax];
    mCond.signal();
    mMutex.unlock();

    return pFrame;
}

void FrameQueue::push() {
    mMutex.lock();
    size++;
    inputIndex++;
    if(inputIndex >= queueMax){
        inputIndex = inputIndex % queueMax;
    }
//    LOGI("push入队size  %d", size);
    mCond.signal();
    mMutex.unlock();
}

JFrame* FrameQueue::getPopFrame() {
    mMutex.lock();
    JFrame* pFrame;

    pFrame = &frame[outputIndex % queueMax];
    mCond.signal();
    mMutex.unlock();

    return pFrame;
}

/**
 * 出队列 先调pop 在调用getLastFrame
 */
void FrameQueue::pop() {
    mMutex.lock();
    while (size <= 0 || !abort){
        mCond.wait(mMutex);
    }
//    LOGI("pop size %d", size);
    outputIndex++;
    if(size < 0){
        size = 0;
    }
    if(outputIndex > queueMax){
        outputIndex = outputIndex % queueMax;
    }
    mCond.signal();
    mMutex.unlock();
}


void FrameQueue::unrefFrame(JFrame *frame) {
    if(frame && frame->upload){
//        LOGI("used");
        av_frame_unref(frame->frame);
//        avsubtitle_free(&frame->sub);
        size--;
    }
}