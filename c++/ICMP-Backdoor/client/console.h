#pragma once

#include <Windows.h>

class GohanConsoleHelper

{
	
    HANDLE _hIn;
	
    HANDLE _hOut;
	
    INPUT_RECORD _InRec;
	
    DWORD _NumRead;
	
public:
	
    WORD VKey;
	
    GohanConsoleHelper(void){
		
        _hIn = GetStdHandle(STD_INPUT_HANDLE);
		
        _hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		
        VKey=0;
		
    }
	
    bool ReadOneInput()
		
    {
		
        return 0!=ReadConsoleInput(_hIn,&_InRec,1,&_NumRead);
		
    }
	
    bool ReadOneInput(INPUT_RECORD& InRec)
		
    {
		
        return 0!=ReadConsoleInput(_hIn,&InRec,1,&_NumRead);
		
    }
	
    DWORD ReadKeyDown()
		
    {
		
        if(!ReadConsoleInput(_hIn,&_InRec,1,&_NumRead))
			
            return 0;
		
        if(_InRec.EventType!=KEY_EVENT)
			
            return 0;
		
        if(_InRec.Event.KeyEvent.bKeyDown > 0)
			
            return 0;
		
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;
		
        return VKey;
		
    }
	
    DWORD ReadKeyPush()
		
    {
		
        if(!ReadConsoleInput(_hIn,&_InRec,1,&_NumRead))
			
            return 0;
		
        if(_InRec.EventType!=KEY_EVENT)
			
            return 0;
		
        if(_InRec.Event.KeyEvent.bKeyDown == 0)
			
            return 0;
		
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;
		
        return VKey;
		
    }
	
public:
	
    ~GohanConsoleHelper(void){}
	
};