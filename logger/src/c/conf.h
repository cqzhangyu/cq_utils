#ifndef __CONF_H__
#define __CONF_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>

void parse_conf(int argc, char **argv);

struct conf_t {
    int size;
    int epoch;
};

extern struct conf_t conf;

#endif
