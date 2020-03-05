#include <jni.h>
#include <string>

#include "android/native_window.h"
#include "android/native_window_jni.h"
#include "egl/WlEglThread.cpp"
#include "GLES2/gl2.h"

//#include "Add/BaseAdd.h"
//#include "../../../../../extendCpp/Add/BaseAdd.h
#include "../../../../extendCpp/Add/BaseAdd.h"
//#include "../../../../../extendCpp/Add/BaseAdd.h"
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_cpp_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


WlEglThread *eglThread = NULL;
void callBackOnCreate() {

   BaseAdd temadd;
    int c = temadd.Add(1,2);
    LOGE("callBackOnCreate");
}

void callBackOnChange(int width, int height) {
    glViewport(0, 0, width, height);
    LOGE("callBackOnChange");
}

void callBackOnDraw() {
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    LOGE("callBackOnDraw");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cpp_opengl_NationOpenGL_nativeSurfaceCreate(JNIEnv *env, jobject instance,
                                                                      jobject surface) {

    eglThread = new WlEglThread();
    eglThread->callBackOnCreate(callBackOnCreate);
    eglThread->callBackOnChange(callBackOnChange);
    eglThread->callBackOnDraw(callBackOnDraw);
    eglThread->setRenderModule(RENDER_MODULE_MANUAL);


    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    eglThread->onSurfaceCreate(nativeWindow);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cpp_opengl_NationOpenGL_nativeSurfaceChanged(JNIEnv *env,
                                                                       jobject instance, jint width,
                                                                       jint height) {
    if (eglThread) {
        eglThread->onSurfaceChange(width, height);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_cpp_opengl_NationOpenGL_nativeSurfaceDestroyed(JNIEnv *env,
                                                                         jobject instance) {
    if (eglThread) {
        eglThread->isExit = true;
        //等待线程结束
        pthread_join(eglThread->mEglThread, NULL);
        delete (eglThread);
        eglThread = NULL;
    }

}
