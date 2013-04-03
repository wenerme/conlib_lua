/*
 * Conlib v1.2
 * Author: Wener
 * E-mail: wenermail@gmail.com
 * Site: http://blog.wener.me
 *
 * һ��windows�¿���̨��̵ĸ�����
 *
 */
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>

#include "conlib.h"

#ifndef conlib_c
#define conlib_c

/* ����ϵͳû�ж������ */
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
/* Ԥ���� */
#define DEFAULT_CONSOLE_MODE (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)

/* �ڲ����� */
static void conlib_close();
static void update_ctrlkey(unsigned state); 
// ����console ģʽ��ʱ��ı� ����ȷ�������������ģʽ
static void mouse_mode_active();
/* ȫ�־�� */ 
static HANDLE hConsoleIn;
static HANDLE hConsoleOut;

/* ����������� */
static ConMouse cMouse; 
static ConKey cKey;
static ConControlKeyState cCtrlKeyState; 
/* ��Ϊ���������ֻ򱳾���ʱ��Ҫ������ֵ */
static WORD wTextColor;
static WORD wBackgroundColor;

static bool initialized = false;
/* ���ݻ������� */
static DWORD oldConsoleMode;
static CONSOLE_CURSOR_INFO oldConsoleCursorInfo;
static WORD oldTextColor;

/* �����Ŀ��� */
static bool beepState;

/* ��ʼ�� */
void conlib_init()
{
	if(true == initialized)
		return; 
		
	initialized = true;
	atexit(conlib_close);
	/* ��ȡ����̨��� */ 
	hConsoleIn = GetStdHandle( STD_INPUT_HANDLE); 
	hConsoleOut = GetStdHandle( STD_OUTPUT_HANDLE);
	/* ���� */
	GetConsoleCursorInfo(hConsoleIn, &oldConsoleCursorInfo);
	GetConsoleMode(hConsoleIn, &oldConsoleMode);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsoleOut, &csbi);
	oldTextColor = csbi.wAttributes;

	/* ���ÿ���̨ģʽ */
	SetConsoleMode(hConsoleIn, DEFAULT_CONSOLE_MODE); 
	
	/* Ĭ����ɫ */
	wTextColor = ConWhite;
	wBackgroundColor = ConBlack; 
	// �ָ�Ĭ����ɫ
	set_text_color(wTextColor);

	/* Ĭ�ϴ����� */
	beep_switch(true);
}
void conlib_close()
{
	if(false == initialized)
		return;
	/* �ָ�����̨״̬ */
	SetConsoleCursorInfo(hConsoleIn, &oldConsoleCursorInfo);
	SetConsoleMode(hConsoleIn, oldConsoleMode);
	SetConsoleTextAttribute(hConsoleOut, oldTextColor);
	/* �رվ�� */
    CloseHandle(hConsoleIn);
    CloseHandle(hConsoleOut);
}
static void update_ctrlkey(unsigned state)
{
	cCtrlKeyState.enhanced = state & ENHANCED_KEY?true:false;
	
	cCtrlKeyState.left_alt = state & LEFT_ALT_PRESSED?true:false;
	cCtrlKeyState.left_ctrl = state & LEFT_CTRL_PRESSED?true:false;
	cCtrlKeyState.right_alt = state & RIGHT_ALT_PRESSED?true:false;
	cCtrlKeyState.right_ctrl = state & RIGHT_CTRL_PRESSED?true:false;
	cCtrlKeyState.shift = state & SHIFT_PRESSED?true:false;

	cCtrlKeyState.capslock = state & CAPSLOCK_ON?true:false;
	cCtrlKeyState.numlock = state & NUMLOCK_ON?true:false;
	cCtrlKeyState.scrolllock = state & SCROLLLOCK_ON?true:false;

}
/* �����Ļ */
void clrscr()
{
	system("cls");
}
/* ��ȡ��ֵ */ 
const ConKey* getkey()
{
	DWORD num;
	KEY_EVENT_RECORD ke;
	INPUT_RECORD e;
	/* ��ȡ�¼�����*/ 
	GetNumberOfConsoleInputEvents(hConsoleIn, &num);
	if(num < 1)
		return NULL;

	PeekConsoleInput(hConsoleIn, & e, 1, &num);
	if(e.EventType != MOUSE_EVENT && e.EventType != KEY_EVENT)
	{
		ReadConsoleInput(hConsoleIn, & e, 1, &num); 
		return NULL;
	}else if(e.EventType == MOUSE_EVENT)
		return NULL;
		
	/* ��ȡ���� */ 
	ReadConsoleInput(hConsoleIn, & e, 1, &num);

	if(e.EventType != KEY_EVENT)
		return NULL;
		
	/* ����ֵ */
	ke = e.Event.KeyEvent;
	
	cKey.key = ke.wVirtualKeyCode;
	cKey.state = ke.bKeyDown;
	cKey.ctrl_key_state = &cCtrlKeyState;
	update_ctrlkey(ke.dwControlKeyState);

return &cKey;
}
/* ��ȡ����¼� */
const ConMouse* getmouse()
{
	mouse_mode_active();

	DWORD num;
	INPUT_RECORD e;
	MOUSE_EVENT_RECORD me;
	/* ��ȡ�¼�����*/ 
	GetNumberOfConsoleInputEvents(hConsoleIn, &num);
	if(num < 1)
		return NULL;
	PeekConsoleInput(hConsoleIn, & e, 1, &num);
	if(e.EventType != MOUSE_EVENT && e.EventType != KEY_EVENT)
	{
		ReadConsoleInput(hConsoleIn, & e, 1, &num); 
		return NULL;
	}else if(e.EventType == KEY_EVENT)
		return NULL;

	/* ��ȡ���� */
	
	ReadConsoleInput(hConsoleIn, & e, 1, &num); 
	if(e.EventType != MOUSE_EVENT)
		return NULL;
		
	me = e.Event.MouseEvent;
	cMouse.x = me.dwMousePosition.X;
	cMouse.y = me.dwMousePosition.Y;
	update_ctrlkey(me.dwControlKeyState);
	cMouse.ctrl_key_state = &cCtrlKeyState;
	cMouse.event = 0;

	unsigned state = me.dwButtonState;
	int button = 0;
	if(state & FROM_LEFT_1ST_BUTTON_PRESSED)
		button |= ConMOUSE_1ST_BUTTON;
	if(state & FROM_LEFT_2ND_BUTTON_PRESSED)
		button |= ConMOUSE_2ND_BUTTON;
	if(state & FROM_LEFT_3RD_BUTTON_PRESSED)
		button |= ConMOUSE_3RD_BUTTON;
	if(state & FROM_LEFT_4TH_BUTTON_PRESSED)
		button |= ConMOUSE_4TH_BUTTON;
	if(state & RIGHTMOST_BUTTON_PRESSED)
		button |= ConMOUSE_RIGHT_BUTTON;
	cMouse.button = button;

	switch(me.dwEventFlags)
	{
		case DOUBLE_CLICK:
			cMouse.event = ConMOUSE_CLICK;
		break;
		case MOUSE_HWHEELED:
			cMouse.event = ConMOUSE_HWHEELED;
		break;
		case MOUSE_MOVED:
			cMouse.event = ConMOUSE_MOVED;
		break;
		case MOUSE_WHEELED:
			cMouse.event = ConMOUSE_WHEELED;
		break;

		/* ������ֱ���жϵ��¼� CLICK/RELEASED */
		default:
		{

			if(0 != cMouse.button)
				cMouse.event = ConMOUSE_CLICK;
			else
				cMouse.event = ConMOUSE_RELEASED;
		}
		break;
	}/* ����¼���� */

	/* ��������ʱ ����buttonΪ����ֵ */
	if(cMouse.event == ConMOUSE_WHEELED || cMouse.event == ConMOUSE_HWHEELED)
	{
		cMouse.button = me.dwButtonState;
	}

return &cMouse;
} 
/* �����ת */ 
void gotoxy( unsigned x, unsigned y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition( hConsoleOut, coord);
}

/* �ӳ� */ 
void delay( unsigned int delayMS)
{
	Sleep(delayMS);
}
/* ���ô��ڱ��� */
bool settitle(const char *title)
{
	return SetConsoleTitle(title);
}
/* ��ȡ���ڱ��� */
int gettitle(char *str, int len)
{
	return GetConsoleTitle(str, len);
}
/* ���ع�� */
bool hide_cursor()
{
	CONSOLE_CURSOR_INFO cur = { 100, 0};
	return SetConsoleCursorInfo( hConsoleOut, &cur);
}
/* ��ʾ��� percent Ϊ 0-100 �����Ĵ�С */ 
bool show_cursor(unsigned int percent)
{
	percent = percent % 101;
	CONSOLE_CURSOR_INFO cur = { percent, true};
	return SetConsoleCursorInfo( hConsoleOut, &cur);
}
/* ����������ɫ */
bool set_text_color(unsigned color)
{
	wTextColor = 0;
	if(color & ConRed)
		wTextColor |= FOREGROUND_RED;
	if(color & ConGreen)
		wTextColor |= FOREGROUND_GREEN;
	if(color & ConBlue)
		wTextColor |= FOREGROUND_BLUE;
	
	return SetConsoleTextAttribute(hConsoleOut, wTextColor | wBackgroundColor);
}
/* ���ñ�����ɫ */
bool set_background_color(unsigned color)
{
	wBackgroundColor = 0;
	if(color & ConRed)
		wBackgroundColor |= BACKGROUND_RED;
	if(color & ConGreen)
		wBackgroundColor |= BACKGROUND_GREEN;
	if(color & ConBlue)
		wBackgroundColor |= BACKGROUND_BLUE;

	return SetConsoleTextAttribute(hConsoleOut, wTextColor | wBackgroundColor);
}
/* ������ */
void beep(int frequence, int duration)
{
	if(beepState)
		Beep(frequence,duration);
}
/* �������� */
void beep_switch(bool turn)
{
	beepState = turn;
}
/* ���������Ŀ���״̬ */
bool beep_state()
{
	return beepState;
}
char pausei()
{
	return getch();
}

void mouse_mode_active()
{
	DWORD mode;
	GetConsoleMode(hConsoleIn, &mode);
	if(mode & ENABLE_MOUSE_INPUT)
		return;

	SetConsoleMode(hConsoleIn, DEFAULT_CONSOLE_MODE); 
}

void console_test()
{
	DWORD mode;
	GetConsoleMode(hConsoleIn, &mode);
	gotoxy(0, 14);
	printf("Mode is (%d):\n", (int)mode);
#define Tand(t)	if(mode & t)printf(#t "\n");
	Tand(ENABLE_ECHO_INPUT);
	Tand(ENABLE_INSERT_MODE);
	Tand(ENABLE_LINE_INPUT);
	Tand(ENABLE_MOUSE_INPUT);
	Tand(ENABLE_PROCESSED_INPUT);
	Tand(ENABLE_QUICK_EDIT_MODE);
	Tand(ENABLE_WINDOW_INPUT);
}

#endif
