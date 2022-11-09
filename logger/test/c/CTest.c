#include "conf.h"
#include "log.h"

int main(int argc, char **argv) {
	parse_conf(argc, argv);
	
	LOG_INFO("size = %d\n", conf.size);
	LOG_INFO("Finish.\n");
	return 0;
}
