/**************************************************************************
 *   prototype.h  --  This file is part of Forth project.                 *
 *                                                                        *
 *   Copyright (C) 2023 Ali Moghaddam.                                    *
 *                                                                        *
 *   Forth is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published    *
 *   by Ali Moghaddam, either version 3 of the License,                   *
 *   or (at your option) any later version.                               *
 *                                                                        *
 *   Forth is distributed in the hope that it will be useful,             *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/
#ifndef PROTOTYPE_H_INCLUDED
#define PROTOTYPE_H_INCLUDED

#include "definition.h"

// prompt for each of three main window
extern const char *prompt;
extern const char *stackPrompt;
extern const char *bottomPrompt;

// defining UI windows and their x,y maximum
extern WINDOW *stackWin, *mainWin, *bottomWin, *tabWin;
extern int xMaxMain, yMaxMain, xMaxStack, yMaxStack, xMaxStdScr, yMaxStdScr;

// current line in buffer and current column and row for most left-up position and last line in UI
extern int line, col, row, endLine;

// storing whole buffer and color of each character
extern char *buffer[MAX_LINE];
extern char *bufferColor[MAX_LINE];

// storing current Mode and State
extern enum Mode mode;
extern enum States mainState;

// variables needed for auto completion
extern int tabSelect, tabStart;

// storing key words and its count
extern char *keyWords[];
extern int keyWordCount;

// storing main and return stack
extern Stack mainStack, returnStack;

// storing word,constant and variable defined by user and its count
extern DefinedWord mainWords[MAX_NUMBER_WORDS];
extern int numberOfWords;
extern Constant mainConstants[MAX_NUMBER_CONSTANTS];
extern int numberOfConstants;
extern Variable mainVariables[MAX_NUMBER_VARIABLES];
extern int numberOfVariables;

// storing last memory referenced to a variable
extern int lastVariableRef;

// structures for storing recorded statement
extern ifTemporary ifTemp;
extern forTemporary forTemp;
extern whileTemporary whileTemp;

// sdl window and renderer for graphics
extern SDL_Window *window;
extern SDL_Renderer *renderer;

// graphics functions
int initialize_window();
void render();

// UI function for receiving character and last character entered by user
int getLastChar();
int getchCustom();

// stack functions
bool isFull(Stack *);
bool isEmpty(Stack *);
enum Err push(Stack *, double);
DoubleWithErr pop(Stack *);
DoubleWithErr peek(Stack *, int);

// calculator for basic math operation in stack
enum Err calculate(char);

// functions for validating input
DoubleWithErr isValidDouble(char[]);
int isValidWord(char[]);
int isValidConst(char[]);
int isValidVar(char[]);
bool isValidKeyWord(char *);

// main functions for executing one and multiple command
enum Err executeOne(char[]);
enum Err execute(char[][MAX_COMMAND_LENGTH], int);

// initializing functions
void initialization();
void initColor();

// UI functions
void printMainWindow();
void printLastLine(char *, char *);
void validateLastLine(char *, char *);
void showStack(Stack);
void customPrint(char[MAX_CHAR_PRINT]);
char validate(char *, char *, int, int);
void drawBottomWin();
void drawTabWin(char *, int);

// utils functions
void insertChar(char[], char, int);
void removeChar(char[], int);
int spaceWord(char *, char[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH]);
void clearChar(char[], int);

#endif