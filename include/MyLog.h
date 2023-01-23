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

bool b_debug_logs = false;

void logDebug(LogLevel iLevel, const char *zFormat, ...);
void EnableLogging();
void DisableLogging();

void logDebug(LogLevel iLevel, const char *zFormat, ...) {
    const char *z_prefix = nullptr;
    switch (iLevel) {
        case LogLevel::DEBUG:
            if (!b_debug_logs) {
                return;
            }
            z_prefix = "DEBUG  ";
            break;
        case LogLevel::INFO:
            z_prefix = "INFO   ";
            break;
        case LogLevel::WARNING:
            z_prefix = "WARNING";
            break;
        case LogLevel::ERROR:
            z_prefix = "ERROR  ";
            break;

        default:
            break;
    }

    va_list args;
    va_start(args, zFormat);
    char z_log_line[MAX_LOG_SIZE];
    vsnprintf(z_log_line, MAX_LOG_SIZE, zFormat, args);
    printf("|%s| %s\n", z_prefix, z_log_line);
    va_end(args);
}

void EnableLogging() {
    b_debug_logs = true;
    LOG_DEBUG("Debug logs enabled");
}

void DisableLogging() {
    LOG_DEBUG("Debug logs disabled");
    b_debug_logs = false;
}

#endif //MYCPPIMPLEMENT_MYLOG_H
