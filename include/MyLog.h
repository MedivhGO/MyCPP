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

bool b_DebugLogs = false;

void logDebug(LogLevel iLevel, const char *zFormat, ...);
void enableLogging();
void disableLogging();

void logDebug(LogLevel iLevel, const char *zFormat, ...) {
    const char *zPrefix = nullptr;
    switch (iLevel) {
        case LogLevel::DEBUG:
            if (!b_DebugLogs) {
                return;
            }
            zPrefix = "DEBUG  ";
            break;
        case LogLevel::INFO:
            zPrefix = "INFO   ";
            break;
        case LogLevel::WARNING:
            zPrefix = "WARNING";
            break;
        case LogLevel::ERROR:
            zPrefix = "ERROR  ";
            break;

        default:
            break;
    }

    va_list args;
    va_start(args, zFormat);
    char zLogLine[MAX_LOG_SIZE];
    vsnprintf(zLogLine, MAX_LOG_SIZE, zFormat, args);
    printf("|%s| %s\n", zPrefix, zLogLine);
    va_end(args);
}

void enableLogging() {
    b_DebugLogs = true;
    LOG_DEBUG("Debug logs enabled");
}

void disableLogging() {
    LOG_DEBUG("Debug logs disabled");
    b_DebugLogs = false;
}

#endif //MYCPPIMPLEMENT_MYLOG_H
