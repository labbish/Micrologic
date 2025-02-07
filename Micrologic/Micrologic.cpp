#include "Interpreter.h"
#include <thread>

using namespace labbish::Micrologic;

int main(int argc, const char* argv[]) {
	SetEncoding();
	Blocks blocks;
	std::string exepath(_getcwd(NULL, 0));
	Interpreter Micrologic(blocks, exepath);
	std::jthread updateThread([&]() {Micrologic.checkUpdate(); });
	if (argc == 2) {
		Micrologic.command(std::format("open {}", argv[1]));
	}
	while (true) {
		Micrologic.flushUpdateMessage();
		std::string cmd = read_command();
		Micrologic.command(cmd);
		if (Micrologic.Exit == 1) break;
	}
	return 0;
}