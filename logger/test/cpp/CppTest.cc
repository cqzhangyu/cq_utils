#include "config.hpp"

using namespace std;

int main(int argc, char **argv) {
	Config cfg;
	cfg.parse(argc, argv);

	Logger &logger(cfg.logger);

	logger << "Finish." << endl;
	
	return 0;
}
