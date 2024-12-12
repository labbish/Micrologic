#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <variant>
#include <array>
#include <cstdarg>
#include <filesystem>
#include <regex>
#include "Blocks.h"
#include "Messages.h"
#include "RepoInfo.h"
#include "TimeDebugger.h"
#include "UpdateChecker.h"
#include "SystemAbstraction.h"

namespace labbish::Micrologic {
	template <typename T, size_t size>
	inline std::array<T, size> operator*(std::array<std::optional<T>, size> arr) {
		std::array<T, size> ans;
		for (int i = 0; i < size; i++) {
			ans[i] = *arr[i];
		}
		return ans;
	}
	template <typename T>
	inline std::vector<T> operator*(std::vector<std::optional<T>> vec) {
		std::vector<T> ans;
		for (int i = 0; i < vec.size(); i++) {
			ans.push_back(*vec[i]);
		}
		return ans;
	}

	template <typename T>
	inline std::vector<T> subVec(const std::vector<T>& vec, int start, int end) {
		if (start < 0 || end > vec.size() || start > end) {
			throw std::out_of_range("");
		}
		return std::vector<T>(vec.begin() + start, vec.begin() + end);
	}

	inline void clearCurrentLine() {
		std::cout << "\r";
		std::cout << std::string(128, ' ');
		std::cout << "\r";
	}
	inline void pause() {
		printf("Click any key to continue...");
		(void)_getch();
		clearCurrentLine();
	}

	std::vector<std::string> exportStructure(const Blocks&, std::optional<std::string> path = std::nullopt);
	std::vector<std::string> exportLineData(const Blocks&);

	class Interpreter {
	public:
		using Position = std::optional<std::pair<int, std::string>>; //(line count, file name)

		bool Echo;
		bool Exit = false;
		std::string lang;
		Blocks& blocks;
		std::string path;
		std::string exepath;
		bool debugTime;
		bool perStep;
		FILE* out;
		FILE* defaultOut;
		Position position;
		std::optional<VersionInfo> latest = std::nullopt; //nullopt if already newest or has already show message
		std::optional<std::string> latestContent = std::nullopt;
		const std::string QuickSaveDirectory = "quick_save";

		Interpreter(Blocks& blocks, std::string exepath, std::string path = "",
			std::string lang = "en_us", bool Echo = true, FILE* defaultOut = stdout,
			bool debugTime = false, bool perStep = false, Position position = std::nullopt)
			:blocks(blocks), exepath(exepath), path(path), lang(lang), Echo(Echo), debugTime(debugTime),
			perStep(perStep), defaultOut(defaultOut), out(defaultOut), position(position) {
		}

		inline const std::string filterANSI(std::string str) {
			std::string result;
			bool escape = false;
			for (char c : str) {
				if (c == '\033') {
					escape = true;
				}
				else if (escape && c == 'm') {
					escape = false;
					continue;
				}
				if (!escape) {
					result += c;
				}
			}
			return result;
		} //erase all ANSI escape code
		inline const std::string filterFileANSI(std::string str) {
			if (out != stdout) return filterANSI(str);
			else return str;
		}

		template<typename... Args>
		inline const void writeError(std::string error, Args... args) {
			if (position.has_value())
				message::ErrorMsg::no_prefix() << std::format("At line {}, file \"{}\"", position->first, position->second);
			Micrologic::writeError(error, args...);
		}

		inline bool assertPositive(int_ a) {
			if (a == std::nullopt) return false;
			if (a <= 0) {
				writeError("NOT_POSITIVE", *a);
				return false;
			}
			return true;
		}
		inline bool assertBit(int_ a) {
			if (a == std::nullopt) return false;
			if (a != 0 && a != 1) {
				writeError("NOT_BIT", *a);
				return false;
			}
			return true;
		}
		inline bool assertInRange(int_ i, size_t min, size_t max) {
			if (i == std::nullopt) return false;
			if (i < min && i > max) {
				writeError("OUT_OF_RANGE", *i);
				return false;
			}
			return true;
		}
		template <typename vecT>
		inline bool assertInRange(int_ i, vecT vec) {
			return assertInRange(i, 0, vec.size() - 1);
		}
		template <typename T, typename T1>
		inline bool assertInMap(T t, std::map<T, T1> mp) {
			if (mp.find(t) == mp.end()) {
				writeError("NO_KEY", t);
				return false;
			}
			return true;
		}
		inline bool assertGoodFile(std::ifstream& fin, const std::string& filename) {
			if (!fin.good()) {
				writeError("NO_FILE", filename);
				return false;
			}
			return true;
		}
		inline bool assertLineType(int_ i, Line::TYPE type) {
			if (i == std::nullopt) return false;
			const Line& l = blocks.L[*i];
			if (l.mode != type) {
				writeError("LINE_TYPE", *i, to_string(type));
				return false;
			}
			return true;
		}
		inline bool assertSameLineType(int_ i, int_ j) {
			if (i == std::nullopt || j == std::nullopt) return false;
			if (blocks.L[*i].mode != blocks.L[*j].mode) {
				writeError("SAME_TYPE", *i, *j);
				return false;
			}
			return true;
		}

		bool isdirty(char);
		bool isspace(char);
		void normalizeArg(std::string&);
		void normalizeArgs(std::vector<std::string>&);
		bool isNum(std::string);
		int_ toInt(char);
		int_ toInt(std::string);
		std::vector<int_> toInt(std::vector<std::string>);
		std::array<bool, 4> toBoolArray(std::array<int, 4>);

		std::string quotedPart(std::string);
		std::string firstWord(std::string);
		std::string pathPart(std::string);
		std::string addSlash(std::string filename); //add slash to the end if none
		std::string convertSlash(std::string filename); //convert slashes
		std::string subCommand(std::vector<std::string> cmd, size_t pos = 0, size_t len = -1);
		std::pair<std::string, std::string> cutRedirection(std::string); //cut "command>file" to ("command","file")
		std::string trim(std::string); //trim extra space and quotation marks
		std::vector<std::string> breakLine(std::string);
		std::string combineLine(std::vector<std::string>);

		void writeDebug();
		std::vector<std::string> getHelp();
		void writeMessage(std::string message, ...);
		void writeConsoleMessage(std::string message, ...);

		void checkUpdate();
		std::string formatUpdateContent(std::string);
		void showUpdateMessage(); //Note: assumes that latest is not nullopt
		void showUpdateContent();
		void flushUpdateMessage();

		virtual void redirect(std::string outfile);

		virtual void end();
		virtual void line(int_ = 1);
		virtual void wline(int_ = 1);
		virtual void N(int_, int_);
		virtual void A(int_, int_, int_);
		virtual void R(int_, int_, int_);
		virtual void T(int_, int_);
		virtual void C(std::array<int_, 4>, int_);
		virtual void P(int_, std::array<int_, 4>);
		virtual void check();
		virtual void check(int_);
		virtual void set(int_, int_);
		virtual void set(int_, std::array<int_, 4>);
		virtual void input_(int_);
		virtual void input(int_, int_);
		virtual void input(int_, std::array<int_, 4>);
		virtual void output_(int_);
		virtual void output();
		virtual void output(int_);
		virtual void tick();
		virtual void tick(int_);
		virtual void tick_();
		virtual void tick_(int_);
		virtual void speed();
		virtual void speed(int_);
		virtual void openInterface(std::string, Interpreter*);
		virtual void open(std::string);
		virtual void safe_open(std::string);
		virtual void mod(std::string, std::string);
		virtual void check_mods();
		virtual void block(std::string, std::vector<int_>);
		virtual void block_type(int_);
		virtual void exec(int_, std::string);
		virtual void tag(int_);
		virtual void type(int_);
		virtual void check_input();
		virtual void check_input(int_);
		virtual void check_output();
		virtual void check_output(int_);
		virtual void del(std::string, int_);
		virtual void export__();
		virtual void export_all();
		virtual void qSave(int_ = 0);
		virtual void qLoad(int_ = 0);
		virtual void echo(std::string);
		virtual void _echo(int_);
		virtual void _clock(int_);
		virtual void _per_step(int_);
		virtual void __path();
		virtual void __path(std::string);
		virtual void clear();
		virtual void help();
		virtual void help(std::string);
		virtual void __lang(std::string);
		virtual void version();
		virtual void neko();

		void command(std::string cmd);
	};

	class SubInterpreter :public Interpreter {
	public:
		using Interpreter::Interpreter;
		SubInterpreter(const Interpreter& father) :Interpreter(father) {}

		void unavailableMessage(std::string);
		inline void qSave(int_) override { unavailableMessage("qsave"); }
		inline void qLoad(int_) override { unavailableMessage("qload"); }
	};

	class SafeInterpreter :public SubInterpreter {
	public:
		using SubInterpreter::SubInterpreter;
		SafeInterpreter(const Interpreter& father) :SubInterpreter(father) {}

		inline void open(std::string f) {
			safe_open(f);
		}
		void redirect(std::string outfile);

		void unavailableMessage(std::string);
		inline void end() override { unavailableMessage("end"); }
		inline void check() override { unavailableMessage("check"); }
		inline void check(int_) override { unavailableMessage("check"); }
		inline void output() override { unavailableMessage("output"); }
		inline void output(int_) override { unavailableMessage("output"); }
		inline void tick() override { unavailableMessage("tick"); }
		inline void tick(int_) override { unavailableMessage("tick"); }
		inline void tick_() override { unavailableMessage("tick!"); }
		inline void tick_(int_) override { unavailableMessage("tick!"); }
		inline void speed() override { unavailableMessage("speed"); }
		inline void check_mods() override { unavailableMessage("check-mods"); }
		inline void block_type(int_) override { unavailableMessage("block-type"); }
		inline void exec(int_, std::string) override { unavailableMessage("exec"); }
		inline void tag(int_) override { unavailableMessage("tag"); }
		inline void type(int_) override { unavailableMessage("type"); }
		inline void check_input() override { unavailableMessage("check-input"); }
		inline void check_input(int_) override { unavailableMessage("check-input"); }
		inline void check_output() override { unavailableMessage("check-output"); }
		inline void check_output(int_) override { unavailableMessage("check-output"); }
		inline void del(std::string, int_) override { unavailableMessage("del"); }
		inline void export__() override { unavailableMessage("export"); }
		inline void export_all() override { unavailableMessage("export-all"); }
		inline void _clock(int_) override { unavailableMessage("@clock"); }
		inline void _per_step(int_) override { unavailableMessage("@per-step"); }
		inline void __path() override { unavailableMessage("path"); }
		inline void __path(std::string) override { unavailableMessage("path"); }
		inline void clear() override { unavailableMessage("clear"); }
		inline void help() override { unavailableMessage("help"); }
		inline void help(std::string) override { unavailableMessage("help"); }
		inline void __lang(std::string) override { unavailableMessage("lang"); }
		inline void version() override { unavailableMessage("version"); }
		inline void neko() override {}
	};
}