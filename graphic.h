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

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

Variable mainVariables[MAX_NUMBER_VARIABLES];
int numberOfVariables = 0;

int initialize_window();
void render();
#endif