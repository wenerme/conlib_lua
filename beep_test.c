#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "conlib.c"

int main(int argc, char* argv[])
{
	const ConKey *key;
	int step = 10;
	int f = 0;
	int d = 0;
	printf("beep���Գ���:\n���ҵ�Ƶ�� ���µ�ʱ�� ����Ϊ%d\n<�ո�>����\n<ESC>�˳�"
			,step);
	conlib_init();
	while(true)
	{	
		key = getkey();
		if(key && key->state)
		{
			switch(key->key)
			{
				case ConKEY_LEFT:
					f -= step;
					beep(f,d);
				break;
				case ConKEY_RIGHT:
					f += step;
					beep(f,d);
				break;
				case ConKEY_UP:
					d += step;
					beep(f,d);
				break;
				case ConKEY_DOWN:
					d -= step;
					beep(f,d);
				break;

				case ConKEY_SPACE:
					beep(f,d);
				break;
				case ConKEY_ESCAPE:
					exit(0);
				break;
			}// ���������ж�
			// ��ʾ��ǰֵ
			gotoxy(0, 5);
			printf("Frequence: %5d, Duration: %5d", f, d);
		}// ������������
	}
return EXIT_SUCCESS;
}
