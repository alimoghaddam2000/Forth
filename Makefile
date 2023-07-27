npC: src/forth.c
	gcc -Wall -std=c99 src/forth.c src/gui.c src/logic.c src/graphics.c src/global.c src/utils.c src/stack.c `sdl2-config --libs --cflags` -lm -lncurses -o Forth.out

