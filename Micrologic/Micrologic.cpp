﻿#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Blocks.h"

using namespace std;

bool Exit = 0;
bool Echo = 1;

void command(Blocks& blocks, string cmd) {
	std::stringstream ss(cmd);
	std::string s;
	std::vector<std::string> args;
	while (std::getline(ss, s, ' ')) {
		args.push_back(s);
	}
	if (args.size() == 0) {}
	else if (args[0] == "end") Exit = 1;
	else if (args[0] == "line" && args.size() == 1) {
		blocks.add({ Line() });
		if (Echo) printf("No.%d line added.\n", (int)blocks.L.size() - 1);
	}
	else if (args[0] == "line" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 1) {
			int s = (int)blocks.L.size();
			for (int i = 0; i < a; i++) blocks.add({ Line() });
			if (Echo) printf("No.%d~%d line added.\n", s, (int)blocks.L.size() - 1);
		}
	}
	else if (args[0] == "N" && args.size() >= 3) {
		int a = -1, b = -1;
		try {
			a = atoi(args[1].c_str()); b = atoi(args[2].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size() && b >= 0 && b < blocks.L.size()) {
			blocks.add({ BlockN({&(blocks.L[a])},{&(blocks.L[b])}) });
			if (Echo) printf("No.%d blockN added. Input: No.%d line. Output: No.%d line.\n", (int)blocks.N.size() - 1, a, b);
		}
	}
	else if (args[0] == "A" && args.size() >= 4) {
		int a = -1, b = -1, c = -1;
		try {
			a = atoi(args[1].c_str()); b = atoi(args[2].c_str()); c = atoi(args[3].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size() && b >= 0 && b < blocks.L.size() && c >= 0 && c < blocks.L.size()) {
			blocks.add({ BlockA({&(blocks.L[a]),&(blocks.L[b])},{&(blocks.L[c])}) });
			if (Echo) printf("No.%d blockA added. Input: No.%d&%d line. Output: No.%d line.\n", (int)blocks.A.size() - 1, a, b, c);
		}
	}
	else if (args[0] == "R" && args.size() >= 4) {
		int a = -1, b = -1, c = -1;
		try {
			a = atoi(args[1].c_str()); b = atoi(args[2].c_str()); c = atoi(args[3].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size() && b >= 0 && b < blocks.L.size() && c >= 0 && c < blocks.L.size()) {
			blocks.add({ BlockR({&(blocks.L[a]),&(blocks.L[b])},{&(blocks.L[c])}) });
			if (Echo) printf("No.%d blockR added. Input: No.%d&%d line. Output: No.%d line.\n", (int)blocks.A.size() - 1, a, b, c);
		}
	}
	else if (args[0] == "T" && args.size() >= 3) {
		int a = -1, b = -1;
		try {
			a = atoi(args[1].c_str()); b = atoi(args[2].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size() && b >= 0 && b < blocks.L.size()) {
			blocks.add({ BlockT({&(blocks.L[a])},{&(blocks.L[b])}) });
			if (Echo) printf("No.%d blockT added. Input: No.%d line. Output: No.%d line.\n", (int)blocks.N.size() - 1, a, b);
		}
	}
	else if (args[0] == "check" && args.size() == 1) {
		printf("Value of lines are: ");
		for (auto l : blocks.L) printf("%d ", l.value);
		printf("\n");
	}
	else if (args[0] == "check" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size()) printf("Value of No.%d line is %d\n", a, blocks.L[a].value);
	}
	else if (args[0] == "set" && args.size() >= 3) {
		int a = -1, b = -1;
		try {
			a = atoi(args[1].c_str());
			b = atoi(args[2].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size() && (b == 0 || b == 1)) {
			blocks.L[a].set(b);
			if (Echo) printf("Set value of No.%d line to %d\n", a, (bool)b);
		}
	}
	else if (args[0] == "input:" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size()) {
			blocks.addInput({ a });
			if (Echo) printf("Added line No.%d as input line No.%d.\n", a, int(blocks.inputs.size() - 1));
		}
	}
	else if (args[0] == "input" && args.size() >= 3) {
		int a = -1, b = -1;
		try {
			a = atoi(args[1].c_str());
			b = atoi(args[2].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.inputs.size()) {
			if (b == 0 || b == 1) {
				blocks.input(a, b);
				printf("Inputed value %d to input line No.%d.\n", b, a);
			}
		}
	}
	else if (args[0] == "output:" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size()) {
			blocks.addOutput({ a });
			if (Echo) printf("Added line No.%d as output line No.%d.\n", a, int(blocks.outputs.size() - 1));
		}
	}
	else if (args[0] == "output" && args.size() == 1) {
		printf("Value of output lines are: ");
		for (bool l : blocks.output()) printf("%d ", l);
		printf("\n");
	}
	else if (args[0] == "output" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.outputs.size()) printf("Value of No.%d output line is %d\n", a, blocks.output(a));
	}
	else if (args[0] == "tick" && args.size() == 1) {
		blocks.tick();
		if (Echo) printf("Ticked.\n");
	}
	else if (args[0] == "tick" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 1) {
			for (int i = 0; i < a; i++) blocks.tick();
			if (Echo) printf("Ticked for %d times.\n", a);
		}
	}
	else if (args[0] == "tick!" && args.size() == 1) {
		blocks.tick();
		printf("Value of lines after ticking are: ");
		for (auto l : blocks.L) printf("%d ", l.value);
		printf("\n");
	}
	else if (args[0] == "tick!" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		for (int i = 0; i < a; i++) {
			blocks.tick();
			printf("Value of lines after ticking are: ");
			for (auto l : blocks.L) printf("%d ", l.value);
			printf("\n");
		}
	}
	else if (args[0] == "open" && args.size() >= 2) {
		ifstream fin;
		try {
			fin.open(args[1], ios::out | ios::in);
			printf("Opened: %s\n", args[1].c_str());
			char fcmd[1000];
			printf("\n");
			while (fin.getline(fcmd, 1000)) {
				command(blocks, fcmd);
			}
			printf("\n\n");
		}
		catch (...) {}
	}
	else if (args[0] == "echo") {
		printf((cmd.substr(5, cmd.size()) + "\n").c_str());
	}
	else if (args[0] == "@echo" && args.size() >= 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a == 0 || a == 1) Echo = a;
	}
	else if (args[0] == "clear") {
		printf("Cleared.\n");
		blocks.clear();
	}
}

int main(int argc, const char* argv[]) {
	//test
	Blocks blocks;
	blocks.add({ Blocks() });
	command(blocks, "line 4");
	command(blocks.Bs[0], "open C:/Users/14354/source/repos/Micrologic/plus.mcl");
	blocks.Bs[0].inputLines.push_back(&(blocks.L[0]));
	blocks.Bs[0].inputLines.push_back(&(blocks.L[1]));
	blocks.Bs[0].outputLines.push_back(&(blocks.L[2]));
	blocks.Bs[0].outputLines.push_back(&(blocks.L[3]));

	if (argc == 2) {
		command(blocks, "open " + string(argv[1]));
	}
	while (1) {
		printf(">>>");
		string cmd;
		getline(cin, cmd);
		command(blocks, cmd);
		if (Exit == 1) break;
	}
}