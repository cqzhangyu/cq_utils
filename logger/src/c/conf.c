#include "conf.h"
#include "log.h"

struct conf_t conf = {0};

static bool has_suffix(const char* str, const char* sfx) {
    size_t len = strlen(str);
    size_t sfx_len = strlen(sfx);

    if (len < sfx_len)
        return false;
    for (size_t i = 0; i < sfx_len; i ++)
        if (str[i + len - sfx_len] != sfx[i])
            return false;
    return true;
}

static uint64_t
convert_to_bytes(char* str) {
    uint64_t temp = atoi(str);
    if (has_suffix(str, "B") == true) {
    } else if (has_suffix(str, "K") == true) {
        temp = temp * 1024;
    } else if (has_suffix(str, "M") == true) {
        temp = temp * 1024 * 1024;
    } else if (has_suffix(str, "G") == true) {
        temp = temp * 1024 * 1024 * 1024;
    } else if (has_suffix(str, "T") == true) {
        temp = temp * 1024 * 1024 * 1024 * 1024;
    } else {
        LOG_WARN("Unrecognized suffix in %s. Supported suffix is B/K/M/G/T, default to Byte\n", str);
    }
    return temp;
}

static inline void
show_usage(char* app) {
    LOG_INFO("Usage: %s [options]\n", app);
    LOG_INFO("\n");
    LOG_INFO("Options:\n");
    LOG_INFO("      -s	--size          size of matrix\n");
    LOG_INFO("      -e	--epoch         number of epochs\n");
    LOG_INFO("      -h	--help          show this message\n");
    
    exit(0);
}

void
parse_conf(int argc, char **argv) {
    char* app = argv[0];
    
    static int lopt = 0;
    static struct option opts[] = {
        {"help", required_argument, &lopt, 1},
        {"size", required_argument, &lopt, 2},
        {"epoch", required_argument, &lopt, 3},
        {"log_fname", required_argument, &lopt, 4},
        {"log_level", required_argument, &lopt, 5},
        {0, 0, 0, 0}
    };
    
    int c, opt_index = 0;
    optind = 1; // reset getopt_long
    while ((c = getopt_long(argc, argv, "hs:e:", opts, &opt_index)) != -1) {
        switch(c) {
        case 0:
            switch(lopt) {
            case 2:
                conf.size = atoi(optarg);
                break;
            case 3:
                conf.epoch = atoi(optarg);
                break;
            case 4:
                log_open(optarg);
                break;
            case 5:
                set_log_level_str(optarg);
                break;
            case 1:
            default:
                show_usage(app);
                break;
            }
            break;
        case 's':
            conf.size = atoi(optarg);
            break;
        case 'e':
            conf.epoch = atoi(optarg);
            break;
        case 'h':
        default:
            show_usage(app);
            break;
        }
    }

    if (!conf.size)
        conf.size = 1000;
    if (!conf.epoch)
        conf.epoch = 1;
}
