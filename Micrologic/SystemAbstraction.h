#pragma once

#include <string>

#ifdef _WIN32

//Windows
#include <conio.h>
#include <direct.h>
#include <Windows.h>
const char Slash[] = "/";
const char StandardSlash[] = "\\";
inline void SetEncoding() {
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, "");
}
#define _fopen(_filename, _mode) _fsopen(_filename, _mode, _SH_DENYNO)
inline std::string read_command() {
	printf(">>>");
	std::string cmd;
	std::getline(std::cin, cmd);
	return cmd;
}
#define clear_screen() system("cls")

#else

//Linux
#include <ncurses.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define _getch getch
#define _getcwd getcwd
const char Slash[] = "/";
const char StandardSlash[] = "/";
inline void SetEncoding() {
	setlocale(LC_ALL, "");
}
#define _fopen(_filename, _mode) fopen(_filename, _mode)
inline std::string read_command() {
	std::string cmd = readline(">>>");
	add_history(cmd.c_str());
	return cmd;
}
#define clear_screen() system("clear")

#endif // _WIN32
