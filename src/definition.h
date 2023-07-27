/**************************************************************************
 *   definition.h  --  This file is part of Forth project.                *
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
#ifndef DEFINITION_H_INCLUDED
#define DEFINITION_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>
#include <SDL.h>
#include <stdbool.h>

// number of auto completion result
#define TAB_SHOW 50

// max stack size
#define STACK_SIZE 100

// max number of entered command and its length by user in single line
#define MAX_COMMAND_LENGTH 50
#define MAX_NUMBER_COMMAND 50

// maximum number of words and constant and variables defined by user
#define MAX_NUMBER_WORDS 100
#define MAX_NUMBER_CONSTANTS 100
#define MAX_NUMBER_VARIABLES 2000

#define MAX_CHAR_PRINT 500

// memory start location to reference for variables
#define MEMORY_START 1000

// maximum line and column in general for UI
#define MAX_LINE 500
#define MAX_COL 500

// length of each pixel and number of pixel for graphics
#define PIXEL_NUM 24
#define PIXEL_LENGTH 20

// graphic window width and height
#define WINDOW_WIDTH (PIXEL_LENGTH * PIXEL_NUM)
#define WINDOW_HEIGHT (PIXEL_LENGTH * PIXEL_NUM)

// Color codes for syntax highlighting
enum Color
{
    COLOR_NORMAL = 1,
    COLOR_VAR,
    COLOR_CONST,
    COLOR_DOUBLE,
    COLOR_WORD,
    COLOR_KEYWORD,
    COLOR_WRONG,
    COLOR_STRING
};

// The two mode for specifying auto completion modes
enum Mode
{
    NORMAL,
    TAB
};

// Different kind of word for auto completion window
enum WordKind
{
    VAR,
    WORD,
    KEYWORD,
    CONST
};

// Different State needed for specifying when to record
enum States
{
    OFF,
    WAITING_FOR_WORD,
    RECORDING_WORDS,
    IF,
    ELSE,
    FOR,
    WAITING_FOR_VARIABLE,
    PRINT,
    WAITING_FOR_CONSTANT,
    WHILE,
    COMMENT
};

// Error code for any error possible
enum Err
{
    NOERR,
    UNDERFLOW_ERRCODE,
    OVERFLOW_ERRCODE,
    OUT_OF_RANGE,
    DIVISION_BY_ZERO,
    NOT_A_NUMBER_ERR,
    UNKNOWN_VAR,
    UNKNOWN_WORD,
    RESERVED_NAME
};

// for storing variables
typedef struct
{
    char name[MAX_COMMAND_LENGTH];
    double value;
    int ref;
} Variable;

// for storing auto completion result
typedef struct
{
    char name[MAX_COMMAND_LENGTH];
    enum WordKind kind;
} TabResult;

// for storing word defined by user
typedef struct
{
    char commandName[MAX_COMMAND_LENGTH];
    char commands[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int numberOfCommands;
} DefinedWord;

// storing recorded if-else-then statement
typedef struct
{
    char commandIf[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    char commandElse[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int ifCommandNumber;
    int elseCommandNumber;
    int ifNumbers;
} ifTemporary;

// storing recorded do-loop statement
typedef struct
{
    char commandFor[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int forCommandNumber;
    int forNumbers;
} forTemporary;

// storing recorded begin-until statement
typedef struct
{
    char commandWhile[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int whileCommandNumber;
    int whileNumbers;
} whileTemporary;

// storing constant defined by user
typedef struct
{
    char name[MAX_COMMAND_LENGTH];
    double value;
} Constant;

// main and return stack structure
typedef struct
{
    int top;
    double stack[STACK_SIZE];
} Stack;

// type for functions to return double with error code
typedef struct
{
    double d;
    enum Err err;
} DoubleWithErr;

#endif
