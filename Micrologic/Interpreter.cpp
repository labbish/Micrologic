#include "Interpreter.h"

namespace labbish::Micrologic {
	std::vector<std::string> exportStructure(const Blocks& blocks, std::optional<std::string> path) {
		std::vector<std::string> commands{};

		if (path != std::nullopt) commands.push_back(std::format("path \"{}\"", *path));
		for (std::pair<std::string, std::string> mod : blocks.mods) {
			commands.push_back(std::format("mod {} {}", mod.first, mod.second));
		}

		auto lineFormat = [](Line::TYPE type, int num) -> std::string {
			std::string cmd;
			if (type == Line::LINE) cmd = "line";
			else cmd = "wline";
			if (num == 1) return cmd;
			else return std::format("{} {}", cmd, num);
			};
		int consecutive = 1;
		for (int i = 0; i < blocks.L.size(); i++) {
			if (i != blocks.L.size() - 1) {
				if (blocks.L[i].mode == blocks.L[i + 1].mode) consecutive++;
				else {
					commands.push_back(lineFormat(blocks.L[i].mode, consecutive));
					consecutive = 1;
				}
			}
			else commands.push_back(lineFormat(blocks.L[i].mode, consecutive));
		}

		for (const BlockN& n : blocks.N) {
			commands.push_back(std::format("N {} {}", to_string(blocks.findLine(n.inputLines[0])), to_string(blocks.findLine(n.outputLines[0]))));
		}
		for (const BlockA& a : blocks.A) {
			commands.push_back(std::format("A {} {} {}", to_string(blocks.findLine(a.inputLines[0])), to_string(blocks.findLine(a.inputLines[1])), to_string(blocks.findLine(a.outputLines[0]))));
		}
		for (const BlockR& r : blocks.R) {
			commands.push_back(std::format("R {} {} {}", to_string(blocks.findLine(r.inputLines[0])), to_string(blocks.findLine(r.inputLines[1])), to_string(blocks.findLine(r.outputLines[0]))));
		}
		for (const BlockT& t : blocks.T) {
			commands.push_back(std::format("T {} {}", to_string(blocks.findLine(t.inputLines[0])), to_string(blocks.findLine(t.outputLines[0]))));
		}
		for (const BlockC& c : blocks.C) {
			commands.push_back(std::format("C {} {} {} {} {}", to_string(blocks.findLine(c.inputLines[0])), to_string(blocks.findLine(c.inputLines[1])), to_string(blocks.findLine(c.inputLines[2])), to_string(blocks.findLine(c.inputLines[3])), to_string(blocks.findLine(c.outputLines[0]))));
		}
		for (const BlockP& p : blocks.P) {
			commands.push_back(std::format("P {} {} {} {} {}", to_string(blocks.findLine(p.inputLines[0])), to_string(blocks.findLine(p.outputLines[0])), to_string(blocks.findLine(p.outputLines[1])), to_string(blocks.findLine(p.outputLines[2])), to_string(blocks.findLine(p.outputLines[3]))));
		}
		for (const Blocks& bs : blocks.Bs) {
			std::string cmd = std::format("block {} ", bs.type);
			for (int i = 0; i < bs.inputLines.size(); i++) cmd = cmd + std::format("{} ", to_string(blocks.findLine(bs.inputLines[i])));
			for (int o = 0; o < bs.outputLines.size(); o++) cmd = cmd + std::format("{} ", to_string(blocks.findLine(bs.outputLines[o])));
			commands.push_back(cmd);
		}

		for (int i : blocks.inputs) {
			commands.push_back(std::format("input: {}", i));
		}
		for (int o : blocks.outputs) {
			commands.push_back(std::format("output: {}", o));
		}

		return commands;
	}

	std::vector<std::string> exportLineData(const Blocks& blocks) {
		std::vector<std::string> commands{};
		for (int i = 0; i < blocks.L.size(); i++) {
			if (blocks.L[i].mode == Line::LINE) commands.push_back(std::format("set {} {}", i, (int)blocks.L[i].value));
			else commands.push_back(std::format("set {} {} {} {} {}", i, (int)blocks.L[i].wideValue[0], (int)blocks.L[i].wideValue[1], (int)blocks.L[i].wideValue[2], (int)blocks.L[i].wideValue[3]));
		}
		return commands;
	}

	bool Interpreter::isdirty(char c) {
		return c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
	}
	bool Interpreter::isspace(char c) {
		return c == ' ' || isdirty(c);
	}

	void Interpreter::normalizeArg(std::string& str) {
		str.erase(std::remove_if(str.begin(), str.end(), [this](char c) { return this->isdirty(c); }), str.end());
	} // delete all dirty chars

	void Interpreter::normalizeArgs(std::vector<std::string>& strs) {
		for (int i = 0; i < strs.size(); i++) {
			normalizeArg(strs[i]);
		}
	}

	bool Interpreter::isNum(std::string str) {
		if (str == "") return false;
		for (int i = 0; i < str.length(); i++) {
			char c = str[i];
			if ((c < '0' || c > '9') && !(i == 0 && c == '-')) return false;
		}
		return true;
	}

	int_ Interpreter::toInt(char c) {
		std::string str(1, c);
		return toInt(str);
	}
	int_ Interpreter::toInt(std::string str) {
		if (!isNum(str)) {
			writeError("NOT_NUM", str);
			return std::nullopt;
		}
		try {
			return std::stoi(str);
		}
		catch (const std::invalid_argument&) {
			writeError("NOT_NUM", str);
		}
		catch (const std::out_of_range&) {
			writeError("OUT_OF_RANGE", str);
		}
		return std::nullopt;
	}

	std::vector<int_> Interpreter::toInt(std::vector<std::string> strs) {
		std::vector<int_> ints;
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
		return f.substr(0, f.rfind(StandardSlash) + 1);
	}

	std::string Interpreter::addSlash(std::string p) {
		if (p == "") return "";
		if (p[p.size() - 1] != StandardSlash[0]) p = p + StandardSlash;
		return p;
	}

	std::string Interpreter::convertSlash(std::string filename) {
		for (int i = 0; i < filename.length(); i++) {
			if (filename[i] == Slash[0]) filename[i] = StandardSlash[0];
		}
		return filename;
	}

	std::string Interpreter::subCommand(std::vector<std::string> cmd, size_t pos, size_t len) {
		std::vector<std::string> subcmd{};
		for (size_t i = pos; i - pos < len && i < cmd.size(); i++) {
			subcmd.push_back(cmd[i]);
		}
		return combineLine(subcmd);
	}

	std::string Interpreter::trim(std::string cmd) {
		if (cmd == "") return cmd;
		while (isspace(cmd[0])) cmd.erase(0, 1);
		if (cmd == "") return cmd;
		while (isspace(cmd[cmd.length() - 1])) cmd.erase(cmd.length() - 1, 1);
		if (cmd.length() >= 2) {
			if (cmd[0] == '\"' && cmd[cmd.length() - 1] == '\"') {
				cmd.erase(0, 1);
				cmd.erase(cmd.length() - 1, 1);
			}
		}
		return cmd;
	}

	std::pair<std::string, std::string> Interpreter::cutRedirection(std::string cmd) {
		size_t pos;
		if (cmd == "") return { "", "" };
		if (cmd[cmd.length() - 1] != '\"') pos = cmd.rfind(">");
		else {
			size_t quote_pos2 = cmd.rfind("\"", cmd.length() - 1);
			size_t quote_pos1 = cmd.rfind("\"", quote_pos2 - 1);
			pos = cmd.rfind(">", quote_pos1);
		}
		if (pos != std::string::npos) return { cmd.substr(0, pos), trim(cmd.substr(pos + 1)) };
		else return { cmd, "" };
	}

	std::vector<std::string> Interpreter::breakLine(std::string cmd) {
		std::stringstream ss(cmd);
		std::string s;
		std::vector<std::string> args;
		while (std::getline(ss, s, ' ')) {
			args.push_back(s);
		}
		normalizeArgs(args);
		return args;
	}

	std::string Interpreter::combineLine(std::vector<std::string> args) {
		std::string cmd = "";
		for (std::string arg : args) {
			cmd += (arg + " ");
		}
		if (cmd != "") cmd = cmd.substr(0, cmd.length() - 1);
		return cmd;
	}

	void Interpreter::end() {
		Exit = 1;
	}
	void Interpreter::line(int_ count) {
		if (!assertPositive(count)) return;
		int before = (int)blocks.L.size();
		for (int i = 0; i < count; i++) blocks.add({ Line(Line::LINE) });
		int after = (int)blocks.L.size();
		if (count == 1) writeMessage("LINE", after - 1);
		else writeMessage("LINES", before, after - 1);
	}
	void Interpreter::wline(int_ count) {
		if (!assertPositive(count)) return;
		int before = (int)blocks.L.size();
		for (int i = 0; i < count; i++) blocks.add({ Line(Line::WIDELINE) });
		int after = (int)blocks.L.size();
		if (count == 1) writeMessage("WLINE", after - 1);
		else writeMessage("WLINES", before, after - 1);
	}
	void Interpreter::N(int_ a, int_ b) {
		if (!assertInRange(a, blocks.L)) return;
		if (!assertInRange(b, blocks.L)) return;
		if (!assertLineType(a, Line::LINE)) return;
		if (!assertLineType(b, Line::LINE)) return;
		blocks.add({ BlockN({&(blocks.L[*a])},{&(blocks.L[*b])}) });
		writeMessage("BLOCKN", (int)blocks.N.size() - 1, *a, *b);
	}
	void Interpreter::A(int_ a, int_ b, int_ c) {
		if (!assertInRange(a, blocks.L)) return;
		if (!assertInRange(b, blocks.L)) return;
		if (!assertInRange(c, blocks.L)) return;
		if (!assertLineType(a, Line::LINE)) return;
		if (!assertLineType(b, Line::LINE)) return;
		if (!assertLineType(c, Line::LINE)) return;
		blocks.add({ BlockA({&(blocks.L[*a]),&(blocks.L[*b])},{&(blocks.L[*c])}) });
		writeMessage("BLOCKA", (int)blocks.A.size() - 1, *a, *b, *c);
	}
	void Interpreter::R(int_ a, int_ b, int_ c) {
		if (!assertInRange(a, blocks.L)) return;
		if (!assertInRange(b, blocks.L)) return;
		if (!assertInRange(c, blocks.L)) return;
		if (!assertLineType(a, Line::LINE)) return;
		if (!assertLineType(b, Line::LINE)) return;
		if (!assertLineType(c, Line::LINE)) return;
		blocks.add({ BlockR({&(blocks.L[*a]),&(blocks.L[*b])},{&(blocks.L[*c])}) });
		writeMessage("BLOCKR", (int)blocks.R.size() - 1, *a, *b, *c);
	}
	void Interpreter::T(int_ a, int_ b) {
		if (!assertInRange(a, blocks.L)) return;
		if (!assertInRange(b, blocks.L)) return;
		if (!assertSameLineType(a, b)) return;
		blocks.add({ BlockT({&(blocks.L[*a])},{&(blocks.L[*b])}) });
		writeMessage("BLOCKT", (int)blocks.T.size() - 1, *a, *b);
	}
	void Interpreter::C(std::array<int_, 4> a, int_ b) {
		for (int i = 0; i < 4; i++) if (!assertInRange(a[i], blocks.L)) return;
		for (int i = 0; i < 4; i++) if (!assertLineType(a[i], Line::LINE)) return;
		if (!assertInRange(b, blocks.L)) return;
		if (!assertLineType(b, Line::WIDELINE)) return;
		blocks.add({ BlockC({&(blocks.L[*a[0]]),&(blocks.L[*a[1]]),&(blocks.L[*a[2]]),&(blocks.L[*a[3]])},{&(blocks.L[*b])}) });
		writeMessage("BLOCKC", (int)blocks.C.size() - 1, *a[0], *a[1], *a[2], *a[3], *b);
	}
	void Interpreter::P(int_ a, std::array<int_, 4> b) {
		if (!assertInRange(a, blocks.L)) return;
		if (!assertLineType(a, Line::WIDELINE)) return;
		for (int i = 0; i < 4; i++) if (!assertInRange(b[i], blocks.L)) return;
		for (int i = 0; i < 4; i++) if (!assertLineType(b[i], Line::LINE)) return;
		blocks.add({ BlockP({&(blocks.L[*a])},{&(blocks.L[*b[0]]),&(blocks.L[*b[1]]),&(blocks.L[*b[2]]),&(blocks.L[*b[3]])}) });
		writeMessage("BLOCKP", (int)blocks.P.size() - 1, *a, *b[0], *b[1], *b[2], *b[3]);
	}
	void Interpreter::check() {
		writeMessage("CHECKS");
		for (Line l : blocks.L) fprintf(out, "%s ", l.checkValue().c_str());
		fprintf(out, "\n");
	}
	void Interpreter::check(int_ i) {
		if (!assertInRange(i, blocks.L)) return;
		writeMessage("CHECK", i, blocks.L[*i].checkValue());
	}
	void Interpreter::set(int_ a, int_ value) {
		if (!assertInRange(a, blocks.L)) return;
		if (!assertBit(value)) return;
		if (!assertLineType(a, Line::LINE)) return;
		blocks.L[*a].set(*value);
		writeMessage("SET", *a, *value);
	}
	void Interpreter::set(int_ a, std::array<int_, 4> value) {
		if (!assertInRange(a, blocks.L)) return;
		for (int i = 0; i < 4; i++) if (!assertBit(value[i])) return;
		if (!assertLineType(a, Line::WIDELINE)) return;
		blocks.L[*a].set(toBoolArray(*value));
		writeMessage("SETW", *a, *value[0], *value[1], *value[2], *value[3]);
	}
	void Interpreter::input_(int_ a) {
		if (!assertInRange(a, blocks.L)) return;
		blocks.addInput({ *a });
		writeMessage("INPUT:", *a, (int)blocks.inputs.size() - 1);
	}
	void Interpreter::input(int_ a, int_ value) {
		if (!assertInRange(a, blocks.inputs)) return;
		if (!assertBit(value)) return;
		blocks.input(*a, *value);
		writeMessage("INPUT", *value, *a);
	}
	void Interpreter::input(int_ a, std::array<int_, 4> value) {
		if (!assertInRange(a, blocks.inputs)) return;
		for (int i = 0; i < 4; i++) if (!assertBit(value[i])) return;
		blocks.input(*a, toBoolArray(*value));
		writeMessage("INPUTW", *value[0], *value[1], *value[2], *value[3], a);
	}
	void Interpreter::output_(int_ a) {
		if (!assertInRange(a, blocks.L)) return;
		blocks.addOutput({ *a });
		writeMessage("OUTPUT:", *a, (int)blocks.inputs.size() - 1);
	}
	void Interpreter::output() {
		writeMessage("OUTPUTS");
		for (std::string l : blocks.output()) fprintf(out, "%s ", l.c_str());
		fprintf(out, "\n");
	}
	void Interpreter::output(int_ i) {
		if (!assertInRange(i, blocks.outputs)) return;
		writeMessage("OUTPUT", *i, blocks.output(*i));
	}
	void Interpreter::tick() {
		blocks.tick();
		writeMessage("TICK");
	}
	void Interpreter::tick(int_ t) {
		if (!assertPositive(t)) return;
		for (int i = 0; i < *t; i++) blocks.tick();
		writeMessage("TICKS", *t);
	}
	void Interpreter::tick_() {
		blocks.tick();
		writeMessage("TICK!");
		for (Line l : blocks.L) if (Echo) fprintf(out, "%s ", l.checkValue().c_str());
		if (Echo) fprintf(out, "\n");
	}
	void Interpreter::tick_(int_ t) {
		if (!assertPositive(t)) return;
		for (int i = 0; i < *t; i++) {
			blocks.tick();
			writeMessage("TICKS!");
			for (Line l : blocks.L) if (Echo) fprintf(out, "%s ", l.checkValue().c_str());
			if (Echo) fprintf(out, "\n");
		}
	}
	void Interpreter::speed() {
		writeMessage("SPEED_CHECK", blocks.speed);
	}
	void Interpreter::speed(int_ v) {
		if (!assertPositive(v)) return;
		blocks.speed = *v;
		writeMessage("SPEED", *v);
	}
	void Interpreter::openInterface(std::string f, Interpreter* interpreter) {
		std::string f1 = f;
		std::ifstream fin;
		fin.open(f1, std::ios::in);
		std::string nextPath = pathPart(f1);
		if (!fin.good()) {
			f1 = path + f1;
			fin.open(f1, std::ios::in);
			nextPath = pathPart(f1);
		}
		if (nextPath != "") interpreter->path = path = nextPath;
		char fcmd[256] = "";
		if (!assertGoodFile(fin, f)) return;
		writeMessage("OPEN", f1.c_str());
		while (fin.getline(fcmd, 256)) {
			if (perStep) pause();
			interpreter->command(fcmd);
		}
		if (Echo) fprintf(out, "\n");
	}
	void Interpreter::open(std::string f) {
		SubInterpreter tempInterpreter = SubInterpreter(*this);
		tempInterpreter.defaultOut = out;
		tempInterpreter.position = { 0, f };
		openInterface(f, &tempInterpreter);
	}
	void Interpreter::safe_open(std::string f) {
		SafeInterpreter tempInterpreter = SafeInterpreter(*this);
		tempInterpreter.defaultOut = out;
		tempInterpreter.position = { 0, f };
		openInterface(f, &tempInterpreter);
	}
	void Interpreter::mod(std::string name, std::string file) {
		blocks.mods.insert({ name, file });
		writeMessage("MOD", name.c_str(), file.c_str());
	}
	void Interpreter::check_mods() {
		writeMessage("CHECK_MODS");
		for (std::pair<std::string, std::string> mod : blocks.mods) {
			fprintf(out, "%s (%s) ", mod.first.c_str(), mod.second.c_str());
		}
		if (Echo) fprintf(out, "\n");
	}
	void Interpreter::block(std::string name, std::vector<int_> ios) {
		std::vector<int_> ins{}, outs{};
		if (!assertInMap(name, blocks.mods)) return;
		blocks.add(Blocks(name));
		Blocks& newBlock = blocks.Bs[blocks.Bs.size() - 1];
		std::string filename = blocks.mods[name];
		SafeInterpreter(newBlock, exepath, path, lang, Echo, out).command("safe-open " + filename);
		if (ios.size() != newBlock.inputs.size() + newBlock.outputs.size()) {
			writeError("LINE_COUNT");
			blocks.Bs.pop_back();
			return;
		}
		ins = subVec(ios, 0, (int)newBlock.inputs.size());
		outs = subVec(ios, (int)newBlock.inputs.size(), (int)ios.size());
		for (int_ i : ins) {
			if (!assertInRange(i, blocks.L)) {
				blocks.Bs.pop_back();
				return;
			}
			if (!assertLineType(i, newBlock.L[newBlock.inputLines.size()].mode)) {
				blocks.Bs.pop_back();
				return;
			}
			newBlock.inputLines.push_back(&(blocks.L[*i]));
		}
		for (int_ o : outs) {
			if (!assertInRange(o, blocks.L)) {
				blocks.Bs.pop_back();
				return;
			}
			if (!assertLineType(o, newBlock.L[newBlock.inputLines.size() + newBlock.outputLines.size()].mode)) {
				blocks.Bs.pop_back();
				return;
			}
			newBlock.outputLines.push_back(&(blocks.L[*o]));
		}
		writeMessage("BLOCK1", (int)blocks.Bs.size() - 1, name.c_str());
		for (int i : *ins) {
			if (Echo) fprintf(out, "%d ", i);
		}
		writeMessage("BLOCK2");
		for (int o : *outs) {
			if (Echo) fprintf(out, "%d ", o);
		}
		writeMessage("BLOCK3");
	}
	void Interpreter::block_type(int_ a) {
		if (!assertInRange(a, blocks.Bs)) return;
		writeMessage("BLOCK_TYPE", *a, blocks.Bs[*a].type.c_str());
	}
	void Interpreter::exec(int_ a, std::string cmd) {
		if (!assertInRange(a, blocks.Bs)) return;
		Interpreter blockInterpreter(blocks.Bs[*a], exepath, path, lang, Echo, out, debugTime, perStep);
		blockInterpreter.command(cmd);
	}
	void Interpreter::tag(int_ a) {
		if (!assertInRange(a, blocks.L)) return;
		std::string I = (count(blocks.inputs.begin(), blocks.inputs.end(), *a) != 0) ? "I" : "-";
		std::string O = (count(blocks.outputs.begin(), blocks.outputs.end(), *a) != 0) ? "O" : "-";
		writeMessage("TAG", *a);
		writeMessage("TAG" + I + O);
	}
	void Interpreter::type(int_ a) {
		if (!assertInRange(a, blocks.L)) return;
		std::string W = (blocks.L[*a].mode == Line::WIDELINE) ? "W" : "L";
		writeMessage("TYPE", *a);
		writeMessage("TYPE" + W);
	}
	void Interpreter::check_input() {
		writeMessage("CHECK_INPUTS");
		for (int i = 0; i < blocks.inputs.size(); i++)
			fprintf(out, "%d ", blocks.inputs[i]);
		fprintf(out, "\n");
	}
	void Interpreter::check_input(int_ a) {
		if (!assertInRange(a, blocks.inputs)) return;
		writeMessage("CHECK_INPUT", *a, blocks.inputs[*a]);
	}
	void Interpreter::check_output() {
		writeMessage("CHECK_OUTPUTS");
		for (int i = 0; i < blocks.outputs.size(); i++)
			fprintf(out, "%d ", blocks.outputs[i]);
		fprintf(out, "\n");
	}
	void Interpreter::check_output(int_ a) {
		if (!assertInRange(a, blocks.outputs)) return;
		writeMessage("CHECK_OUTPUT", *a, blocks.outputs[*a]);
	}
	void Interpreter::del(std::string type, int_ a) {
		if (type == "line") {
			if (!assertInRange(a, blocks.L)) return;
			blocks.L.erase(*a);
		}
		if (type == "N") {
			if (!assertInRange(a, blocks.N)) return;
			blocks.N.erase(*a);
		}
		if (type == "A") {
			if (!assertInRange(a, blocks.A)) return;
			blocks.A.erase(*a);
		}
		if (type == "R") {
			if (!assertInRange(a, blocks.R)) return;
			blocks.R.erase(*a);
		}
		if (type == "T") {
			if (!assertInRange(a, blocks.T)) return;
			blocks.T.erase(*a);
		}
		if (type == "C") {
			if (!assertInRange(a, blocks.C)) return;
			blocks.C.erase(*a);
		}
		if (type == "P") {
			if (!assertInRange(a, blocks.P)) return;
			blocks.P.erase(*a);
		}
		if (type == "block") {
			if (!assertInRange(a, blocks.Bs)) return;
			blocks.Bs.erase(*a);
		}
		writeMessage("DEL", type.c_str(), *a);
	}
	void Interpreter::export__() {
		std::vector<std::string> lines = exportStructure(blocks, path);
		for (std::string line : lines) fprintf(out, "%s\n", filterFileANSI(line).c_str());
	}
	void Interpreter::export_all() {
		std::vector<std::string> lines = exportStructure(blocks, path);
		for (std::string line : lines) fprintf(out, "%s\n", filterFileANSI(line).c_str());
		lines = exportLineData(blocks);
		for (std::string line : lines) fprintf(out, "%s\n", filterFileANSI(line).c_str());
	}
	void Interpreter::qSave() {
		std::string save = exepath + StandardSlash + "quick_load.mcl";
		FILE* fout = fopen(save.c_str(), "w");
		if (fout == NULL) {
			writeError("CANNOT_WRITE", save);
			return;
		}
		for (std::string line : exportStructure(blocks, path)) fprintf(fout, "%s\n", filterFileANSI(line).c_str());
		for (std::string line : exportLineData(blocks)) fprintf(fout, "%s\n", filterFileANSI(line).c_str());
		fclose(fout);
		writeMessage("QSAVE");
	}
	void Interpreter::qLoad() {
		std::string save = exepath + StandardSlash + "quick_load.mcl";
		open(save);
		writeMessage("QLOAD");
	}
	void Interpreter::echo(std::string msg) {
		fprintf(out, (msg + "\n").c_str());
	}
	void Interpreter::_echo(int_ echo) {
		if (!assertBit(echo)) return;
		Echo = *echo;
	}
	void Interpreter::_clock(int_ time) {
		if (!assertBit(time)) return;
		debugTime = *time;
	}
	void Interpreter::_per_step(int_ step) {
		if (!assertBit(step)) return;
		perStep = *step;
	}
	void Interpreter::__path() {
		writeMessage("PATH", path.c_str());
	}
	void Interpreter::__path(std::string p) {
		path = p;
		writeMessage("PATH_SET", path.c_str());
	}
	void Interpreter::clear() {
		clear_screen();
		writeMessage("CLEAR");
		blocks.clear();
	}
	void Interpreter::help() {
		for (std::string l : getHelp()) {
			fprintf(out, (l + "\n").c_str());
		}
	}
	void Interpreter::help(std::string cmd) {
		for (std::string l : getHelp()) {
			if (firstWord(l) == cmd) fprintf(out, (l + "\n").c_str());
		}
	}
	void Interpreter::__lang(std::string lan) {
		if (hasLanguage(lan)) {
			lang = lan;
			writeMessage("LANG", lang.c_str());
		}
		else if (lan == "list") {
			writeMessage("LANG_LIST");
			for (std::string l : getKeys()) {
				fprintf(out, "%s ", l.c_str());
			}
			fprintf(out, "\n");
		}
		writeError("NO_LANG", lang);
	}
	void Interpreter::version() {
		writeMessage("VERSION", to_string(RepoInfo::Version).c_str());
	}
	void Interpreter::neko() {
		writeMessage("NEKO");
	}

	void Interpreter::command(std::string cmdline) {
		message::TimeDebugger timeDebugger;
		timeDebugger.flush();

		normalizeArg(cmdline);
		std::string cmd = cutRedirection(cmdline).first;
		std::vector<std::string> args = breakLine(cmd);

		std::string outfile = cutRedirection(cmdline).second;
		redirect(outfile);

		if (position.has_value()) position->first++;

		if (args.size() == 0) {}
		else if (args[0] == "end" && args.size() == 1) end();
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
		else if (args[0] == "set" && args.size() == 3 && args[2].length() == 4) set(toInt(args[1]), { toInt(args[2][0]), toInt(args[2][1]), toInt(args[2][2]), toInt(args[2][3]) });
		else if (args[0] == "set" && args.size() == 3) set(toInt(args[1]), toInt(args[2]));
		else if (args[0] == "set" && args.size() == 6) set(toInt(args[1]), { toInt(args[2]), toInt(args[3]), toInt(args[4]), toInt(args[5]) });
		else if (args[0] == "input:" && args.size() == 2) input_(toInt(args[1]));
		else if (args[0] == "input" && args.size() == 3 && args[2].length() == 4) input(toInt(args[1]), { toInt(args[2][0]), toInt(args[2][1]), toInt(args[2][2]), toInt(args[2][3]) });
		else if (args[0] == "input" && args.size() == 3) input(toInt(args[1]), toInt(args[2]));
		else if (args[0] == "input" && args.size() == 6) input(toInt(args[1]), { toInt(args[2]), toInt(args[3]), toInt(args[4]), toInt(args[5]) });
		else if (args[0] == "output:" && args.size() == 2) output_(toInt(args[1]));
		else if (args[0] == "output" && args.size() == 1) output();
		else if (args[0] == "output" && args.size() == 2) output(toInt(args[1]));
		else if (args[0] == "tick" && args.size() == 1) tick();
		else if (args[0] == "tick" && args.size() == 2) tick(toInt(args[1]));
		else if (args[0] == "tick!" && args.size() == 1) tick_();
		else if (args[0] == "tick!" && args.size() == 2) tick_(toInt(args[1]));
		else if (args[0] == "speed" && args.size() == 1) speed();
		else if (args[0] == "speed" && args.size() == 2) speed(toInt(args[1]));
		else if (args[0] == "open" && count(cmd.begin(), cmd.end(), '\"') >= 2) open(convertSlash(quotedPart(cmd)));
		else if (args[0] == "open" && args.size() == 2) open(convertSlash(args[1]));
		else if (args[0] == "safe-open" && count(cmd.begin(), cmd.end(), '\"') >= 2) safe_open(convertSlash(quotedPart(cmd)));
		else if (args[0] == "safe-open" && args.size() == 2) safe_open(convertSlash(args[1]));
		else if (args[0] == "mod" && args.size() == 3) mod(args[1], args[2]);
		else if (args[0] == "check-mods" && args.size() == 1) check_mods();
		else if (args[0] == "block" && args.size() >= 2) block(args[1], toInt(subVec(args, 2, (int)args.size())));
		else if (args[0] == "block-type" && args.size() == 2) block_type(toInt(args[1]));
		else if (args[0] == "exec" && args.size() >= 2) exec(toInt(args[1]), subCommand(args, 2));
		else if (args[0] == "tag" && args.size() == 2) tag(toInt(args[1]));
		else if (args[0] == "type" && args.size() == 2) type(toInt(args[1]));
		else if (args[0] == "check-input" && args.size() == 1) check_input();
		else if (args[0] == "check-input" && args.size() == 2) check_input(toInt(args[1]));
		else if (args[0] == "check-output" && args.size() == 1) check_output();
		else if (args[0] == "check-output" && args.size() == 2) check_output(toInt(args[1]));
		else if (args[0] == "del" && args.size() == 3) del(args[1], toInt(args[2]));
		else if (args[0] == "export" && args.size() == 1) export__();
		else if (args[0] == "export-all" && args.size() == 1) export_all();
		else if (args[0] == "qsave" && args.size() == 1) qSave();
		else if (args[0] == "qload" && args.size() == 1) qLoad();
		else if (args[0] == "echo" && args.size() > 1) echo(cmd.substr(5, cmd.size()));
		else if (args[0] == "@echo" && args.size() == 2) _echo(toInt(args[1]));
		else if (args[0] == "@clock" && args.size() == 2) _clock(toInt(args[1]));
		else if (args[0] == "@per-step" && args.size() == 2) _per_step(toInt(args[1]));
		else if (args[0] == "path" && args.size() == 1) __path();
		else if (args[0] == "path" && count(cmd.begin(), cmd.end(), '\"') >= 2) __path(addSlash(convertSlash(quotedPart(cmd))));
		else if (args[0] == "path" && args.size() == 2) __path(addSlash(args[1]));
		else if (args[0] == "clear") clear();
		else if (args[0] == "help" && args.size() == 1) help();
		else if (args[0] == "help" && args.size() == 2) help(args[1]);
		else if (args[0] == "lang" && args.size() == 2) __lang(args[1]);
		else if (args[0] == "version" && args.size() == 1) version();
		else if (args[0] == "neko" && args.size() == 1) neko();
		else writeError("NO_CMD", cmd);
		writeDebug();
		if (debugTime) timeDebugger.debug();
		if (out != stdout && out != defaultOut) {
			fclose(out);
		}
	}

	void Interpreter::writeDebug() {
		FILE* file = _fopen((exepath + StandardSlash + "debug.log").c_str(), "w");
		if (file == nullptr) {
			writeError("CANNOT_WRITE", "debug.log");
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
		std::string f = exepath + StandardSlash + "help.txt";
		fin.open(f, std::ios::in);
		if (!assertGoodFile(fin, f)) return { "" };
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
			vfprintf(out, getMessage(lang, message).c_str(), args);
			va_end(args);
		}
	}

	void Interpreter::checkUpdate() {
		if (std::filesystem::exists(std::filesystem::path(exepath) / ".no_update_check")) return;
		auto webErrorHandler = [this](const std::string& owner, const std::string& repo) {
			writeError("UPDATE_GET", owner, repo);
			};
		auto jsonErrorHandler = [this](const std::string& owner, const std::string& repo) {
			writeError("UPDATE_PARSE");
			};
		auto timeoutHandler = [this](const std::string& owner, const std::string& repo) {
			writeError("TIMEOUT");
			};
		std::optional<std::string> latest = UpdateChecker::getLatestReleaseName(RepoInfo::Author, RepoInfo::Name,
			webErrorHandler, jsonErrorHandler);
		if (latest != std::nullopt) {
			if (RepoInfo::Version != VersionInfo(*latest))
				writeMessage("NEW_VER", to_string(VersionInfo(*latest)).c_str(),
					RepoInfo::Author.c_str(), RepoInfo::Name.c_str(), (exepath + StandardSlash).c_str());
		}
	}

	void Interpreter::redirect(std::string outfile) {
		out = defaultOut;
		if (outfile == "stdout") out = stdout;
		else if (outfile != "") {
			FILE* fout = fopen(outfile.c_str(), "a");
			if (fout == NULL) writeError("CANNOT_WRITE", outfile);
			else out = fout;
		}
	}

	void SubInterpreter::unavailableMessage(std::string cmd) {
		writeError("SUB_MODE", cmd);
	}

	void SafeInterpreter::unavailableMessage(std::string cmd) {
		writeError("SAFE_MODE", cmd);
	}

	void SafeInterpreter::redirect(std::string outfile) {
		out = defaultOut;
		if (outfile == "stdout") out = stdout;
		else if (outfile != "") writeError("SAFE_REDIRECT");
	}
}