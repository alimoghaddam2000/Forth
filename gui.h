#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "./forthLogic.h"

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

enum Mode
{
    NORMAL,
    TAB
};

enum WordKind
{
    VAR,
    WORD,
    KEYWORD,
    CONST
};

typedef struct
{
    char name[MAX_COMMAND_LENGTH];
    enum WordKind kind;
} TabResult;

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