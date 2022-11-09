#pragma once

#include "cmdline.h"
#include "logger.hpp"

#include <string>
#include <unistd.h>


class Config {
public:
    Logger logger;
    std::string log_fname;
    std::string log_level;
    
    void parse(int argc, char **argv) {
        cmdline::parser parser;

        parser.add<std::string>("log_fname", 0, "log file name", false, "");
        parser.add<std::string>("log_level", 0, "log level", false, "INFO");

        parser.parse_check(argc, argv);

        log_fname = parser.get<std::string>("log_fname");
        log_level = parser.get<std::string>("log_level");

        if (log_fname != "")
            logger.open(log_fname);
        logger.set_level(log_level);
    }
};
