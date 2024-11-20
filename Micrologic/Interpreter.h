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
#include "Blocks.h"
#include "Messages.h"
#include "TimeDebugger.h"
#include "SystemAbstraction.h"

namespace labbish {
	namespace Micrologic {
		const int NOT_NUM = 0xDEADCAFE; //Magic~

		inline bool assertPositive(int a) {
			if (a == NOT_NUM) return false;
			if (a <= 0) {
				writeError("NOT_POSITIVE", a);
				return false;
			}
			return true;
		}
		inline bool assertBit(int a) {
			if (a == NOT_NUM) return false;
			if (a != 0 && a != 1) {
				writeError("NOT_BIT", a);
				return false;
			}
			return true;
		}
		template <typename T>
		inline bool assertInRange(int i, std::vector<T> vec) {
			if (i == NOT_NUM) return false;
			if (i < 0 || i >= vec.size()) {
				writeError("OUT_OF_RANGE", i);
				return false;
			}
			return true;
		}
		template <typename T>
		inline bool assertInRange(int i, StableVector<T> vec) {
			if (i == NOT_NUM) return false;
			if (i < 0 || i >= vec.size()) {
				writeError("OUT_OF_RANGE", i);
				return false;
			}
			return true;
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
			_getch();
			clearCurrentLine();
		}

		class Interpreter {
		public:
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

			Interpreter(Blocks& blocks, std::string exepath, std::string path = "", std::string lang = "en_us", bool Echo = true, FILE* defaultOut = stdout, bool debugTime = false, bool perStep = false)
				:blocks(blocks), exepath(exepath), path(path), lang(lang), Echo(Echo), debugTime(debugTime), perStep(perStep), defaultOut(defaultOut), out(defaultOut) {
			}

			void normalizeArg(std::string&);
			void normalizeArgs(std::vector<std::string>&);
			bool isNum(std::string);
			int toInt(std::string);
			std::vector<int> toInt(std::vector<std::string>);
			std::array<bool, 4> toBoolArray(std::array<int, 4>);

			std::string quotedPart(std::string);
			std::string firstWord(std::string);
			std::string pathPart(std::string);
			std::string addSlash(std::string filename); //add slash to the end if none
			std::string convertSlash(std::string filename); //convert slashes
			std::string subCommand(std::vector<std::string> cmd, size_t pos = 0, size_t len = -1);
			std::pair<std::string, std::string> cutRedirection(std::string); //cut "command>file" to ("command","file")
			std::vector<std::string> breakLine(std::string);
			std::string combineLine(std::vector<std::string>);

			void writeDebug();
			std::vector<std::string> getHelp();
			void writeMessage(std::string message, ...);

			virtual void redirect(std::string outfile);

			virtual void line(int = 1);
			virtual void wline(int = 1);
			virtual void N(int, int);
			virtual void A(int, int, int);
			virtual void R(int, int, int);
			virtual void T(int, int);
			virtual void C(std::array<int, 4>, int);
			virtual void P(int, std::array<int, 4>);
			virtual void check();
			virtual void check(int);
			virtual void set(int, int);
			virtual void set(int, std::array<int, 4>);
			virtual void input_(int);
			virtual void input(int, int);
			virtual void input(int, std::array<int, 4>);
			virtual void output_(int);
			virtual void output();
			virtual void output(int);
			virtual void tick();
			virtual void tick(int);
			virtual void tick_();
			virtual void tick_(int);
			virtual void speed(int);
			virtual void openInterface(std::string, Interpreter*);
			virtual void open(std::string);
			virtual void safe_open(std::string);
			virtual void mod(std::string, std::string);
			virtual void check_mods();
			virtual void block(std::string, std::vector<int>);
			virtual void block_type(int);
			virtual void exec(int, std::string);
			virtual void tag(int);
			virtual void type(int);
			virtual void check_input();
			virtual void check_input(int);
			virtual void check_output();
			virtual void check_output(int);
			virtual void inspect(std::string, int);
			virtual void del(std::string, int);
			virtual void export__();
			virtual void echo(std::string);
			virtual void _echo(int);
			virtual void _clock(int);
			virtual void _per_step(int);
			virtual void __path();
			virtual void __path(std::string);
			virtual void clear();
			virtual void help();
			virtual void help(std::string);
			virtual void __lang(std::string);
			virtual void neko();

			bool command(std::string cmd);
		};

		class SafeInterpreter :public Interpreter {
		public:
			using Interpreter::Interpreter;
			SafeInterpreter(const Interpreter&);

			inline void open(std::string f) {
				safe_open(f);
			}
			void redirect(std::string outfile);

			void unavailableMessage(std::string);
			inline void end() { unavailableMessage("end"); }
			inline void check() { unavailableMessage("check"); }
			inline void check(int) { unavailableMessage("check"); }
			inline void output() { unavailableMessage("output"); }
			inline void output(int) { unavailableMessage("output"); }
			inline void tick() { unavailableMessage("tick"); }
			inline void tick(int) { unavailableMessage("tick"); }
			inline void tick_() { unavailableMessage("tick!"); }
			inline void tick_(int) { unavailableMessage("tick!"); }
			inline void check_mods() { unavailableMessage("check-mods"); }
			inline void tag(int) { unavailableMessage("tag"); }
			inline void type(int) { unavailableMessage("type"); }
			inline void check_input() { unavailableMessage("check-input"); }
			inline void check_input(int) { unavailableMessage("check-input"); }
			inline void check_output() { unavailableMessage("check-output"); }
			inline void check_output(int) { unavailableMessage("check-output"); }
			inline void inspect(std::string, int) { unavailableMessage("inspect"); }
			inline void del() { unavailableMessage("del"); }
			inline void export__() { unavailableMessage("export"); }
			inline void _clock(int) { unavailableMessage("@clock"); }
			inline void _per_step(int) { unavailableMessage("@per-step"); }
			inline void __path() { unavailableMessage("path"); }
			inline void __path(std::string) { unavailableMessage("path"); }
			inline void clear() { unavailableMessage("clear"); }
			inline void help() { unavailableMessage("help"); }
			inline void help(std::string) { unavailableMessage("help"); }
			inline void __lang(std::string) { unavailableMessage("lang"); }
		};
	}
}