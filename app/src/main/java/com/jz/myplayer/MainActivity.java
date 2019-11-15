package com.jz.myplayer;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ListView;
import android.widget.TextView;

import com.cgfay.medialibrary.engine.MediaScanEngine;
import com.cgfay.medialibrary.listener.OnMediaSelectedListener;
import com.cgfay.medialibrary.loader.impl.GlideMediaLoader;
import com.cgfay.medialibrary.model.MimeType;

import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.TreeMap;
import java.util.TreeSet;


public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    // Used to load the 'native-lib' library on application startup.


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.remote_play).setOnClickListener(this);
        findViewById(R.id.local_play).setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        Intent intent = new Intent(this, PlayerActivity.class);
        String url = "";
        switch (v.getId()){

            case R.id.local_play:
//                url = "/storage/emulated/0/tencent/MicroMsg/WeiXin/wx_camera_1571389049938.mp4";
                scanMedia(false, false, true);
                break;


            case R.id.remote_play:
                url = "rtmp://202.69.69.180:443/webcast/bshdlive-pc";
                intent.putExtra("url", url);
                startActivity(intent);
                break;
        }

    }

    private void scanMedia(boolean enableGif, boolean enableImage, boolean enableVideo) {
        MediaScanEngine.from(this)
                .setMimeTypes(MimeType.ofAll())
                .ImageLoader(new GlideMediaLoader())
                .spanCount(4)
                .showCapture(false)
                .showImage(enableImage)
                .showVideo(enableVideo)
                .enableSelectGif(enableGif)
                .setMediaSelectedListener(new OnMediaSelectedListener() {
                    @Override
                    public void onSelected(List<Uri> uriList, List<String> pathList, boolean isVideo) {
                        if (isVideo) {
                            Intent intent = new Intent(MainActivity.this, PlayerActivity.class);
                            intent.putExtra("url", pathList.get(0));
                            startActivity(intent);
                        }
                    }
                })
                .scanMedia();
    }

}