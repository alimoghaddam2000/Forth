#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <SDL.h>
#include <stdbool.h>

#define PIXEL_NUM 24
#define PIXEL_LENGTH 20

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480
#define MAX_COMMAND_LENGTH 50
#define MAX_NUMBER_VARIABLES 2000

typedef struct
{
    char name[MAX_COMMAND_LENGTH];
    double value;
    int ref;
} Variable;

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern Variable mainVariables[MAX_NUMBER_VARIABLES];
extern int numberOfVariables;

int initialize_window();
void render();
#endif