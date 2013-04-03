require 'conlib'
require 'luakit'

local C = conlib

function clearpart(x, y, w, h)
	local cleanbar = ' '
	cleanbar = cleanbar:rep(w)
	for i = y, h
	do
		conlib.gotoxy(x, i)
		print(cleanbar)
	end
end

print("conlib对象的整体结构")
printr("==========#conlib#==========",conlib);
print("conlib对象的整体结构")
C.pause()

local key = {}
local mouse = {}

conlib.gotoxy(0,0)
print("鼠标和按键测试 e退出 c手动清屏\n注意 关闭快速编辑模式 这样才能获取鼠标")
while(true)
do
	if key.ascii
	then
		if key.ascii == 101 then break end
		if key.ascii == 99 then conlib.clrscr() end
	end;

	if conlib.getkey(key, key) then
		clearpart(0, 2, 70, 20)
		conlib.gotoxy(0, 2)
		printr("==========#conlib.getkey()#==========",key);
	end

	if conlib.getmouse(mouse) then
		clearpart(0, 2, 70, 20)
		conlib.gotoxy(0, 2)
		printr("==========#conlib.getkmouse()#==========",mouse);
	end
end
C.clrscr();
C.gotoxy(0,0)
print('颜色测试-背景颜色')
C.setBackgroundColor(C.Color.Red)
clearpart(0,1,4,4)
C.setBackgroundColor(C.Color.Green)
clearpart(4,1,4,4)
C.setBackgroundColor(C.Color.Blue)
clearpart(8,1,4,4)

-- 恢复正常背景颜色
C.setBackgroundColor(C.Color.Black)
print('颜色测试-字体颜色')
C.setTextColor(C.Color.Red)
print('Red')
C.setTextColor(C.Color.Green)
print('Green')
C.setTextColor(C.Color.Blue)
print('Blue')

-- 恢复正常字体颜色
C.setTextColor(C.Color.White)
print('颜色测试-颜色混合')
C.setTextColor(bit32.bor(C.Color.Red, C.Color.Green))
print('Red & Green')
C.setTextColor(bit32.bor(C.Color.Green, C.Color.Blue))
print('Green & Blue')
C.setTextColor(bit32.bor(C.Color.Red, C.Color.Blue))
print('Blue & Red')

-- 恢复正常字体颜色
C.setTextColor(C.Color.White)
C.pause()
C.clrscr()
print("感谢使用!")
