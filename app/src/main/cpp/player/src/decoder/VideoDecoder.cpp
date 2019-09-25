//
// Created by jackzhous on 2019/8/15.
//

#include <common/FrameQueue.h>
#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(AVCodecContext *codecCtx, AVStream *stream, int streamIndex,
                           PlayerStatus *status, AVFormatContext* pForamtCtx) : MediaDecoder(codecCtx, stream, streamIndex, status){
    masterClock = NULL;
    frameQueue = new FrameQueue(8);
    this->pForamtCtx = pForamtCtx;
}

VideoDecoder::~VideoDecoder() {
    if(masterClock){
        delete masterClock;
        masterClock = NULL;
    }

    if(frameQueue){
        delete frameQueue;
        frameQueue = NULL;
    }
}

void VideoDecoder::run() {
    decode();
}

void VideoDecoder::flush() {
    mMutex.lock();
    MediaDecoder::flush();
    if(frameQueue){
        frameQueue->flush();
    }
    mCond.signal();
    mMutex.unlock();
}


int VideoDecoder::getFrameSize() {
    return frameQueue? frameQueue->getFrameLen() : 0;
}

FrameQueue* VideoDecoder::getFrameQueue() {
    return frameQueue;
}

void VideoDecoder::decode() {
    int gotPicture, ret;
    AVFrame* frame = av_frame_alloc();
    if(!frame){
        playerStatus->queue->addMessage(ALLOC_MEMORY_FAILED, "alloc av frame failed");
        mCond.signal();
        return;
    }

    AVPacket* packet = av_packet_alloc();
    if(!packet){
        playerStatus->queue->addMessage(ALLOC_MEMORY_FAILED, "alloc av packet failed");
        mCond.signal();
        return;
    }
    JFrame* jFrame;

    AVRational timebase = stream->time_base;
    AVRational frameRate = av_guess_frame_rate(pForamtCtx, stream, NULL);
    while (abort){
        if(playerStatus->seekRequest){
            continue;
        }

        if(playerStatus->abortRequest){
            playerStatus->queue->addMessage(STOP_VIDEO_DECODER, "stop video decoder");
            break;
        }

        if(queue->getPacketLen() < 0){
            playerStatus->queue->addMessage(AV_PACKET_QUEUE_ERROR, "av packet queue below 0");
            break;
        }
        ret = queue->pullPacket(packet);
        if(ret == PLAYER_FAILED){
            LOGE("get packet failed");
            break;
        }

        if(avcodec_send_packet(pCodecCtx, packet) < 0){
            av_packet_unref(packet);
            LOGE("send video packet to codec failed");
            continue;
        }

        if(avcodec_receive_frame(pCodecCtx, frame) < 0){
            av_frame_unref(frame);
            av_packet_unref(packet);
            LOGE("receive video frame to codec failed");
            continue;
        } else{
            gotPicture = 1;

            //是否需要重排
            if(playerStatus->reorderVideoPts == -1){
                frame->pts = av_frame_get_best_effort_timestamp(frame);
            } else{
                frame->pts = frame->pkt_dts;
            }
            //宽高比
            frame->sample_aspect_ratio = av_guess_sample_aspect_ratio(pForamtCtx, stream, frame);
            if(masterClock != NULL){
                double pts = NAN;
                if(frame->pts != AV_NOPTS_VALUE){
                    pts = av_q2d(timebase) * pts;
                }

                //检查此帧数据是否被丢弃
                if(playerStatus->syncType != AV_SYNC_VIDEO){
                    if(frame->pts != AV_NOPTS_VALUE){
                        double diff = pts - masterClock->getClock();
                        //拖拽视频时判断
                        if(!isnan(diff) && diff < 0 && fabs(diff) < AV_NOSYNC_THRESHOLD && queue->getPacketLen() > 0){
                            gotPicture = 0;
                            av_frame_unref(frame);
                        }
                    }
                }
            }

        }
        if(gotPicture){
            jFrame = frameQueue->getpushFrame();

            jFrame->pts = frame->pts == AV_NOPTS_VALUE ? 0 : av_q2d(timebase) * frame->pts;
            jFrame->frameWidth = frame->width;
            jFrame->frameHeight = frame->height;
            jFrame->format = frame->format;
            jFrame->upload = 0;
            jFrame->duration = frameRate.num && frameRate.den
                           ? av_q2d((AVRational){frameRate.den, frameRate.num}) : 0;
            av_frame_move_ref(jFrame->frame, frame);

            frameQueue->push();
        }

        // 释放数据包和缓冲帧的引用，防止内存泄漏
        av_frame_unref(frame);
        av_packet_unref(packet);
    }

    av_frame_free(&frame);
    av_free(frame);
    frame = NULL;

    av_packet_free(&packet);
    av_free(packet);
    packet = NULL;

    mCond.signal();
}

void VideoDecoder::setMasterClock(MediaClock* clock) {
    AutoMutex lock(mMutex);
    this->masterClock = clock;
}