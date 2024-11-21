#include "Interpreter.h"

using namespace labbish::Micrologic;

int main(int argc, const char* argv[]) {
	SetEncoding();
	Blocks blocks;
	std::string exepath = std::string(_getcwd(NULL, 0));
	Interpreter Micrologic(blocks, exepath);
	if (argc == 2) {
		Micrologic.command("open " + std::string(argv[1]));
	}
	while (true) {
		std::string cmd = read_command();
		Micrologic.command(cmd);
		if (Micrologic.Exit == 1) break;
	}
	return 0;
}