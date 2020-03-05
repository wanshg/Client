//
// Created by wanshaojun on 2020/3/5.
//

#ifndef CPP_WLEGLHELPER_H
#define CPP_WLEGLHELPER_H

#include "EGL/egl.h"
#include "../Log/WlAndroidLog.h"

class WlEglHelper {
public:
    WlEglHelper();
    ~WlEglHelper();
    int initEgl();

    int initEgl(EGLNativeWindowType window);//环境搭建初始化

    int swapBuffers();//交换缓冲 绘制

    void destroyEgl();//销毁



    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    //EGLConfig mEglConfig;
    EGLContext mEglContext;
};


#endif //CPP_WLEGLHELPER_H
