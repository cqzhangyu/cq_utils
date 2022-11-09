#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CO_RED                "\033[1;31m"
#define CO_BLACK              "\033[1;30m"
#define CO_RED                "\033[1;31m"
#define CO_GREEN              "\033[1;32m"
#define CO_YELLOW             "\033[1;33m"
#define CO_BLUE               "\033[1;34m"
#define CO_PURPLE             "\033[1;35m"
#define CO_SKYBLUE            "\033[1;36m"
#define CO_WHITE              "\033[1;37m"
#define CO_RESET              "\033[0m"
#define BG_BLACK              "\033[40m"
#define BG_RED                "\033[41m"
#define BG_GREEN              "\033[42m"
#define BG_YELLOW             "\033[43m"
#define BG_BLUE               "\033[44m"
#define BG_PURPLE             "\033[45m"
#define BG_SKYBLUE            "\033[46m"
#define BG_WHITE              "\033[47m"
#define BG_RESET              "\033[0m"

#define LOG_LEVEL_ERRO    1
#define LOG_LEVEL_WARN    2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_DEBUG   4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_ERRO
#endif

#ifndef LOG_LEVEL_MASK
#define LOG_LEVEL_MASK -1
#endif

int log_open(const char *fname);

int set_log_level(int level);

int set_log_level_str(const char *str);

int set_log_level_mask(int mask);

int lprintf(const char *fmt, ...);

int log_print(int level, const char *fmt, ...);

void log_close();

#define __LOG_ANY(format, ...) 
#define LOG_ERRO(format, ...)  log_print(LOG_LEVEL_ERRO, "[" CO_RED "ERRO" CO_RESET "] " format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  log_print(LOG_LEVEL_WARN, "[" CO_YELLOW "WARN" CO_RESET "] " format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  log_print(LOG_LEVEL_INFO, "[" CO_GREEN "INFO" CO_RESET "] " format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) log_print(LOG_LEVEL_DEBUG, "[" CO_PURPLE "DEBUG" CO_RESET "] " format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
