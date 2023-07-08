#ifndef FORTHLOGIC_H_INCLUDED
#define FORTHLOGIC_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>

#include "./graphic.h"

#define TAB_SHOW 50
#define STACK_SIZE 100
#define MAX_NUMBER_COMMAND 50
#define MAX_NUMBER_WORDS 100
#define MAX_NUMBER_CONSTANTS 100
#define MAX_CHAR_PRINT 500
#define MEMORY_START 1000
#define MAX_LINE 500
#define MAX_COL 500

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

typedef struct
{
    char commandName[MAX_COMMAND_LENGTH];
    char commands[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int numberOfCommands;
} DefinedWord;

typedef struct
{
    char commandIf[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    char commandElse[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int ifCommandNumber;
    int elseCommandNumber;
    int ifNumbers;
} ifTemporary;

typedef struct
{
    char commandFor[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int forCommandNumber;
    int forNumbers;
} forTemporary;

typedef struct
{
    char commandWhile[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int whileCommandNumber;
    int whileNumbers;
} whileTemporary;

typedef struct
{
    char name[MAX_COMMAND_LENGTH];
    double value;
} Constant;

typedef struct
{
    int top;
    double stack[STACK_SIZE];
} Stack;

typedef struct
{
    double d;
    enum Err err;
} DoubleWithErr;

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
#endif