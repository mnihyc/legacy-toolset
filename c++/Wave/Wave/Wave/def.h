#pragma once

#include <cstdio>
#include <iostream>
#include <string>
using namespace std;
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <cmath>
//#include <fstream>//Too slow


#define VERSION "DEBUG 0.0.1"
#define PACK_FILEEXT "wavePACK"

#define COMMAND_NOFOUND 0x0FFF
#define COMMAND_PACK 0x0001
#define COMMAND_UNPACK 0x0002
#define COMMAND_DRAW_PICTURE 0x0003
#define COMMAND_PLAY_WAVE 0x0004
#define COMMAND_CONVERT_TO8BIT 0x0005
#define COMMAND_DEBUG_OUTSOUND 0x0006
#define COMMAND_ADDVOLUME 0x0007

#define StrEqu(a,b) (strcmp(a,b)==0)
#define StrLowEqu(a,b) (stricmp(a,b)==0)

union
{
	short j;
	struct
	{
		BYTE HighPart;
		BYTE LowPart;
	}Part;
}HLPGet;
inline short HighPart(short p)
{
	HLPGet.j = p;
	return HLPGet.Part.HighPart;
}
inline short LowPart(short p)
{
	HLPGet.j = p;
	return HLPGet.Part.LowPart;
}
inline short MakeByPart(short high,short low)
{
	HLPGet.Part.HighPart = high;
	HLPGet.Part.LowPart = low;
	return HLPGet.j;
}