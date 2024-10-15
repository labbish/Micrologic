#include "Interpreter.h"

bool Interpreter::isNum(std::string str) {
	for (int i = 0; i < str.length(); i++) {
		char c = str[i];
		if ((c < '0' || c > '9') && !(i == 0 && c == '-')) return false;
	}
	return true;
}

int Interpreter::toInt(std::string str) {
	if (!isNum(str)) {
		ErrorMsg() << "Not a num: " << str;
		return -1;
	}
	try {
		return std::stoi(str);
	}
	catch (const std::invalid_argument&) {
		ErrorMsg() << "Not a num: " << str;
	}
	catch (const std::out_of_range&) {
		ErrorMsg() << "Num out of range: " << str;
	}
	return -1;
}

std::vector<int> Interpreter::toInt(std::vector<std::string> strs) {
	std::vector<int> ints;
	for (std::string str : strs) {
		ints.push_back(toInt(str));
	}
	return ints;
}

std::array<bool, 4> Interpreter::toBoolArray(std::array<int, 4> ints) {
	std::array<bool, 4> bools{};
	for (int i = 0; i < 4; i++) bools[i] = ints[i];
	return bools;
}

std::string Interpreter::quotedPart(std::string cmd) {
	int x = (int)cmd.find("\""), y = (int)cmd.rfind("\"");
	std::string f = cmd.substr(x + 1, y - x - 1);
	return f;
}

std::string Interpreter::firstWord(std::string l) {
	return l.substr(0, l.find(" "));
}

std::string Interpreter::pathPart(std::string f) {
	return f.substr(0, f.rfind("\\") + 1);
}

std::string Interpreter::addSlash(std::string p) {
	if (p[p.size() - 1] != '\\') p = p + "\\";
	return p;
}

std::string Interpreter::convertSlash(std::string filename) {
	for (int i = 0; i < filename.length(); i++) {
		if (filename[i] == '/') filename[i] = '\\';
	}
	return filename;
}

std::vector<std::string> Interpreter::breakLine(std::string cmd) {
	std::stringstream ss(cmd);
	std::string s;
	std::vector<std::string> args;
	while (std::getline(ss, s, ' ')) {
		args.push_back(s);
	}
	return args;
}

void Interpreter::line(int count) {
	if (!assertPositive(count)) return;
	int before = (int)blocks.L.size();
	for (int i = 0; i < count; i++) blocks.add({ Line(Line::LINE) });
	int after = (int)blocks.L.size();
	if (count == 1) writeMessage("LINE", after - 1);
	else writeMessage("LINES", before, after - 1);
}
void Interpreter::wline(int count) {
	if (!assertPositive(count)) return;
	int before = (int)blocks.L.size();
	for (int i = 0; i < count; i++) blocks.add({ Line(Line::WIDELINE) });
	int after = (int)blocks.L.size();
	if (count == 1) writeMessage("WLINE", after - 1);
	else writeMessage("WLINES", before, after - 1);
}
void Interpreter::N(int a, int b) {
	if (!assertInRange(a, blocks.L)) return;
	if (!assertInRange(b, blocks.L)) return;
	blocks.add({ BlockN({&(blocks.L[a])},{&(blocks.L[b])}) });
	writeMessage("BLOCKN", (int)blocks.N.size() - 1, a, b);
}
void Interpreter::A(int a, int b, int c) {
	if (!assertInRange(a, blocks.L)) return;
	if (!assertInRange(b, blocks.L)) return;
	if (!assertInRange(c, blocks.L)) return;
	blocks.add({ BlockA({&(blocks.L[a]),&(blocks.L[b])},{&(blocks.L[c])}) });
	writeMessage("BLOCKA", (int)blocks.A.size() - 1, a, b, c);
}
void Interpreter::R(int a, int b, int c) {
	if (!assertInRange(a, blocks.L)) return;
	if (!assertInRange(b, blocks.L)) return;
	if (!assertInRange(c, blocks.L)) return;
	blocks.add({ BlockR({&(blocks.L[a]),&(blocks.L[b])},{&(blocks.L[c])}) });
	writeMessage("BLOCKR", (int)blocks.A.size() - 1, a, b, c);
}
void Interpreter::T(int a, int b) {
	if (!assertInRange(a, blocks.L)) return;
	if (!assertInRange(b, blocks.L)) return;
	blocks.add({ BlockT({&(blocks.L[a])},{&(blocks.L[b])}) });
	writeMessage("BLOCKT", (int)blocks.N.size() - 1, a, b);
}
void Interpreter::C(std::array<int, 4> a, int b) {
	for (int i = 0; i < 4; i++) if (!assertInRange(a[i], blocks.L)) return;
	if (!assertInRange(b, blocks.L)) return;
	blocks.add({ BlockC({&(blocks.L[a[0]]),&(blocks.L[a[1]]),&(blocks.L[a[2]]),&(blocks.L[a[3]])},{&(blocks.L[b])}) });
	writeMessage("BLOCKC", (int)blocks.C.size() - 1, a[0], a[1], a[2], a[3], b);
}
void Interpreter::P(int a, std::array<int, 4> b) {
	if (!assertInRange(a, blocks.L)) return;
	for (int i = 0; i < 4; i++) if (!assertInRange(b[i], blocks.L)) return;
	blocks.add({ BlockP({&(blocks.L[a])},{&(blocks.L[b[0]]),&(blocks.L[b[1]]),&(blocks.L[b[2]]),&(blocks.L[b[3]])}) });
	writeMessage("BLOCKP", (int)blocks.P.size() - 1, a, b[0], b[1], b[2], b[3]);
}
void Interpreter::check() {
	writeMessage("CHECKS");
	for (Line l : blocks.L) printf("%s ", l.checkValue().c_str());
	printf("\n");
}
void Interpreter::check(int i) {
	if (!assertInRange(i, blocks.L)) return;
	writeMessage("CHECK", i, blocks.L[i].checkValue());
}
void Interpreter::set(int a, int value) {
	if (!assertInRange(a, blocks.L)) return;
	if (!assertBit(value)) return;
	blocks.L[a].set(value);
	writeMessage("SET", a, value);
}
void Interpreter::set(int a, std::array<int, 4> value) {
	if (!assertInRange(a, blocks.L)) return;
	for (int i = 0; i < 4; i++) if (!assertBit(value[i])) return;
	blocks.L[a].set(toBoolArray(value));
	writeMessage("SET", a, value);
}
void Interpreter::input_(int a) {
	if (!assertInRange(a, blocks.L)) return;
	blocks.addInput({ a });
	writeMessage("INPUT:", a, (int)blocks.inputs.size() - 1);
}
void Interpreter::input(int a, int value) {
	if (!assertInRange(a, blocks.inputs)) return;
	if (!assertBit(value)) return;
	blocks.input(a, value);
	writeMessage("INPUT", value, a);
}
void Interpreter::input(int a, std::array<int, 4> value) {
	if (!assertInRange(a, blocks.inputs)) return;
	for (int i = 0; i < 4; i++) if (!assertBit(value[i])) return;
	blocks.input(a, toBoolArray(value));
	writeMessage("INPUTW", value[0], value[1], value[2], value[3], a);
}
void Interpreter::output_(int a) {
	if (!assertInRange(a, blocks.L)) return;
	blocks.addOutput({ a });
	writeMessage("OUTPUT:", a, (int)blocks.inputs.size() - 1);
}
void Interpreter::output() {
	writeMessage("OUTPUTS");
	for (std::string l : blocks.output()) printf("%s ", l.c_str());
	printf("\n");
}
void Interpreter::output(int i) {
	if (!assertInRange(i, blocks.outputs)) return;
	writeMessage("OUTPUT", i, blocks.output(i));
}
void Interpreter::tick() {
	blocks.tick();
	writeMessage("TICK");
}
void Interpreter::tick(int t) {
	if (!assertPositive(t)) return;
	for (int i = 0; i < t; i++) blocks.tick();
	writeMessage("TICKS", t);
}
void Interpreter::tick_() {
	blocks.tick();
	writeMessage("TICK!");
	for (Line l : blocks.L) if (Echo) printf("%s ", l.checkValue().c_str());
	if (Echo) printf("\n");
}
void Interpreter::tick_(int t) {
	if (!assertPositive(t)) return;
	for (int i = 0; i < t; i++) {
		blocks.tick();
		writeMessage("TICKS!");
		for (Line l : blocks.L) if (Echo) printf("%s ", l.checkValue().c_str());
		if (Echo) printf("\n");
	}
}
void Interpreter::speed(int v) {
	if (!assertPositive(v)) return;
	blocks.speed = v;
	writeMessage("SPEED", v);
}
void Interpreter::open(std::string f) {
	std::ifstream fin;
	fin.open(f, std::ios::in);
	std::string nextPath = pathPart(f);
	if (!fin.good()) {
		f = path + f;
		fin.open(f, std::ios::in);
		nextPath = pathPart(f);
	}
	if (nextPath != "") path = nextPath;
	char fcmd[256] = "";
	if (!assertGoodFile(fin)) return;
	writeMessage("OPEN", f.c_str());
	while (fin.getline(fcmd, 256)) {
		command(fcmd);
	}
	if (Echo) printf("\n");
}
void Interpreter::safe_open(std::string f) {
	std::ifstream fin;
	fin.open(f, std::ios::in);
	std::string nextPath = pathPart(f);
	if (!fin.good()) {
		f = path + f;
		fin.open(f, std::ios::in);
		nextPath = pathPart(f);
	}
	if (nextPath != "") path = nextPath;
	char fcmd[256] = "";
	if (!assertGoodFile(fin)) return;
	writeMessage("SAFE_OPEN", f.c_str());
	SubInterpreter sub = SubInterpreter(*this);
	while (fin.getline(fcmd, 256)) {
		sub.command(fcmd);
	}
	if (Echo) printf("\n");
}
void Interpreter::mod(std::string name, std::string file) {
	blocks.mods.insert({ name, file });
	writeMessage("MOD", name.c_str(), file.c_str());
}
void Interpreter::check_mods() {
	writeMessage("CHECK_MODS");
	for (std::pair<std::string, std::string> mod : blocks.mods) {
		printf("%s (%s) ", mod.first.c_str(), mod.second.c_str());
	}
	if (Echo) printf("\n");
}
void Interpreter::block(std::string name, std::vector<int> ios) {
	std::vector<int> ins{}, outs{};
	if (!assertInMap(name, blocks.mods)) return;
	blocks.add(Blocks(name));
	Blocks& newBlock = blocks.Bs[blocks.Bs.size() - 1];
	std::string filename = blocks.mods[name];
	SubInterpreter(newBlock, exepath, path, lang, Echo).command("safe-open " + filename);
	if (ios.size() != newBlock.inputs.size() + newBlock.outputs.size()) {
		ErrorMsg() << "Incorrect line count";
		blocks.Bs.pop_back();
		return;
	}
	ins = subVec(ios, 0, (int)newBlock.inputs.size());
	outs = subVec(ios, (int)newBlock.inputs.size(), (int)ios.size());
	for (int i : ins) {
		if (!assertInRange(i, blocks.L)) {
			blocks.Bs.pop_back();
			return;
		}
		newBlock.inputLines.push_back(&(blocks.L[i]));
	}
	for (int o : outs) {
		if (!assertInRange(o, blocks.L)) {
			blocks.Bs.pop_back();
			return;
		}
		newBlock.outputLines.push_back(&(blocks.L[o]));
	}
	writeMessage("BLOCK1", (int)blocks.Bs.size() - 1, name.c_str());
	for (int i : ins) {
		if (Echo) printf("%d ", i);
	}
	writeMessage("BLOCK2");
	for (int o : outs) {
		if (Echo) printf("%d ", o);
	}
	writeMessage("BLOCK3");
}
void Interpreter::block_type(int a) {
	if (!assertInRange(a, blocks.Bs)) return;
	writeMessage("BLOCK_TYPE", a, blocks.Bs[a].type.c_str());
}
void Interpreter::tag(int a) {
	if (!assertInRange(a, blocks.L)) return;
	std::string I = (count(blocks.inputs.begin(), blocks.inputs.end(), a) != 0) ? "I" : "-";
	std::string O = (count(blocks.outputs.begin(), blocks.outputs.end(), a) != 0) ? "O" : "-";
	writeMessage("TAG", a);
	writeMessage("TAG" + I + O);
}
void Interpreter::type(int a) {
	if (!assertInRange(a, blocks.L)) return;
	std::string W = (blocks.L[a].mode == Line::WIDELINE) ? "W" : "L";
	writeMessage("TYPE", a);
	writeMessage("TYPE" + W);
}
void Interpreter::check_input() {
	writeMessage("CHECK_INPUTS");
	for (int i = 0; i < blocks.inputs.size(); i++)
		printf("%d ", blocks.inputs[i]);
	printf("\n");
}
void Interpreter::check_input(int a) {
	if (!assertInRange(a, blocks.inputs)) return;
	writeMessage("CHECK_INPUT", a, blocks.inputs[a]);
}
void Interpreter::check_output() {
	writeMessage("CHECK_OUTPUTS");
	for (int i = 0; i < blocks.outputs.size(); i++)
		printf("%d ", blocks.outputs[i]);
	printf("\n");
}
void Interpreter::check_output(int a) {
	if (!assertInRange(a, blocks.outputs)) return;
	writeMessage("CHECK_OUTPUT", a, blocks.outputs[a]);
}
void Interpreter::inspect(std::string type, int a) {
	bool b = 0;
	if (type == "N") {
		if (!assertInRange(a, blocks.N)) return;
		b = blocks.N[a].check();
	}
	if (type == "A") {
		if (!assertInRange(a, blocks.A)) return;
		b = blocks.A[a].check();
	}
	if (type == "R") {
		if (!assertInRange(a, blocks.R)) return;
		b = blocks.R[a].check();
	}
	if (type == "T") {
		if (!assertInRange(a, blocks.T)) return;
		b = blocks.T[a].check();
	}
	if (type == "C") {
		if (!assertInRange(a, blocks.C)) return;
		b = blocks.C[a].check();
	}
	if (type == "P") {
		if (!assertInRange(a, blocks.P)) return;
		b = blocks.P[a].check();
	}
	if (type == "block") {
		if (!assertInRange(a, blocks.Bs)) return;
		b = blocks.Bs[a].check();
	}
	writeMessage("INSPECT", type.c_str(), a, b);
}
void Interpreter::del(std::string type, int a) {
	if (type == "line") {
		if (!assertInRange(a, blocks.L)) return;
		blocks.L.erase(a);
	}
	if (type == "N") {
		if (!assertInRange(a, blocks.N)) return;
		blocks.N.erase(a);
	}
	if (type == "A") {
		if (!assertInRange(a, blocks.A)) return;
		blocks.A.erase(a);
	}
	if (type == "R") {
		if (!assertInRange(a, blocks.R)) return;
		blocks.R.erase(a);
	}
	if (type == "T") {
		if (!assertInRange(a, blocks.T)) return;
		blocks.T.erase(a);
	}
	if (type == "C") {
		if (!assertInRange(a, blocks.C)) return;
		blocks.C.erase(a);
	}
	if (type == "P") {
		if (!assertInRange(a, blocks.P)) return;
		blocks.P.erase(a);
	}
	if (type == "block") {
		if (!assertInRange(a, blocks.Bs)) return;
		blocks.Bs.erase(a);
	}
	writeMessage("DEL", type.c_str(), a);
}
void Interpreter::export__() {
	for (std::string line : blocks.exportBlocks()) printf("%s\n", line.c_str());
}
void Interpreter::echo(std::string msg) {
	printf((msg + "\n").c_str());
}
void Interpreter::_echo(int echo) {
	if (!assertBit(echo)) return;
	Echo = echo;
}
void Interpreter::__path() {
	writeMessage("PATH", path.c_str());
}
void Interpreter::__path(std::string p) {
	path = p;
	writeMessage("PATH_SET", path.c_str());
}
void Interpreter::clear() {
	system("cls");
	writeMessage("CLEAR");
	blocks.clear();
}
void Interpreter::help() {
	for (std::string l : getHelp()) printf((l + "\n").c_str());
}
void Interpreter::help(std::string cmd) {
	for (std::string l : getHelp())
		if (firstWord(l) == cmd) printf((l + "\n").c_str());
}
void Interpreter::__lang(std::string lan) {
	if (hasLanguage(lan)) {
		lang = lan;
		writeMessage("LANG", lang.c_str());
	}
	else if (lan == "list") {
		writeMessage("LANG_LIST");
		for (std::string l : getKeys()) {
			printf("%s ", l.c_str());
		}
		printf("\n");
	}
	else ErrorMsg() << "Language not found: " << lan;
}
void Interpreter::neko() {
	writeMessage("NEKO");
}

bool Interpreter::command(std::string cmd) {
	std::vector<std::string> args = breakLine(cmd);
	if (args.size() == 0) {}
	else if (args[0] == "end" && args.size() == 1) Exit = 1;
	else if (args[0] == "line" && args.size() == 1) line();
	else if (args[0] == "line" && args.size() == 2) line(toInt(args[1]));
	else if (args[0] == "wline" && args.size() == 1) wline();
	else if (args[0] == "wline" && args.size() == 2) wline(toInt(args[1]));
	else if (args[0] == "N" && args.size() == 3) N(toInt(args[1]), toInt(args[2]));
	else if (args[0] == "A" && args.size() == 4) A(toInt(args[1]), toInt(args[2]), toInt(args[3]));
	else if (args[0] == "R" && args.size() == 4) R(toInt(args[1]), toInt(args[2]), toInt(args[3]));
	else if (args[0] == "T" && args.size() == 3) T(toInt(args[1]), toInt(args[2]));
	else if (args[0] == "C" && args.size() == 6) C({ toInt(args[1]), toInt(args[2]), toInt(args[3]), toInt(args[4]) }, toInt(args[5]));
	else if (args[0] == "P" && args.size() == 6) P(toInt(args[1]), { toInt(args[2]), toInt(args[3]), toInt(args[4]), toInt(args[5]) });
	else if (args[0] == "check" && args.size() == 1) check();
	else if (args[0] == "check" && args.size() == 2) check(toInt(args[1]));
	else if (args[0] == "set" && args.size() == 3) set(toInt(args[1]), toInt(args[2]));
	else if (args[0] == "set" && args.size() == 6) set(toInt(args[1]), { toInt(args[2]), toInt(args[3]), toInt(args[4]), toInt(args[5]) });
	else if (args[0] == "input:" && args.size() == 2) input_(toInt(args[1]));
	else if (args[0] == "input" && args.size() == 3) input(toInt(args[1]), toInt(args[2]));
	else if (args[0] == "input" && args.size() == 6) input(toInt(args[1]), { toInt(args[2]), toInt(args[3]), toInt(args[4]), toInt(args[5]) });
	else if (args[0] == "output:" && args.size() == 2) output_(toInt(args[1]));
	else if (args[0] == "output" && args.size() == 1) output();
	else if (args[0] == "output" && args.size() == 2) output(toInt(args[1]));
	else if (args[0] == "tick" && args.size() == 1) tick();
	else if (args[0] == "tick" && args.size() == 2) tick(toInt(args[1]));
	else if (args[0] == "tick!" && args.size() == 1) tick_();
	else if (args[0] == "tick!" && args.size() == 2) tick_(toInt(args[1]));
	else if (args[0] == "speed" && args.size() == 2) speed(toInt(args[1]));
	else if (args[0] == "open" && count(cmd.begin(), cmd.end(), '\"') >= 2) open(convertSlash(quotedPart(cmd)));
	else if (args[0] == "open" && args.size() == 2) open(convertSlash(args[1]));
	else if (args[0] == "safe-open" && count(cmd.begin(), cmd.end(), '\"') >= 2) safe_open(convertSlash(quotedPart(cmd)));
	else if (args[0] == "safe-open" && args.size() == 2) safe_open(convertSlash(args[1]));
	else if (args[0] == "mod" && args.size() == 3) mod(args[1], args[2]);
	else if (args[0] == "check-mods" && args.size() == 1) check_mods();
	else if (args[0] == "block" && args.size() >= 2) block(args[1], toInt(subVec(args, 2, (int)args.size())));
	else if (args[0] == "block-type" && args.size() == 2) block_type(toInt(args[1]));
	else if (args[0] == "tag" && args.size() == 2) tag(toInt(args[1]));
	else if (args[0] == "type" && args.size() == 2) type(toInt(args[1]));
	else if (args[0] == "check-input" && args.size() == 1) check_input();
	else if (args[0] == "check-input" && args.size() == 2) check_input(toInt(args[1]));
	else if (args[0] == "check-output" && args.size() == 1) check_output();
	else if (args[0] == "check-output" && args.size() == 2) check_output(toInt(args[1]));
	else if (args[0] == "inspect" && args.size() == 3) inspect(args[1], toInt(args[2]));
	else if (args[0] == "del" && args.size() == 3) del(args[1], toInt(args[2]));
	else if (args[0] == "export" && args.size() == 1) export__();
	else if (args[0] == "echo" && args.size() > 1) echo(cmd.substr(5, cmd.size()));
	else if (args[0] == "@echo" && args.size() == 2) _echo(toInt(args[1]));
	else if (args[0] == "path" && args.size() == 1) __path();
	else if (args[0] == "path" && count(cmd.begin(), cmd.end(), '\"') >= 2) __path(addSlash(convertSlash(quotedPart(cmd))));
	else if (args[0] == "path" && args.size() == 2) __path(addSlash(args[1]));
	else if (args[0] == "clear") clear();
	else if (args[0] == "help" && args.size() == 1) help();
	else if (args[0] == "help" && args.size() == 2) help(args[1]);
	else if (args[0] == "lang" && args.size() == 2) __lang(args[1]);
	else if (args[0] == "neko" && args.size() == 1) neko();
	else ErrorMsg() << "No such command or incorrect argument count";
	writeDebug();
	return Echo;
}

void Interpreter::writeDebug() {
	FILE* file = _fsopen((exepath + "\\debug.log").c_str(), "w", _SH_DENYNO);
	if (file == nullptr) {
		ErrorMsg() << "Error writing debug.log";
	}
	else {
		for (int i = 0; i < blocks.L.size(); i++) {
			Line l = blocks.L[i];
			char I = blocks.isInput(i) ? 'I' : '-';
			char O = blocks.isOutput(i) ? 'O' : '-';
			fprintf(file, "%c%c%s ", I, O, l.checkValue().c_str());
		}
		fclose(file);
	}
}

std::vector<std::string> Interpreter::getHelp() {
	std::vector<std::string> helps;
	std::ifstream fin;
	fin.open(exepath + "\\help.txt", std::ios::in);
	if (!assertGoodFile(fin)) return { "" };
	char fhelp[1024] = "";
	while (fin.getline(fhelp, 1024)) {
		helps.push_back(fhelp);
	}
	return helps;
}

void Interpreter::writeMessage(std::string message, ...) {
	if (Echo) {
		va_list args;
		va_start(args, message);
		vprintf(getMessage(lang, message).c_str(), args);
		va_end(args);
	}
}

SubInterpreter::SubInterpreter(const Interpreter& father) :Interpreter(father) {}

void SubInterpreter::unavailableMessage(std::string cmd) {
	ErrorMsg() << "Command " << cmd << " is unavailable when opening file in safe mode";
}