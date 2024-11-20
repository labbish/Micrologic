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
#include <optional>
#include "Blocks.h"
#include "Messages.h"
#include "TimeDebugger.h"
#include "SystemAbstraction.h"

namespace labbish {
	namespace Micrologic {
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

		inline bool assertPositive(std::optional<int> a) {
			if (a == std::nullopt) return false;
			if (a <= 0) {
				writeError("NOT_POSITIVE", *a);
				return false;
			}
			return true;
		}
		inline bool assertBit(std::optional<int> a) {
			if (a == std::nullopt) return false;
			if (a != 0 && a != 1) {
				writeError("NOT_BIT", *a);
				return false;
			}
			return true;
		}
		template <typename T>
		inline bool assertInRange(std::optional<int> i, std::vector<T> vec) {
			if (i == std::nullopt) return false;
			if (i < 0 || i >= vec.size()) {
				writeError("OUT_OF_RANGE", *i);
				return false;
			}
			return true;
		}
		template <typename T>
		inline bool assertInRange(std::optional<int> i, StableVector<T> vec) {
			if (i == std::nullopt) return false;
			if (i < 0 || i >= vec.size()) {
				writeError("OUT_OF_RANGE", *i);
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
			std::optional<int> toInt(std::string);
			std::vector<std::optional<int>> toInt(std::vector<std::string>);
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

			virtual void line(std::optional<int> = 1);
			virtual void wline(std::optional<int> = 1);
			virtual void N(std::optional<int>, std::optional<int>);
			virtual void A(std::optional<int>, std::optional<int>, std::optional<int>);
			virtual void R(std::optional<int>, std::optional<int>, std::optional<int>);
			virtual void T(std::optional<int>, std::optional<int>);
			virtual void C(std::array<std::optional<int>, 4>, std::optional<int>);
			virtual void P(std::optional<int>, std::array<std::optional<int>, 4>);
			virtual void check();
			virtual void check(std::optional<int>);
			virtual void set(std::optional<int>, std::optional<int>);
			virtual void set(std::optional<int>, std::array<std::optional<int>, 4>);
			virtual void input_(std::optional<int>);
			virtual void input(std::optional<int>, std::optional<int>);
			virtual void input(std::optional<int>, std::array<std::optional<int>, 4>);
			virtual void output_(std::optional<int>);
			virtual void output();
			virtual void output(std::optional<int>);
			virtual void tick();
			virtual void tick(std::optional<int>);
			virtual void tick_();
			virtual void tick_(std::optional<int>);
			virtual void speed(std::optional<int>);
			virtual void openInterface(std::string, Interpreter*);
			virtual void open(std::string);
			virtual void safe_open(std::string);
			virtual void mod(std::string, std::string);
			virtual void check_mods();
			virtual void block(std::string, std::vector<std::optional<int>>);
			virtual void block_type(std::optional<int>);
			virtual void exec(std::optional<int>, std::string);
			virtual void tag(std::optional<int>);
			virtual void type(std::optional<int>);
			virtual void check_input();
			virtual void check_input(std::optional<int>);
			virtual void check_output();
			virtual void check_output(std::optional<int>);
			virtual void inspect(std::string, std::optional<int>);
			virtual void del(std::string, std::optional<int>);
			virtual void export__();
			virtual void echo(std::string);
			virtual void _echo(std::optional<int>);
			virtual void _clock(std::optional<int>);
			virtual void _per_step(std::optional<int>);
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
			inline void check(std::optional<int>) { unavailableMessage("check"); }
			inline void output() { unavailableMessage("output"); }
			inline void output(std::optional<int>) { unavailableMessage("output"); }
			inline void tick() { unavailableMessage("tick"); }
			inline void tick(std::optional<int>) { unavailableMessage("tick"); }
			inline void tick_() { unavailableMessage("tick!"); }
			inline void tick_(std::optional<int>) { unavailableMessage("tick!"); }
			inline void check_mods() { unavailableMessage("check-mods"); }
			inline void tag(std::optional<int>) { unavailableMessage("tag"); }
			inline void type(std::optional<int>) { unavailableMessage("type"); }
			inline void check_input() { unavailableMessage("check-input"); }
			inline void check_input(std::optional<int>) { unavailableMessage("check-input"); }
			inline void check_output() { unavailableMessage("check-output"); }
			inline void check_output(std::optional<int>) { unavailableMessage("check-output"); }
			inline void inspect(std::string, std::optional<int>) { unavailableMessage("inspect"); }
			inline void del() { unavailableMessage("del"); }
			inline void export__() { unavailableMessage("export"); }
			inline void _clock(std::optional<int>) { unavailableMessage("@clock"); }
			inline void _per_step(std::optional<int>) { unavailableMessage("@per-step"); }
			inline void __path() { unavailableMessage("path"); }
			inline void __path(std::string) { unavailableMessage("path"); }
			inline void clear() { unavailableMessage("clear"); }
			inline void help() { unavailableMessage("help"); }
			inline void help(std::string) { unavailableMessage("help"); }
			inline void __lang(std::string) { unavailableMessage("lang"); }
		};
	}
}