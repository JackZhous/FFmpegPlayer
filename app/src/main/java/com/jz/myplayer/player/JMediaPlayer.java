package com.jz.myplayer.player;


import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.Surface;

import com.jz.myplayer.JLog;

import java.lang.ref.WeakReference;

/**
 * @author jackzhous
 * @package com.jz.myplayer.player
 * @filename JMediaPlayer
 * date on 2019/8/9 9:23 AM
 * @describe TODO
 * @email jackzhouyu@foxmail.com
 **/
public class JMediaPlayer {

    private NativeHelper helper;
    protected Handler handler;
    private PreparedListener listener;

    public JMediaPlayer() {
        this.helper = NativeHelper.Holder.getNativeHelper();
        handler = new MyHandler(Looper.getMainLooper());
        //开启消息线程读取,以及初始化一些类
        helper.nativeSetConnect(this, new WeakReference<>(this));
    }


    public static void handleNativeMsg(int what, Object value){
        JMediaPlayer player = ((WeakReference<JMediaPlayer>)value).get();
        if(player == null){
            return;
        }
        Message msg = Message.obtain();
        msg.what = what;
        player.handler.sendMessage(msg);
    }

    public NativeHelper getHelper() {
        return helper;
    }

    public void setVideoUrl(String url){
        helper.setUrl(url);
    }

    public void prepareAsyn(){
        helper.preparePlayerAsyn();
    }

    public void setSurface(Surface surface){
        helper.setVideoSurface(surface);
    }

    public void setListener(PreparedListener listener) {
        this.listener = listener;
    }

    private class MyHandler extends Handler{
        public MyHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what){
                case 5:
                    if(listener != null){
                        listener.onPrepared();
                    }
            }
        }
    }


    public interface PreparedListener{
        void onPrepared();
    }
}
