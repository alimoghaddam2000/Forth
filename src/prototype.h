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

extern const char *prompt;
extern const char *stackPrompt;
extern const char *bottomPrompt;
extern WINDOW *stackWin, *mainWin, *bottomWin, *tabWin;
extern int xMaxMain, yMaxMain, xMaxStack, yMaxStack, xMaxStdScr, yMaxStdScr;
extern int line, col, row, endLine;
extern char *buffer[MAX_LINE];
extern char *bufferColor[MAX_LINE];
extern enum Mode mode;
extern int tabSelect, tabStart;
extern char *keyWords[];
extern int keyWordCount;
extern Stack mainStack, returnStack;
extern DefinedWord mainWords[MAX_NUMBER_WORDS];
extern int numberOfWords;
extern enum States mainState;
extern ifTemporary ifTemp;
extern forTemporary forTemp;
extern whileTemporary whileTemp;
extern int lastVariableRef;
extern Constant mainConstants[MAX_NUMBER_CONSTANTS];
extern int numberOfConstants;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern Variable mainVariables[MAX_NUMBER_VARIABLES];
extern int numberOfVariables;

int initialize_window();
void render();
int getLastChar();
int getchCustom();
bool isFull(Stack *);
bool isEmpty(Stack *);
enum Err push(Stack *, double);
DoubleWithErr pop(Stack *);
DoubleWithErr peek(Stack *, int);
enum Err calculate(char);
DoubleWithErr isValidDouble(char[]);
int isValidWord(char[]);
int isValidConst(char[]);
int isValidVar(char[]);
enum Err executeOne(char[]);
enum Err execute(char[][MAX_COMMAND_LENGTH], int);
void showStack(Stack);
void customPrint(char[MAX_CHAR_PRINT]);
bool isValidKeyWord(char *);
void insertChar(char[], char, int);
void removeChar(char[], int);
void clearChar(char[], int);
void initialization();
void initColor();
void printMainWindow();
void printLastLine(char *, char *);
void validateLastLine(char *, char *);
int spaceWord(char *, char[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH]);
char validate(char *, char *, int, int);
void drawBottomWin();
void drawTabWin(char *, int);

#endif