#pragma once
// Use NT option to prevent error C2065: 'WH_KEYBOARD_LL' : undeclared identifier
#define _WIN32_WINNT 0x0500 
#include <windows.h>

// //Auto hook when loading
extern "C"  __declspec(dllexport) bool GetStopPlay();
extern "C"  __declspec(dllexport) bool HookAll();
extern "C"  __declspec(dllexport) void UnhookAll();
