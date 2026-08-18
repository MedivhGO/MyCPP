//
// Created by Lee on 2022/9/6.
//

#ifndef MYCPPIMPLEMENT_MYLOG_H
#define MYCPPIMPLEMENT_MYLOG_H

#include <cstdio>
#include <cstdarg>

#define LOG_DEBUG(fmt, args...)     logDebug(LogLevel::DEBUG, fmt, ##args)
#define LOG_INFO(fmt, args...)      logDebug(LogLevel::INFO, fmt, ##args)
#define LOG_WARN(fmt, args...)      logDebug(LogLevel::WARNING, fmt, ##args)
#define LOG_ERROR(fmt, args...)     logDebug(LogLevel::ERROR, fmt, ##args)

#define MAX_LOG_SIZE        500

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

extern bool b_debug_logs;

void logDebug(LogLevel iLevel, const char *zFormat, ...);
void EnableLogging();
void DisableLogging();

#endif //MYCPPIMPLEMENT_MYLOG_H
