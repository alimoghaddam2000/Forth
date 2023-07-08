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