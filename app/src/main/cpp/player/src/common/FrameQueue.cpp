//
// Created by jackzhous on 2019/8/16.
//

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
        avsubtitle_free(&(frame[i].sub));
    }
}

void FrameQueue::flush() {
    mMutex.lock();
    while (size > 0){
        av_frame_unref(frame[outputIndex].frame);
        avsubtitle_free(&(frame[outputIndex].sub));
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


void FrameQueue::setAbort(short abort) {
    this->abort = abort;
}

JFrame* FrameQueue::getpushFrame() {
    mMutex.lock();
    JFrame* pFrame;
    while (size > queueMax){
        mCond.wait(&mMutex);
    }
    pFrame = &frame[inputIndex % queueMax];
    inputIndex++;
    mCond.signal();
    mMutex.unlock();

    return pFrame;
}

void FrameQueue::push() {
    mMutex.lock();
    size++;
    mCond.signal();
    mMutex.unlock();
}

JFrame* FrameQueue::getPopFrame() {
    mMutex.lock();
    JFrame* pFrame;
    while (size <= 0){
        mCond.wait(&mMutex);
    }
    pFrame = &frame[outputIndex % queueMax];
    outputIndex++;
    mCond.signal();
    mMutex.unlock();

    return pFrame;
}

void FrameQueue::pop() {
    mMutex.lock();
    size--;
    mCond.signal();
    mMutex.unlock();
}