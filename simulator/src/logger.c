#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

static LogLevel current_log_level = LOG_INFO;

void logger_init(LogLevel level) {
    current_log_level = level;
}

void logger_set_level(LogLevel level) {
    current_log_level = level;
}

void logger_log(LogLevel level, const char* fmt, ...) {
    if (level < current_log_level) return;

    const char* prefix = "";
    switch (level) {
        case LOG_DEBUG: prefix = "[DEBUG]"; break;
        case LOG_INFO:  prefix = "[INFO ]"; break;
        case LOG_WARN:  prefix = "[WARN ]"; break;
        case LOG_ERROR: prefix = "[ERROR]"; break;
    }

    va_list args;
    va_start(args, fmt);
    printf("%s ", prefix);
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}