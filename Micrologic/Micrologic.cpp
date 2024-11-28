#include "Interpreter.h"
#include <thread>

using namespace labbish::Micrologic;

void checkUpdateInThread(Interpreter& Micrologic) {
	Micrologic.checkUpdate();
}

int main(int argc, const char* argv[]) {
	SetEncoding();
	Blocks blocks;
	std::string exepath(_getcwd(NULL, 0));
	Interpreter Micrologic(blocks, exepath);
	std::thread updateThread(checkUpdateInThread, std::ref(Micrologic));
	if (argc == 2) {
		Micrologic.command("open " + std::string(argv[1]));
	}
	while (true) {
		Micrologic.flushUpdateMessage();
		std::string cmd = read_command();
		Micrologic.command(cmd);
		if (Micrologic.Exit == 1) break;
	}
	updateThread.join();
	return 0;
}