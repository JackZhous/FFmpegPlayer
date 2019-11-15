package com.jz.myplayer;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.jz.myplayer.player.JMediaPlayer;



/**
 * @author jackzhous
 * @package com.jz.myplayer
 * @filename PlayerActivity
 * date on 2019/8/8 5:21 PM
 * @describe TODO
 * @email jackzhouyu@foxmail.com
 **/
public class PlayerActivity extends AppCompatActivity implements
                                                                SurfaceHolder.Callback {

    private SurfaceView surfaceView;
    private JMediaPlayer player;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);

        init();
    }


    private void init(){
        String url = getIntent().getStringExtra("url");
        player = new JMediaPlayer();
        JLog.I(url);
        surfaceView = findViewById(R.id.surfaceView);
        surfaceView.getHolder().addCallback(this);
        player.setVideoUrl(url);
        player.setListener(new JMediaPlayer.PreparedListener() {
            @Override
            public void onPrepared() {
                JLog.I("java received prepared " + player.getHelper().getRorate());
                JLog.I("jni width height " + player.getHelper().getVideoWidth() + " " + player.getHelper().getVideoHeight());
                int viewWidth  = surfaceView.getWidth();
                int viewHeight;
                if (player.getHelper().getRorate() % 180 != 0) {
                    viewHeight = viewWidth * player.getHelper().getVideoWidth() / player.getHelper().getVideoHeight();
                } else {
                    viewHeight = viewWidth * player.getHelper().getVideoHeight() / player.getHelper().getVideoWidth();
                }
                if(viewHeight > surfaceView.getHeight()){
                    viewHeight = surfaceView.getHeight();
                }
                JLog.I("java width height " + viewWidth + " " + viewHeight);
                ViewGroup.LayoutParams layoutParams = surfaceView.getLayoutParams();
                layoutParams.width = viewWidth;
                layoutParams.height = viewHeight;
                surfaceView.setLayoutParams(layoutParams);

                player.getHelper().start();
            }
        });
        player.prepareAsyn();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        if(player != null){
            player.setSurface(surfaceView.getHolder().getSurface());
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    protected void onResume() {
        super.onResume();
        player.getHelper().onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        player.getHelper().onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        player.getHelper().onStop();
        player.getHelper().release();
    }
}
