/** @file nerecord_base.h
  * @brief 定义导出宏。该文件只定义导出宏，不定义其他任何东西！
  * @copyright (c) 2015-2019, NetEase Inc. All rights reserved
  * @author Harrison
  * @date 2018/11/23
  */

#ifndef NERECORD_BASE_H
#define NERECORD_BASE_H

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#define NERECORD_CALL __cdecl
#if defined(NERTC_SDK_EXPORT)
#define NERECORD_API extern "C" __declspec(dllexport)
#else
#define NERECORD_API extern "C" __declspec(dllimport)
#endif
#elif defined(__APPLE__)
#define NERECORD_API __attribute__((visibility("default"))) extern "C"
#define NERECORD_CALL
#elif defined(__ANDROID__) || defined(__linux__)
#define NERECORD_API extern "C" __attribute__((visibility("default")))
#define NERECORD_CALL
#else
#define NERECORD_API extern "C"
#define NERECORD_CALL
#endif

#endif