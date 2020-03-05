//
// Created by wanshaojun on 2020/3/5.
//

#include "WlEglThread.h"

WlEglThread::WlEglThread() {
    pthread_mutex_init(&pthread_mutex, NULL);
    pthread_cond_init(&pthread_cond, NULL);
}

WlEglThread::~WlEglThread() {
    pthread_mutex_destroy(&pthread_mutex);
    pthread_cond_destroy(&pthread_cond);
}


void *eglThreadImpl(void *context) {
    WlEglThread *eglThread = static_cast<WlEglThread *>(context);
    if (!eglThread) {
        LOGE("eglThreadImpl eglThread is null");
        return 0;
    }

    WlEglHelper *eglHelper = new WlEglHelper();

    if (eglHelper->initEgl(eglThread->mANativeWindow) != 0) {
        LOGE("eglHelper initEgl error");
        return 0;
    }
    eglThread->isExit = false;
    while (!eglThread->isExit) {

        if (eglThread->isCreate) {
            eglThread->isCreate = false;
            eglThread->onCreate();
        }

        if (eglThread->isChange) {
            eglThread->isChange = false;
            eglThread->isStart = true;
            eglThread->onChange(eglThread->surfaceWidth, eglThread->surfaceHeight);
        }

        if (eglThread->isStart) {
            eglThread->onDraw();
            //切换缓冲区，显示
            eglHelper->swapBuffers();

            if (eglThread->mRenderType == RENDER_MODULE_AUTO) {
                usleep(1000000 / 60);
            } else {
                pthread_mutex_lock(&eglThread->pthread_mutex);
                pthread_cond_wait(&eglThread->pthread_cond, &eglThread->pthread_mutex);
                pthread_mutex_unlock(&eglThread->pthread_mutex);
            }
        }

    }

    eglHelper->destroyEgl();
    delete eglHelper;
    eglHelper = NULL;
    //return 0表示线程结束
    return 0;

}


void WlEglThread::onSurfaceCreate(EGLNativeWindowType window) {
    if (mEglThread == -1) {
        isCreate = true;
        mANativeWindow = window;
        pthread_create(&mEglThread, NULL, eglThreadImpl, this);
    }
}


void WlEglThread::onSurfaceChange(int width, int height) {
    if (mEglThread != -1) {
        surfaceWidth = width;
        surfaceHeight = height;
        isChange = true;

        notifyRender();
    }
}

void WlEglThread::setRenderModule(int renderType) {
    mRenderType = renderType;
    notifyRender();
}

void WlEglThread::notifyRender() {
    pthread_mutex_lock(&pthread_mutex);
    pthread_cond_signal(&pthread_cond);
    pthread_mutex_unlock(&pthread_mutex);
}


void WlEglThread::callBackOnCreate(WlEglThread::OnCreate onCreate) {
    this->onCreate = onCreate;
}

void WlEglThread::callBackOnChange(WlEglThread::OnChange onChange) {
    this->onChange = onChange;
}

void WlEglThread::callBackOnDraw(WlEglThread::OnDraw onDraw) {
    this->onDraw = onDraw;
}


