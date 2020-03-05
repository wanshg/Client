package com.example.cpp.opengl;

import android.view.Surface;

/**
 * Created by wsj on 2019/4/22
 * Des:
 */
public class NationOpenGL {

    static {
        System.loadLibrary("native-lib");
    }

    public native void nativeSurfaceCreate(Surface surface);

    public native void nativeSurfaceChanged(int width, int height);

    public native void nativeSurfaceDestroyed();

}
