#include "Interpreter.h"
#include <Windows.h>

int main(int argc, const char* argv[]) {
	SetConsoleOutputCP(CP_UTF8);
	Blocks blocks;
	std::string exepath = std::string(_getcwd(NULL, 0));
	Interpreter Micrologic(blocks, exepath);
	if (argc == 2) {
		Micrologic.Echo = Micrologic.command("open " + std::string(argv[1]));
	}
	while (true) {
		printf(">>>");
		std::string cmd;
		std::getline(std::cin, cmd);
		Micrologic.command(cmd);
		if (Micrologic.Exit == 1) break;
	}
}