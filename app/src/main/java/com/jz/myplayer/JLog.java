package com.jz.myplayer;

import android.util.Log;

/**
 * @author jackzhous
 * @package com.jz.myplayer
 * @filename JLog
 * date on 2019/8/12 10:31 AM
 * @describe TODO
 * @email jackzhouyu@foxmail.com
 **/
public class JLog {
    private static final String TAG = "java_tag";

    public static void I(String msg){
        Log.i(TAG, msg);
    }
}
