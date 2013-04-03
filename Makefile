CFLAG=-Wall -g3
conlib:
	gcc --shared lconlib.c lua52.dll conlib.c -o conlib.dll -Wall
clean:
	-rm -f conlib.dll *.o
