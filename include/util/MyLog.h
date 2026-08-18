//
// Created by Lee on 2022/9/6.
//

#ifndef MYCPPIMPLEMENT_MYLOG_H
#define MYCPPIMPLEMENT_MYLOG_H

#include <cstdarg>
#include <cstdint>
#include <cstdio>

#define LOG_DEBUG(fmt, args...) logDebug(LogLevel::DEBUG, fmt, ##args)
#define LOG_INFO(fmt, args...) logDebug(LogLevel::INFO, fmt, ##args)
#define LOG_WARN(fmt, args...) logDebug(LogLevel::WARNING, fmt, ##args)
#define LOG_ERROR(fmt, args...) logDebug(LogLevel::ERROR, fmt, ##args)

enum : std::uint16_t { MAX_LOG_SIZE = 500 };

enum LogLevel : std::uint8_t { DEBUG, INFO, WARNING, ERROR };

extern bool b_debug_logs;

void logDebug(LogLevel iLevel, const char *zFormat, ...);
void EnableLogging();
void DisableLogging();

#endif  // MYCPPIMPLEMENT_MYLOG_H
