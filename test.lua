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

print("conlib���������ṹ")
printr("==========#conlib#==========",conlib);
print("conlib���������ṹ")
C.pause()

local key = {}
local mouse = {}

conlib.gotoxy(0,0)
print("���Ͱ������� e�˳� c�ֶ�����\nע�� �رտ��ٱ༭ģʽ �������ܻ�ȡ���")
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
print('��ɫ����-������ɫ')
C.setBackgroundColor(C.Color.Red)
clearpart(0,1,4,4)
C.setBackgroundColor(C.Color.Green)
clearpart(4,1,4,4)
C.setBackgroundColor(C.Color.Blue)
clearpart(8,1,4,4)

-- �ָ�����������ɫ
C.setBackgroundColor(C.Color.Black)
print('��ɫ����-������ɫ')
C.setTextColor(C.Color.Red)
print('Red')
C.setTextColor(C.Color.Green)
print('Green')
C.setTextColor(C.Color.Blue)
print('Blue')

-- �ָ�����������ɫ
C.setTextColor(C.Color.White)
print('��ɫ����-��ɫ���')
C.setTextColor(bit32.bor(C.Color.Red, C.Color.Green))
print('Red & Green')
C.setTextColor(bit32.bor(C.Color.Green, C.Color.Blue))
print('Green & Blue')
C.setTextColor(bit32.bor(C.Color.Red, C.Color.Blue))
print('Blue & Red')

-- �ָ�����������ɫ
C.setTextColor(C.Color.White)
C.pause()
C.clrscr()
print("��лʹ��!")
