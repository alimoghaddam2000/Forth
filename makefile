npC: main.c
	gcc -Wall -std=c99 main.c gui.c forthLogic.c graphic.c `sdl2-config --libs --cflags` -lm -lncurses -o Final

