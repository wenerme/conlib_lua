#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <lualib.h>
#include <lauxlib.h>
#include <lua.h>
#include "conlib.h"

#define PROJECT_TABLENAME "conlib"
#define MAX_TITLE_LENGTH 256

static const ConKey *l_key;
static const ConMouse *l_mouse;

LUAMOD_API int luaopen_conlib(lua_State *L);

static int l_conlib_init(lua_State *L);

static int l_getkey(lua_State *L);
static int l_getmouse(lua_State *L);

static int l_settitle(lua_State *L);
static int l_gettitle(lua_State *L);

static int l_hide_cursor(lua_State *L);
static int l_show_cursor(lua_State *L);

static int l_set_text_color(lua_State *L);
static int l_set_background_color(lua_State *L);

static int l_beep(lua_State *L);
static int l_beep_switch(lua_State *L);
static int l_beep_state(lua_State *L);

static int l_delay(lua_State *L);
static int l_pause(lua_State *L);
static int l_clrscr(lua_State *L);
static int l_gotoxy(lua_State *L);
// bool(title)
static int l_settitle(lua_State *L)
{
	const char *title= luaL_checkstring(L, 1);
	bool ret;

	ret = settitle(title);
	lua_pushboolean(L, ret);
	return 1;
}

// string bool()
static int l_gettitle(lua_State *L)
{
	char title[MAX_TITLE_LENGTH];
	bool ret;

	ret = gettitle(title, sizeof(title));
	lua_pushstring(L, title);
	lua_pushboolean(L, ret);
	return 2;
}

// bool()
static int l_hide_cursor(lua_State *L)
{
	bool ret;

	ret = hide_cursor();
	lua_pushboolean(L, ret);
	return 1;
}
// bool(int)
static int l_show_cursor(lua_State *L)
{
	int percent;
	bool ret;

	percent = luaL_checkint(L, 1);
	luaL_argcheck(L, percent >= 0, 1, "non-nagetive value");
	ret = show_cursor(percent);
	lua_pushboolean(L, ret);
	return 1;
}

// bool(color)
static int l_set_text_color(lua_State *L)
{
	int val;
	bool ret;

	val = luaL_checkint(L, 1);
	ret = set_text_color(val);
	lua_pushboolean(L, ret);
	return 1;
}
// bool(color)
static int l_set_background_color(lua_State *L)
{
	int val;
	bool ret;

	val = luaL_checkint(L, 1);
	ret = set_background_color(val);
	lua_pushboolean(L, ret);
	return 1;
}

// void(freq, dur)
static int l_beep(lua_State *L)
{
	int freq, dur;

	freq = luaL_checkint(L, 1);
	dur = luaL_checkint(L, 2);
	beep(freq, dur);
	return 0;
}
// void(bool)
static int l_beep_switch(lua_State *L)
{
	int val;

	val = luaL_checkint(L, 1);
	beep_switch(val);
	return 0;
}
// bool()
static int l_beep_state(lua_State *L)
{
	bool ret;

	ret = beep_state();
	lua_pushboolean(L, ret);
	return 1;
}

// void(ms)
static int l_delay(lua_State *L)
{
	int val;

	val = luaL_checkint(L, 1);
	delay(val);
	return 0;
}
// int()
static int l_pause(lua_State *L)
{
	bool ret;

	ret = pausei();
	lua_pushinteger(L, ret);
	return 1;
}
//
static int l_clrscr(lua_State *L)
{
	clrscr();
	return 0;
}
// void(x, y)
static int l_gotoxy(lua_State *L)
{
	int x, y;
	x = luaL_checkint(L, 1);
	y = luaL_checkint(L, 2);
	luaL_argcheck(L, x >= 0, 1, "non-nagetive value");
	luaL_argcheck(L, y >= 0, 1, "non-nagetive value");
	gotoxy(x, y);
	return 0;
}

// 辅助函数 用于生成控制键状态table 主table在gettop
static void set_ctrl_key_state(lua_State *L, const ConControlKeyState *cks)
{
	lua_getfield(L, -1, "ctrlKey");
	if(false == lua_istable(L, -1))
	{
		// 如果之前没有ctrlKey这个table 则新建 并弹出栈顶的那个值
		lua_remove(L, -1);
		lua_newtable(L);
		lua_setfield(L, -2, "ctrlKey");
		// 确保当前在栈顶
		lua_getfield(L, -1, "ctrlKey");
	}
	// 设置每个field
	lua_pushboolean(L, cks->enhanced);
	lua_setfield(L, -2, "enhanced");
	
	lua_pushboolean(L, cks->left_ctrl);
	lua_setfield(L, -2, "leftCtrl");
	
	lua_pushboolean(L, cks->right_ctrl);
	lua_setfield(L, -2, "rightCtrl");
	
	lua_pushboolean(L, cks->left_alt);
	lua_setfield(L, -2, "leftAlt");
	
	lua_pushboolean(L, cks->right_alt);
	lua_setfield(L, -2, "rightAlt");
	
	lua_pushboolean(L, cks->shift);
	lua_setfield(L, -2, "shift");
	
	lua_pushboolean(L, cks->numlock);
	lua_setfield(L, -2, "numlock");
	
	lua_pushboolean(L, cks->capslock);
	lua_setfield(L, -2, "capslock");
	
	lua_pushboolean(L, cks->scrolllock);
	lua_setfield(L, -2, "scrolllock");

	// 恢复栈状态
	lua_remove(L, -1);
}

// table([table]) 参数table可选,如果有则不创建table
static int l_getkey(lua_State *L)
{
	if(lua_istable(L, 1))
	{
		lua_pushvalue(L, 1);
	}else if(lua_gettop(L) != 0)
	{
		// 有参数 担不是 table 报错
		luaL_error(L, "the optional parammater must be table");
	}else
	{
		// 生成一个新table
		lua_newtable(L);
	}
	// 此时需要返回的table在stacktop
	// 更新键值
	getkey();
	
	lua_pushinteger(L, l_key->key);
	lua_setfield(L, -2, "key");

	lua_pushboolean(L, l_key->state);
	lua_setfield(L, -2, "state");

	set_ctrl_key_state(L, l_key->ctrl_key_state);

	return 1;
}

// table([table]) 参数table可选,如果有则不创建table
static int l_getmouse(lua_State *L)
{
	if(lua_istable(L, 1))
	{
		lua_pushvalue(L, 1);
	}else if(lua_gettop(L) != 0)
	{
		// 有参数 担不是 table 报错
		luaL_error(L, "the optional parammater must be table");
	}else
	{
		// 生成一个新table
		lua_newtable(L);
	}
	// 此时需要返回的table在stacktop
	// 更新值
	getmouse();
	
	lua_pushinteger(L, l_mouse->x);
	lua_setfield(L, -2, "x");

	lua_pushinteger(L, l_mouse->y);
	lua_setfield(L, -2, "y");

	lua_pushinteger(L, l_mouse->button);
	lua_setfield(L, -2, "button");

	lua_pushinteger(L, l_mouse->event);
	lua_setfield(L, -2, "event");


	set_ctrl_key_state(L, l_mouse->ctrl_key_state);

	return 1;
}


// 初始化
static int l_conlib_init(lua_State *L)
{
	// 键盘和鼠标获取对象
	l_key = getkey();
	l_mouse = getmouse();
	
	// 预定义颜色值
	return 0;
}

LUAMOD_API int luaopen_conlib(lua_State *L)
{
	static const struct luaL_Reg conlib[] = 
	{
		{"settitle", l_settitle}
		,{"gettitle", l_gettitle}

		,{"hideCursor", l_hide_cursor}
		,{"showCursor", l_show_cursor}

		,{"setTextColor",l_set_text_color}
		,{"setBackgroundColor",l_set_background_color}

		,{"beep",l_beep}
		,{"beepSwitch",l_beep_switch}
		,{"beepState",l_beep_state}

		,{"delay",l_delay}
		,{"pause",l_pause}
		,{"clrscr",l_clrscr}
		,{"gotoxy",l_gotoxy}

		,{"getkey", l_getkey}
		,{"getmouse", l_getmouse}
		,{NULL, NULL}
	};
	//
	// 初始化 conlib 库
	conlib_init();
	luaL_openlib(L, PROJECT_TABLENAME, conlib, 0);
	l_conlib_init(L);
	return 1;
}
