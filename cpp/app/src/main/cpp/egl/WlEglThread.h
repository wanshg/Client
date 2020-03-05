//
// Created by wanshaojun on 2020/3/5.
//

#ifndef CPP_WLEGLTHREAD_H
#define CPP_WLEGLTHREAD_H

#include <EGL/eglplatform.h>
#include "pthread.h"
#include "android/native_window.h"
#include "WlEglHelper.h"
#include <unistd.h>
#include "GLES2/gl2.h"

#define RENDER_MODULE_AUTO 1
#define RENDER_MODULE_MANUAL 2

class WlEglThread {
public:
    pthread_t mEglThread = -1;
    ANativeWindow *mANativeWindow = NULL;

    int mRenderType = RENDER_MODULE_MANUAL;

    bool isCreate = false;
    bool isChange = false;
    bool isExit = false;
    bool isStart = false;

    int surfaceWidth = 0;
    int surfaceHeight = 0;


    typedef void(*OnCreate)();
    OnCreate onCreate;

    typedef void(*OnChange)(int width, int height);
    OnChange onChange;

    typedef void(*OnDraw)();
    OnDraw onDraw;


    pthread_mutex_t pthread_mutex;
    pthread_cond_t pthread_cond;

public:
    WlEglThread();
    ~WlEglThread();

    //生命周期对应响应
    void onSurfaceCreate(EGLNativeWindowType window);

    void onSurfaceChange(int width, int height);

    //设置模式
    void setRenderModule(int renderModule);

    void notifyRender();

    //设置回调
    void callBackOnCreate(OnCreate onCreate);

    void callBackOnChange(OnChange onChange);

    void callBackOnDraw(OnDraw onDraw);


};


#endif //CPP_WLEGLTHREAD_H
