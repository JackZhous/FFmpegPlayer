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

JFrame* FrameQueue::getCurrentFrame() {
    return &frame[outputIndex & queueMax];
}


/**
 * 有待商榷，因为第一次调用时可能出错
 */
JFrame* FrameQueue::getLastFrame() {
    int i = outputIndex - 1;
    if(i < 0){
        i = queueMax - 1;
    }
    return &frame[i];
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
    while (size > queueMax && !abort){
        mCond.wait(&mMutex);
    }
    pFrame = &frame[inputIndex % queueMax];
    mCond.signal();
    mMutex.unlock();

    return pFrame;
}

void FrameQueue::push() {
    mMutex.lock();
    size++;
    inputIndex++;
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

void FrameQueue::pop() {
    mMutex.lock();

    while (size <= 0 && !abort){
        mCond.wait(&mMutex);
    }

    size--;
    outputIndex++;
    mCond.signal();
    mMutex.unlock();
}