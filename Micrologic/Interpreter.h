#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <variant>
#include <array>
#include <cstdarg>
#include <direct.h>
#include "Blocks.h"
#include "Messages.h"

inline bool assertPositive(int a) {
	if (a <= 0) {
		ErrorMsg() << "Should be positive: " << a;
		return false;
	}
	return true;
}
inline bool assertBit(int a) {
	if (a != 0 && a != 1) {
		ErrorMsg() << "Should be bit: " << a;
		return false;
	}
	return true;
}
template <typename T>
inline bool assertInRange(int i, std::vector<T> vec) {
	if (i < 0 || i >= vec.size()) {
		ErrorMsg() << "Number out of range: " << i;
		return false;
	}
	return true;
}
template <typename T, typename T1>
inline bool assertInMap(T t, std::map<T, T1> mp) {
	if (mp.find(t) == mp.end()) {
		ErrorMsg() << "Key not found: " << t;
		return false;
	}
	return true;
}
inline bool assertGoodFile(std::ifstream& f) {
	if (!f.good()) {
		ErrorMsg() << "File not found";
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

class Interpreter {
public:
	bool Echo;
	bool Exit = false;
	std::string lang;
	Blocks& blocks;
	std::string path;
	std::string exepath;

	Interpreter(Blocks& blocks, std::string exepath, std::string path = "", std::string lang = "en_us", bool Echo = true)
		:blocks(blocks), exepath(exepath), path(path), lang(lang), Echo(Echo) {
	}

	bool isNum(std::string);
	int toInt(std::string);
	std::vector<int> toInt(std::vector<std::string>);
	std::array<bool, 4> toBoolArray(std::array<int, 4>);

	std::string quotedPart(std::string);
	std::string firstWord(std::string);
	std::string pathPart(std::string);
	std::string addSlash(std::string filename); //add backslash to the end if none
	std::string convertSlash(std::string filename); //convert all slashes to backslashes

	std::vector<std::string> breakLine(std::string);

	void writeDebug();
	std::vector<std::string> getHelp();
	void writeMessage(std::string message, ...);

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
	virtual void open(std::string);
	virtual void safe_open(std::string);
	virtual void mod(std::string, std::string);
	virtual void block(std::string, std::vector<int>);
	virtual void tag(int);
	virtual void type(int);
	virtual void check_input();
	virtual void check_input(int);
	virtual void check_output();
	virtual void check_output(int);
	virtual void inspect(std::string, int);
	virtual void echo(std::string);
	virtual void _echo(int);
	virtual void __path();
	virtual void __path(std::string);
	virtual void clear();
	virtual void help();
	virtual void help(std::string);
	virtual void __lang(std::string);
	virtual void neko();

	bool command(std::string cmd);
};

class SubInterpreter :public Interpreter {
public:
	using Interpreter::Interpreter;
	SubInterpreter(const Interpreter&);

	void unavailable(std::string);
	inline void check() { unavailable("check"); }
	inline void check(int) { unavailable("check"); }
	inline void output() { unavailable("output"); }
	inline void output(int) { unavailable("output"); }
	inline void tick() { unavailable("tick"); }
	inline void tick(int) { unavailable("tick"); }
	inline void tick_() { unavailable("tick!"); }
	inline void tick_(int) { unavailable("tick!"); }
	inline void tag(int) { unavailable("tag"); }
	inline void type(int) { unavailable("type"); }
	inline void check_input() { unavailable("check-input"); }
	inline void check_input(int) { unavailable("check-input"); }
	inline void check_output() { unavailable("check-input"); }
	inline void check_output(int) { unavailable("check-output"); }
	inline void inspect(std::string, int) { unavailable("check-output"); }
	inline void __path() { unavailable("path"); }
	inline void __path(std::string) { unavailable("path"); }
	inline void clear() { unavailable("clear"); }
	inline void help() { unavailable("help"); }
	inline void help(std::string) { unavailable("help"); }
	inline void __lang(std::string) { unavailable("lang"); }
};