#pragma once

#ifdef _WIN32

//Windows
#include <conio.h>
#include <direct.h>
#include <Windows.h>
const char Slash[] = "/";
const char StandardSlash[] = "\\";
inline void SetEncoding() {
	SetConsoleOutputCP(CP_UTF8);
}
#define _fopen(_filename, _mode) _fsopen(_filename, _mode, _SH_DENYNO)

#else

//Linux
#include <ncurses.h>
#include <unistd.h>
#define _getch getch
#define _getcwd getcwd
const char Slash[] = "/";
const char StandardSlash[] = "/";
inline void SetEncoding() {}
#define _fopen(_filename, _mode) fopen(_filename, _mode)

#endif // _WIN32
