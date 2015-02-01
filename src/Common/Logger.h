//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __TIZEN_COMMON_LOGGER_H__
#define __TIZEN_COMMON_LOGGER_H__

#include <dlog.h>
#include <sstream>

#undef LOG_TAG
#define LOG_TAG "WRT_PLUGINS/TIZEN"

#define _LOGGER(prio, fmt, args...) \
    do { \
        std::ostringstream platformLog; \
        platformLog << "%s: %s(%d) > "; \
        platformLog << fmt; \
        print_log(prio, LOG_TAG, platformLog.str().c_str(), __MODULE__, __func__, __LINE__, ##args); \
    } while(0)

#define LoggerD(fmt, args...)   _LOGGER(DLOG_DEBUG, fmt, ##args)

#define LoggerI(fmt, args...)   _LOGGER(DLOG_INFO, fmt, ##args)
#define LoggerW(fmt, args...)   _LOGGER(DLOG_WARN, fmt, ##args)
#define LoggerE(fmt, args...)   _LOGGER(DLOG_ERROR, fmt, ##args)



#define _SLOGGER(prio, fmt, args...) \
    do { \
        std::ostringstream platformLog; \
        platformLog << "%s: %s(%d) > [SECURE_LOG] "; \
        platformLog << fmt; \
        print_log(prio, LOG_TAG, platformLog.str().c_str(), __MODULE__, __func__, __LINE__, ##args); \
    } while(0)

#define SLoggerD(fmt, args...)  _SLOGGER(DLOG_DEBUG, fmt, ##args)
#define SLoggerI(fmt, args...)  _SLOGGER(DLOG_INFO, fmt, ##args)
#define SLoggerE(fmt, args...)  _SLOGGER(DLOG_ERROR, fmt, ##args)

#endif // __TIZEN_COMMON_LOGGER_H__
