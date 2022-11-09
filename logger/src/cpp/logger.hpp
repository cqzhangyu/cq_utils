#pragma once

#include <cmath>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <mutex>

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

class Logger {
private:
    std::recursive_mutex mtx;
public:
    std::ofstream of;
    enum Level {
        NO,
        ERRO,
        WARN,
        INFO,
        DEBUG,
    };

#ifndef LOG_LEVEL_DEFAULT
#define LOG_LEVEL_DEFAULT INFO
#endif

    uint32_t log_mask;
    Level log_level;

    Logger() {
        log_level = LOG_LEVEL_DEFAULT;
        log_mask = -1;
    }
    ~Logger() {
        if (of.is_open())
            of.close();
    }

    void open(const std::string &fname) {
        std::unique_lock<std::recursive_mutex> lck(mtx);

        of.open(fname, std::ios::out);
        if (of.fail()) {
            std::cout << "Failed to open " << fname << " as log file." << std::endl;
        }
    }

    void set_level(const std::string &level) {
        std::unique_lock<std::recursive_mutex> lck(mtx);

        if (level == "NO")
            log_level = NO;
        else if (level == "ERRO")
            log_level = ERRO;
        else if (level == "WARN")
            log_level = WARN;
        else if (level == "INFO")
            log_level = INFO;
        else if (level == "DEBUG")
            log_level = DEBUG;
        else
            std::cout << "Unknown log level : " << level << "\n";
    }

    void flush() {
        std::unique_lock<std::recursive_mutex> lck(mtx);

        std::cout.flush();
        of.flush();
    }

    Logger & operator << (const double &a) {
        std::unique_lock<std::recursive_mutex> lck(mtx);

        std::cout << a;
        if (fabs(a) < 0.001) {
            if (fabs(a) < 1e-9)
                of << 0;
            else
                of << std::fixed << std::showpoint << std::setprecision(6) << a;
        }
        else
            of << a;
        return *this;
    }

    template<typename T>
    Logger & operator << (const T &a) {
        std::unique_lock<std::recursive_mutex> lck(mtx);

        std::cout << a;
        of << a;
        return *this;
    }

    Logger & operator << (std::ostream& (*fp)(std::ostream&)) {
        std::unique_lock<std::recursive_mutex> lck(mtx);

        std::cout << fp;
        of << fp;
        return *this;
    }
    
    template<typename T>
    Logger & operator () (const T &arg1) {
        std::unique_lock<std::recursive_mutex> lck(mtx);

        return (*this) << arg1 << "\n";
    }

    
    template<typename T, typename... Args>
    Logger & operator () (const T &arg1, const Args &...args) {
        std::unique_lock<std::recursive_mutex> lck(mtx);
        
        return ((*this) << arg1)(args...);
    }

    template<typename... Args>
    Logger & info(const Args &...args) {
        if (INFO <= log_level && ((log_mask >> INFO) & 1))
            return (*this)("[" CO_GREEN "INFO" CO_RESET "] ", args...);
        return *this;
    }
    
    template<typename... Args>
    Logger & warn(const Args &...args) {
        if (WARN <= log_level && ((log_mask >> WARN) & 1))
            return (*this)("[" CO_YELLOW "WARN" CO_RESET "] ", args...);
        return *this;
    }
    
    template<typename... Args>
    Logger & erro(const Args &...args) {
        if (ERRO <= log_level && ((log_mask >> ERRO) & 1))
            return (*this)("[" CO_RED "ERRO" CO_RESET "] ", args...);
        return *this;
    }

    template<typename... Args>
    Logger & debug(const Args &...args) {
        if (DEBUG <= log_level && ((log_mask >> DEBUG) & 1))
            return (*this)("[" CO_PURPLE "DEBUG" CO_RESET "] ", args...);
        return *this;
    }
};
