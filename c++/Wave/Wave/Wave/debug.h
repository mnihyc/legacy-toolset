#pragma once

#include "def.h"

#define NO_DEBUG 0x00000000
#define ERROR_DEBUG 0x00000001
#define WARNING_DEBUG 0x00000002
#define INFO_DEBUG 0x00000004
#define ALL_DEBUG ERROR_DEBUG|WARNING_DEBUG|INFO_DEBUG

UINT DebugLevel = ERROR_DEBUG;

#define CheckDebugPriv(s) (DebugLevel&s)

//For easy
#define COLOR_BLUE (FOREGROUND_BLUE)
#define COLOR_RED (FOREGROUND_RED)
#define COLOR_GREEN (FOREGROUND_GREEN)
#define COLOR_WHITE (COLOR_BLUE|COLOR_RED|COLOR_GREEN)

inline void ChangeColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|color);
	SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), FOREGROUND_INTENSITY|color);
}

namespace DEBUG{
	inline void err(string st="")
	{
		if (!CheckDebugPriv(ERROR_DEBUG))return;
		ChangeColor(COLOR_RED); cout << "[-]"; ChangeColor(COLOR_WHITE);
		cout << st << endl;
		getch();
		exit(666);
	}
	inline void wrn(string s="")
	{
		if (!CheckDebugPriv(WARNING_DEBUG))return;
		ChangeColor(COLOR_RED|COLOR_GREEN); cout << "[!]"; ChangeColor(COLOR_WHITE);
		cout << s << endl;
	}
	inline void info(string s = "")
	{
		if (!CheckDebugPriv(INFO_DEBUG))return;
		cout << "[*]";
		cout << s << endl;
	}
}