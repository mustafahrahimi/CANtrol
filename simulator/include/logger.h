#pragma once 

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

void logger_init(LogLevel level);
void logger_log(LogLevel level, const char* fmt, ...);
void logger_set_level(LogLevel level);