package com.jz.myplayer;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
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
public class PlayerActivity extends AppCompatActivity implements View.OnClickListener,
                                                                SurfaceHolder.Callback {

    private SurfaceView surfaceView;
    private TextView display;
    private JMediaPlayer player;
    private String url = "rtmp://58.200.131.2:1935/livetv/hunantv";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_player);

        init();
    }


    private void init(){
        surfaceView = findViewById(R.id.surfaceView);
        display = findViewById(R.id.display);
        player = new JMediaPlayer();
        findViewById(R.id.start).setOnClickListener(this);
        player.setVideoUrl(url);
        player.setListener(new JMediaPlayer.PreparedListener() {
            @Override
            public void onPrepared() {
                JLog.I("java received prepared");
            }
        });
    }

    @Override
    public void onClick(View v) {
        player.prepareAsyn();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        if(player != null){
          //  player.setSurface(surfaceView.getHolder().getSurface());
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }
}
