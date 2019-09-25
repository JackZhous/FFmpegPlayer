//
// Created by jackzhous on 2019/8/13.
//

#define MIN(a,b) (a > b ? b : a)

#ifndef MYPLAYER_CONFIG_H
#define MYPLAYER_CONFIG_H

#define NATIVE_SETUP 1

#define SET_VIDEO_DATA 2

#define FIND_AV_STREAM 3

#define PREPARE_DECODER 4

#define PREPARE_PLAYER 5

#define SEEK_REQUEST_AGAIN 6

#define ALLOC_MEMORY_FAILED 8

#define SEEK_END 7

#define STOP_VIDEO_DECODER 9

#define AV_PACKET_QUEUE_ERROR 10

#define MSG_CURRENT_POSITON             0x300   // 当前时钟

#define START_VIDEO_DECODER 11

#define START_AUDIO_DECODER 12

#define MIN_FRAMES 25

#define AV_NOSYNC_THRESHOLD 10.0

#define SAMPLE_CORRECTION_PERCENT_MAX 10

#define FFMPEG_INIT_FAILED -1

#define EXIT_PLAYER 5

#define PLAYER_OK 1
#define PLAYER_FAILED 0

#define AV_NOSYNC_THRESHOLD 10.0


#define KEEP_WAIT 0
#define STOP_WAIT 1

#define MAX_QUEUE_SIZE 15 * 1024 * 1024

//将s加一个双引号转成字符串
#define STRINGS(s) #s


#define AUDIO_MIN_BUFFER_SIZE 512

#define AUDIO_MAX_CALLBACKS_PER_SEC 30

#define AUDIO_DIFF_AVG_NB   20

#endif //MYPLAYER_CONFIG_H