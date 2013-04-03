conlib for lua
===========

## 作者:
Author: *Wener*

E-mail: *wenermail(at)gmail.com*

Site: [Blog.wener.me](http://blog.wener.me)

## 描述:
* 供lua使用的conlib
* C的[conlib](https://github.com/WenerLove/conlib)

## *Tips*:
* require "conlib"
* 使用getkey或者getmouse时效率更高
	不用每次都生成新的table,而且用赋值的方法
	当没有输入的时候返回nil,会清除当前的table,
	下一次有输入的时候又需要创建新的table
* Enjoy it!

## EXAMPLE

	require "conlib"
	conlib.gotoxy(10, 10)
	print "I am here now"

## conlib.

* Color
	预定义的颜色table, 可以按位混合
* Key
	预定义的虚拟按键table
* MouseEvent
	预定义的鼠标事件
* MouseKey
	枚举的鼠标按键

* keytable
	getkey获取的key对象 
	{
		int key 虚拟键值
		int ascii 字符值
		bool state 按下或是释放的状态
		ctrlKey 控制键状态
	}
* mousetable
	getmouse获取的mouse对象 
	{
		int x, y 鼠标位置
		button 鼠标事件的按键 *注意* 当为滚动事件时,用该值来判断是向前还是向后
		event 事件类型
		ctrlKey 控制键状态
	}

* ctrlKey table
	控制按键 均为bool值 按下或未按下
	{
		enhanced
		leftCtrl, rightCtrl
		leftAlt, rightAlt
		shift
		numlock, capslock, scrolllock
	}

* bool settitle(string)
	设置窗口标题
* string gettitle()
	获取标题

* bool hideCursor()
	隐藏光标
* bool showCorsor(percent)
	显示光标 percent 为 [0,100] 光标显示的大小

* bool setTextColor(Color)
	设置文字颜色
* bool setBackgroundColor(Color)
	设置背景颜色

* beep(freq, dur)
	蜂鸣声,freq为频率,dur为时长
* beepSwitch(bool)
	开关蜂鸣的播放
* bool beepState()
	获取当前是否播放蜂鸣声

* delay(ms)
	延时ms毫秒
* char pause()
	暂停 等同于 getch
* clrscr()
	清除屏幕
* gotoxy(x, y)
	设置当前输出位置

* key/nil getkey([table])
	非阻塞的获取当前键盘输入
	当没有输入返回nil,当传入参数时,直接将结果写入到参数
	参数必须是table
* mouse/nil getmouse([table])
	非阻塞的获取当前鼠标输入
	当没有输入返回nil,当传入参数时,直接将结果写入到参数
	参数必须是table


