package com.jz.myplayer;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;


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
        switch (v.getId()){

            case R.id.local_play:

                break;


            case R.id.remote_play:
                startActivity(new Intent(this, PlayerActivity.class));
                break;
        }
    }
}
