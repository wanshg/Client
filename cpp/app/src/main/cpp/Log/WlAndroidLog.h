//
// Created by wanshaojun on 2020/3/5.
//

#ifndef CPP_WLANDROIDLOG_H
#define CPP_WLANDROIDLOG_H

#include "android/log.h"

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG, "test", FORMAT, ##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, "test", FORMAT, ##__VA_ARGS__);

#endif //CPP_WLANDROIDLOG_H
