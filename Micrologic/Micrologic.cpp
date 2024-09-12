#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Blocks.h"

using namespace std;

bool Exit = 0;
string path = "";

bool command(Blocks& blocks, string cmd) {
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
	else if (args[0] == "line" && args.size() == 2) {
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
	else if (args[0] == "wline" && args.size() == 1) {
		blocks.add({ Line(WIDELINE) });
		if (Echo) printf("No.%d wide line added.\n", (int)blocks.L.size() - 1);
	}
	else if (args[0] == "wline" && args.size() == 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 1) {
			int s = (int)blocks.L.size();
			for (int i = 0; i < a; i++) blocks.add({ Line(WIDELINE) });
			if (Echo) printf("No.%d~%d wide line added.\n", s, (int)blocks.L.size() - 1);
		}
	}
	else if (args[0] == "N" && args.size() == 3) {
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
	else if (args[0] == "A" && args.size() == 4) {
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
	else if (args[0] == "R" && args.size() == 4) {
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
	else if (args[0] == "T" && args.size() == 3) {
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
	else if (args[0] == "C" && args.size() == 6) {
		int a[4] = { -1,-1,-1,-1 }, b = -1;
		try {
			for (int i = 0; i < 4; i++) a[i] = atoi(args[i + 1].c_str());
			b = atoi(args[5].c_str());
		}
		catch (...) {}
		bool valid = 1;
		for (int i = 0; i < 4; i++) if (a[i] < 0 || a[i] >= blocks.L.size()) valid = 0;
		if (b >= 0 && b <= blocks.L.size() && valid) {
			blocks.add({ BlockC({&(blocks.L[a[0]]),&(blocks.L[a[1]]),&(blocks.L[a[2]]),&(blocks.L[a[3]])},{&(blocks.L[b])}) });
			if (Echo) printf("No.%d blockC added. Input: No.%d %d %d %d line. Output: No.%d line.\n", (int)blocks.C.size() - 1, a[0], a[1], a[2], a[3], b);
		}
	}
	else if (args[0] == "P" && args.size() == 6) {
		int a = -1, b[4] = { -1,-1,-1,-1 };
		try {
			a = atoi(args[1].c_str());
			for (int i = 0; i < 4; i++) b[i] = atoi(args[i + 2].c_str());
		}
		catch (...) {}
		bool valid = 1;
		for (int i = 0; i < 4; i++) if (b[i] < 0 || b[i] >= blocks.L.size()) valid = 0;
		if (a >= 0 && a <= blocks.L.size() && valid) {
			blocks.add({ BlockP({&(blocks.L[a])} ,{&(blocks.L[b[0]]),&(blocks.L[b[1]]),&(blocks.L[b[2]]),&(blocks.L[b[3]])}) });
			if (Echo) printf("No.%d blockP added. Input: No.%d line. Output: No.%d %d %d %d line.\n", (int)blocks.P.size() - 1, a, b[0], b[1], b[2], b[3]);
		}
	}
	else if (args[0] == "check" && args.size() == 1) {
		printf("Value of lines are: ");
		for (Line l : blocks.L) printf("%s ", l.checkValue().c_str());
		printf("\n");
	}
	else if (args[0] == "check" && args.size() == 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size()) printf("Value of No.%d line is %s\n", a, blocks.L[a].checkValue().c_str());
	}
	else if (args[0] == "set" && args.size() == 3) {
		int a = -1, b = -1;
		try {
			a = atoi(args[1].c_str());
			b = atoi(args[2].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.L.size() && (b == 0 || b == 1)) {
			blocks.L[a].set(b);
			if (Echo) printf("Set value of No.%d line to %d.\n", a, b);
		}
	}
	else if (args[0] == "set" && args.size() == 6) {
		int a = -1;
		short b[4] = { -1,-1,-1,-1 };
		try {
			a = atoi(args[1].c_str());
			for (int i = 0; i < 4; i++) b[i] = atoi(args[i + 2].c_str());
		}
		catch (...) {}
		bool valid = 1;
		for (int i = 0; i < 4; i++) if (b[i] != 0 && b[i] != 1) valid = 0;
		if (a >= 0 && a < blocks.L.size() && valid) {
			bool c[4];
			for (int i = 0; i < 4; i++) c[i] = b[i];
			blocks.L[a].set(c);
			if (Echo) printf("Set wide value of No.%d line to %d%d%d%d.\n", a, b[0], b[1], b[2], b[3]);
		}
	}
	else if (args[0] == "input:" && args.size() == 2) {
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
	else if (args[0] == "input" && args.size() == 3) {
		int a = -1, b = -1;
		try {
			a = atoi(args[1].c_str());
			b = atoi(args[2].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.inputs.size()) {
			if (b == 0 || b == 1) {
				blocks.input(a, b);
				if (Echo) printf("Inputted value %d to input line No.%d.\n", b, a);
			}
		}
	}
	else if (args[0] == "input" && args.size() == 6) {
		int a = -1, b[4] = { -1,-1,-1,-1 };
		try {
			a = atoi(args[1].c_str());
			for (int i = 0; i < 4; i++) b[i] = atoi(args[i + 2].c_str());
		}
		catch (...) {}
		bool valid = 1;
		for (int i = 0; i < 4; i++) if (b[i] != 0 && b[i] != 1) valid = 0;
		if (a >= 0 && a < blocks.L.size() && valid) {
			bool c[4];
			for (int i = 0; i < 4; i++) c[i] = b[i];
			blocks.input(a, c);
			if (Echo) printf("Inputted value %d%d%d%d to input line No.%d.\n", b[0], b[1], b[2], b[3], a);
		}
	}
	else if (args[0] == "output:" && args.size() == 2) {
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
		for (std::string l : blocks.output()) printf("%s ", l.c_str());
		printf("\n");
	}
	else if (args[0] == "output" && args.size() == 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a >= 0 && a < blocks.outputs.size()) printf("Value of No.%d output line is %s\n", a, blocks.output(a).c_str());
	}
	else if (args[0] == "tick" && args.size() == 1) {
		blocks.tick();
		if (Echo) printf("Ticked.\n");
	}
	else if (args[0] == "tick" && args.size() == 2) {
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
		for (Line l : blocks.L) printf("%s ", l.checkValue().c_str());
		printf("\n");
	}
	else if (args[0] == "tick!" && args.size() == 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		for (int i = 0; i < a; i++) {
			blocks.tick();
			printf("Value of lines after ticking are: ");
			for (Line l : blocks.L) printf("%s ", l.checkValue().c_str());
			printf("\n");
		}
	}
	else if (args[0] == "open" && args.size() == 2) {
		ifstream fin;
		try {
			fin.open(path + args[1], ios::out | ios::in);
			char fcmd[256] = "";
			bool b = fin.good() && Echo;
			if (b) printf("Opened: %s\n\n", args[1].c_str());
			while (fin.getline(fcmd, 256)) {
				command(blocks, fcmd);
			}
			if (b) printf("\n");
		}
		catch (...) {}
	}
	else if (args[0] == "mod" && args.size() == 3) {
		blocks.mods.insert({ args[1],args[2] });
		if (Echo) printf("Loaded Mod: %s (%s)\n", args[1].c_str(), args[2].c_str());
	}
	else if (args[0] == "block" && args.size() >= 2) {
		string filename = blocks.mods[args[1]];
		bool del = 0;
		blocks.Bs.push_back(Blocks());
		command(blocks.Bs[blocks.Bs.size() - 1], "open " + filename);
		vector<int> ins, outs;
		if (blocks.Bs[blocks.Bs.size() - 1].inputs.size() == 0 && blocks.Bs[blocks.Bs.size() - 1].outputs.size() == 0) del = 1;
		if (args.size() >= 2 + blocks.Bs[blocks.Bs.size() - 1].inputs.size() + blocks.Bs[blocks.Bs.size() - 1].outputs.size()) {
			for (int i = 2; i < 2 + blocks.Bs[blocks.Bs.size() - 1].inputs.size(); i++) {
				int a = -1;
				try {
					a = atoi(args[i].c_str());
				}
				catch (...) {}
				if (a >= 0 && a < blocks.L.size()) {
					blocks.Bs[blocks.Bs.size() - 1].inputLines.push_back(&(blocks.L[a]));
					ins.push_back(a);
				}
				else del = 1;
			}
			for (int i = 2 + blocks.Bs[blocks.Bs.size() - 1].inputs.size(); i < 2 + blocks.Bs[blocks.Bs.size() - 1].inputs.size() + blocks.Bs[blocks.Bs.size() - 1].outputs.size(); i++) {
				int a = -1;
				try {
					a = atoi(args[i].c_str());
				}
				catch (...) {}
				if (a >= 0 && a < blocks.L.size()) {
					blocks.Bs[blocks.Bs.size() - 1].outputLines.push_back(&(blocks.L[a]));
					outs.push_back(a);
				}
				else del = 1;
			}
		}
		else del = 1;
		if (del) blocks.Bs.pop_back();
		else {
			if (Echo) {
				printf("No.%d block (%s) added. Input: No.", (int)blocks.Bs.size() - 1, args[1].c_str());
				for (int a : ins) {
					printf("%d ", a);
				}
				printf("line. Output: No.");
				for (int a : outs) {
					printf("%d ", a);
				}
				printf("line.\n");
			}
		}
	}
	else if (args[0] == "inspect" && args.size() == 3) {
		string type = args[1];
		int a = -1;
		try {
			a = atoi(args[2].c_str());
			bool b;
			bool valid = 0;
			if (type == "N") { b = blocks.N[a].check(); valid = 1; }
			if (type == "A") { b = blocks.A[a].check(); valid = 1; }
			if (type == "R") { b = blocks.R[a].check(); valid = 1; }
			if (type == "T") { b = blocks.T[a].check(); valid = 1; }
			if (type == "C") { b = blocks.C[a].check(); valid = 1; }
			if (type == "P") { b = blocks.P[a].check(); valid = 1; }
			if (type == "block") { b = blocks.Bs[a].check(); valid = 1; }
			if (valid) printf("Inspection result of %s No.%d is %d.\n", type.c_str(), a, b);
		}
		catch (...) {}
	}
	else if (args[0] == "echo") {
		if (Echo) printf((cmd.substr(5, cmd.size()) + "\n").c_str());
	}
	else if (args[0] == "@echo" && args.size() == 2) {
		int a = -1;
		try {
			a = atoi(args[1].c_str());
		}
		catch (...) {}
		if (a == 0 || a == 1) Echo = a;
	}
	else if (args[0] == "path" && args.size() == 1) {
		printf("Current Path: %s\n", path.c_str());
	}
	else if (args[0] == "path" && args.size() == 2) {
		path = args[1];
		printf("Set path to: %s\n", args[1].c_str());
	}
	else if (args[0] == "clear") {
		system("cls");
		printf("Cleared.\n");
		blocks.clear();
	}
	return Echo;
}

int main(int argc, const char* argv[]) {
	Blocks blocks;
	if (argc == 2) {
		Echo = command(blocks, "open " + string(argv[1]));
	}
	while (1) {
		printf(">>>");
		string cmd;
		getline(cin, cmd);
		command(blocks, cmd);
		if (Exit == 1) break;
	}
}