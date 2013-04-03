/*
 * Conlib v1.3
 * Author: Wener
 * E-mail: wenermail@gmail.com
 * Site: http://blog.wener.me
 *
 * һ��windows�¿���̨��̵ĸ�����
 *
 */

#include <stdbool.h>

#ifndef conlib_h
#define conlib_h

/* ��갴�� */
#define	ConMOUSE_LEFT_BUTTON	1
#define	ConMOUSE_1ST_BUTTON		1
#define	ConMOUSE_RIGHT_BUTTON	2
#define	ConMOUSE_CENTER_BUTTON	4
#define	ConMOUSE_2ND_BUTTON		4
#define	ConMOUSE_3RD_BUTTON		8
#define	ConMOUSE_4TH_BUTTON		10
/* ���̰��� */
/*
 * ����:
 * http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
 */
#define ConKEY_BACKSPACE		0x8
#define ConKEY_TAB				0x9
#define ConKEY_ESCAPE			0x1B
#define ConKEY_SPACE			0x20
#define ConKEY_LEFT				0x25
#define ConKEY_UP				0x26
#define ConKEY_RIGHT			0x27
#define ConKEY_DOWN				0x28
#define ConKEY_RETURN			0x0D
#define ConKEY_CONTROL			0x11
#define ConKEY_ALT				0x12

/* ��ɫ���� */
enum ConColor
{
	ConRed = 1,
	ConGreen = 2,
	ConBlue = 4,

	ConWhite = ConRed | ConGreen | ConBlue,
	ConBlack = 0
};
/* ���̿��Ƽ�״̬ */
typedef struct _ConControlKeyState
{
	bool enhanced;
	bool left_ctrl, right_ctrl;
	bool left_alt, right_alt;
	bool shift; 
	bool numlock,capslock,scrolllock; 
} ConControlKeyState; 
/* ������� */
enum ConMouseEvent
{
	ConMOUSE_MOVED
	,ConMOUSE_CLICK
	,ConMOUSE_DBCLICK
	,ConMOUSE_HWHEELED
	,ConMOUSE_WHEELED
	,ConMOUSE_RELEASED
}; 
typedef struct _ConMouse
{
	int x, y;
	int button; 
	enum ConMouseEvent event;
	ConControlKeyState *ctrl_key_state;
} ConMouse;
/* �������� */ 
typedef struct _ConKey
{
	int key;
	char ascii;
	bool state;
	ConControlKeyState *ctrl_key_state;
} ConKey;

/* ��ʼ�� ����ǰ�����ȵ��øú��� */
void conlib_init();

/* === ���� === */
/* ��ȡ��ֵ ������ʱ����NULL */
const ConKey* getkey();
/* ��ȡ��� ������ʱ����NULL */
const ConMouse* getmouse();

/* === ���� === */
/* ���ô��ڱ��� */
bool settitle(const char *title);
/* ��ȡ���ڱ��� ������д��str�� ����Ϊlen */
int gettitle(char *str, int len);

/* === ��� === */
/* ���ع�� */
bool hide_cursor();
/* ��ʾ��� percent Ϊ 0-100 �����Ĵ�С */ 
bool show_cursor(unsigned int percent);

/* === ɫ�� === */
/* ����������ɫ */
bool set_text_color(enum ConColor color);
/* ���ñ�����ɫ */
bool set_background_color(enum ConColor color);

/* === ���� === */
/* ������ ����ΪƵ�ʺ�ʱ�� */
void beep(int frequence, int duration);
/* �������� */
void beep_switch(bool turn);
/* ���������Ŀ���״̬ */
bool beep_state();

/* === һЩ�������� === */
/* �ӳ� */ 
void delay( unsigned int delayMS); 
/* ��ͣ���� ͬ getch ��ΪpauseΪunistd�й�����̵ĺ��� �����ͻ */
char pausei();
/* �����Ļ */
void clrscr();
/* �����ת */ 
void gotoxy(unsigned x, unsigned y);

#endif
