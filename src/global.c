/**************************************************************************
 *   global.c  --  This file is part of Forth project.                    *
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
#include "prototype.h"

char *keyWords[] = {
    "do",
    "loop",
    "begin",
    "until",
    "+",
    "*",
    "/",
    "-",
    "%",
    "mod",
    "if",
    "else",
    "then",
    "variable",
    "constant",
    "key",
    "allot",
    "=",
    ">",
    "<",
    "and",
    "or",
    "invert",
    "i",
    "i'",
    "j",
    "dup",
    ".",
    "drop",
    "swap",
    "over",
    "rot",
    "@",
    "!",
    "+!",
    "?",
    ":",
    ";",
    "emit",
    "cr",
    ".\"",
    "sleep",
    "last-key",
    "random",
    "+loop"};
int keyWordCount = sizeof(keyWords) / sizeof(keyWords[0]);
Stack mainStack, returnStack;
DefinedWord mainWords[MAX_NUMBER_WORDS];
int numberOfWords = 0;
enum States mainState = OFF;
ifTemporary ifTemp;
forTemporary forTemp;
whileTemporary whileTemp;
int lastVariableRef = MEMORY_START;
Constant mainConstants[MAX_NUMBER_CONSTANTS];
int numberOfConstants = 0;
const char *prompt = "~~>";
const char *stackPrompt = "TOP ->";
const char *bottomPrompt = "Thanks for using my app!!";
WINDOW *stackWin, *mainWin, *bottomWin, *tabWin = NULL;
int xMaxMain, yMaxMain, xMaxStack, yMaxStack, xMaxStdScr, yMaxStdScr;
int line = 1, col = 0, row = 0, endLine = 0;
char *buffer[MAX_LINE];
char *bufferColor[MAX_LINE];
enum Mode mode = NORMAL;
int tabSelect = 0, tabStart = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Variable mainVariables[MAX_NUMBER_VARIABLES];
int numberOfVariables = 0;