/**************************************************************************
 *   gui.c  --  This file is part of Forth project.                       *
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

/**
 * Displays the contents of a stack in a window.
 *
 * @param s The stack to be displayed.
 */
void showStack(Stack s)
{
    werase(stackWin);
    box(stackWin, 0, 0);
    mvwprintw(stackWin, 1, 1, stackPrompt);
    int index = 2 + strlen(stackPrompt);
    char double2string[MAX_CHAR_PRINT];
    for (int i = s.top; i >= 0; i--)
    {
        double x = s.stack[i];
        if (x != (int)x)
            snprintf(double2string, MAX_CHAR_PRINT, "%.1lf", x);
        else
            snprintf(double2string, MAX_CHAR_PRINT, "%d", (int)x);
        for (int j = 0; j < strlen(double2string); j++)
        {
            wattron(stackWin, COLOR_PAIR(COLOR_DOUBLE));
            mvwaddch(stackWin, 1, index, double2string[j]);
            wattroff(stackWin, COLOR_PAIR(COLOR_DOUBLE));
            index++;
            if (index == xMaxStack - 5)
            {
                wrefresh(stackWin);
                return;
            }
        }
        mvwaddch(stackWin, 1, index, ' ');
        index++;
        wattron(stackWin, A_REVERSE | COLOR_PAIR(COLOR_DOUBLE));
        mvwaddch(stackWin, 1, index, ' ');
        index++;
        wattroff(stackWin, A_REVERSE | COLOR_PAIR(COLOR_DOUBLE));
        mvwaddch(stackWin, 1, index, ' ');
        index++;
    }
    wrefresh(stackWin);
}

/**
 * Prints a custom input to the screen.
 *
 * @param input The message to be printed.
 */
void customPrint(char input[MAX_CHAR_PRINT])
{
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == '\n')
        {
            endLine++;
            clearChar(buffer[endLine + 1], MAX_COL);
            clearChar(bufferColor[endLine + 1], MAX_COL);
            line++;
            if (line > yMaxMain - 2 - 1)
            {
                row++;
                line--;
            }
            continue;
        }
        buffer[endLine][strlen(buffer[endLine])] = input[i];
        bufferColor[endLine][strlen(bufferColor[endLine])] = COLOR_NORMAL;
    }
    buffer[endLine][strlen(buffer[endLine])] = ' ';
    bufferColor[endLine][strlen(bufferColor[endLine])] = COLOR_NORMAL;
    endLine++;
    printMainWindow();
    endLine--;
    wrefresh(mainWin);
}

/**
 * Draws the bottom window with a box and displays a prompt in the center.
 */
void drawBottomWin()
{
    box(bottomWin, 0, 0);
    mvwprintw(bottomWin, 3, xMaxStdScr / 2 - strlen(bottomPrompt) / 2, bottomPrompt);
}

/**
 * Performs the necessary initialization steps
 */
void initialization()
{
    srand(time(NULL));
    char initialInput[][MAX_COMMAND_LENGTH] = {":", "cells", "1", "*", ";", "variable", "graphics", "24", "24", "*", "allot"};
    execute(initialInput, 11);
    mainStack.top = -1;
    returnStack.top = -1;
    initscr();
    noecho();
    cbreak();
    start_color();
    initColor();
    getmaxyx(stdscr, yMaxStdScr, xMaxStdScr);
    yMaxStdScr -= 5;
    stackWin = newwin(3, xMaxStdScr, 0, 0);
    bottomWin = newwin(5, xMaxStdScr, yMaxStdScr, 0);
    box(stackWin, 0, 0);
    box(bottomWin, 0, 0);
    wbkgd(stackWin, COLOR_PAIR(COLOR_NORMAL));
    mvwprintw(stackWin, 1, 1, stackPrompt);
    mvwprintw(bottomWin, 3, xMaxStdScr / 2 - strlen(bottomPrompt) / 2, bottomPrompt);
    refresh();
    wrefresh(stackWin);
    wrefresh(bottomWin);
    mainWin = newwin(yMaxStdScr - 3, xMaxStdScr, 3, 0);
    box(mainWin, 0, 0);
    keypad(stdscr, TRUE);
    refresh();
    wrefresh(mainWin);
    getmaxyx(mainWin, yMaxMain, xMaxMain);
    getmaxyx(stackWin, yMaxStack, xMaxStack);
    for (int i = 0; i < MAX_LINE; i++)
        buffer[i] = (char *)calloc(MAX_COL, sizeof(char));
    for (int i = 0; i < MAX_LINE; i++)
        bufferColor[i] = (char *)calloc(MAX_COL, sizeof(char));
    clearChar(buffer[0], MAX_COL);
    clearChar(bufferColor[0], MAX_COL);
    mvwprintw(mainWin, row + 1, 1, prompt);
    wrefresh(mainWin);
    initialize_window();
}

/**
 * Initializes the color pairs used in the program.
 */
void initColor()
{
    init_pair(COLOR_NORMAL, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_DOUBLE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_VAR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_CONST, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_WORD, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_KEYWORD, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_WRONG, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_STRING, COLOR_CYAN, COLOR_BLACK);
}

/**
 * Prints the contents of the main window.
 */
void printMainWindow()
{
    werase(mainWin);
    box(mainWin, 0, 0);
    for (int i = row; i <= yMaxMain + row - 2; i++)
    {
        if (i == endLine)
            break;
        for (int j = col; j < col + xMaxMain - 2; j++)
        {
            if (buffer[i][j] == '\0')
                break;
            wattron(mainWin, COLOR_PAIR(bufferColor[i][j]));
            mvwaddch(mainWin, i - row + 1, j - col + 1, buffer[i][j]);
            wattroff(mainWin, COLOR_PAIR(bufferColor[i][j]));
        }
    }
}

/**
 * Prints the last line of the main window with the specified color.
 *
 * @param lastLine The last line to be printed.
 * @param lastLineColor The color of each character in the last line.
 */
void printLastLine(char *lastLine, char *lastLineColor)
{
    clearChar(lastLineColor, MAX_COL);
    validateLastLine(lastLine, lastLineColor);
    char *finalLL = (char *)calloc(MAX_COL, sizeof(char));
    strcpy(finalLL, prompt);
    strcat(finalLL, lastLine);
    char *finalLLColor = (char *)calloc(MAX_COL, sizeof(char));
    for (int i = 0; i < strlen(prompt); i++)
        finalLLColor[i] = COLOR_NORMAL;
    finalLLColor[strlen(prompt)] = '\0';
    strcat(finalLLColor, lastLineColor);
    for (int i = col; i < col + xMaxMain - 2; i++)
    {
        if (finalLL[i] == '\0')
            break;
        if (!isspace(finalLL[i]))
        {
            wattron(mainWin, COLOR_PAIR(finalLLColor[i]));
            mvwaddch(mainWin, line, i - col + 1, finalLL[i]);
            wattroff(mainWin, COLOR_PAIR(finalLLColor[i]));
        }
        else
        {
            mvwaddch(mainWin, line, i - col + 1, finalLL[i]);
        }
    }
    free(finalLL);
    free(finalLLColor);
}

/**
 * Validates the last line of the main window and assigns the appropriate color to each character.
 *
 * @param lastLine The last line to be validated.
 * @param lastLineColor The array to store the color of each character in the last line.
 */
void validateLastLine(char *lastLine, char *lastLineColor)
{
    int mode = OFF;
    int isComment = false;
    char words[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    int count = spaceWord(lastLine, words);
    int p = 0;
    char temp;
    for (int i = 1; i <= count; i++)
    {
        if (i == 1)
            temp = validate(words[0], "", mode, isComment);
        else
            temp = validate(words[i - 1], words[i - 2], mode, isComment);
        for (int j = 0; j < strlen(words[i - 1]); j++)
        {
            lastLineColor[p] = temp;
            p++;
        }
        lastLineColor[p] = COLOR_NORMAL;
        p++;
        if (strcmp(words[i - 1], "(") == 0)
            isComment = true;
        else if (strcmp(words[i - 1], ")") == 0)
        {
            isComment = false;
        }
        if (strcmp(words[i - 1], ".\"") == 0)
            mode = PRINT;
        else if (words[i - 1][strlen(words[i - 1]) - 1] == '\"')
        {
            mode = OFF;
            lastLineColor[p - 2] = COLOR_KEYWORD;
        }
    }
}

/**
 * Validates a word and determines its appropriate color based on the current mode and context.
 *
 * @param currentWord The word to be validated.
 * @param prevWord The previous word in the line.
 * @param mode The current mode.
 * @param isComment Flag indicating if the word is inside a comment.
 * @return The color code for the word.
 */
char validate(char *currentWord, char *prevWord, int mode, int isComment)
{
    if (mode == PRINT || isComment || strcmp(currentWord, "(") == 0 || strcmp(currentWord, ")") == 0)
        return COLOR_STRING;
    if (isValidKeyWord(currentWord))
        return COLOR_KEYWORD;
    if (isValidConst(currentWord) != -1)
        return COLOR_CONST;
    if (isValidVar(currentWord) != -1)
        return COLOR_VAR;
    if (isValidWord(currentWord) != -1)
        return COLOR_WORD;
    DoubleWithErr num = isValidDouble(currentWord);
    if (num.err == NOERR)
        return COLOR_DOUBLE;
    if (strcmp(prevWord, "variable") == 0)
        return COLOR_VAR;
    if (strcmp(prevWord, "constant") == 0)
        return COLOR_CONST;
    if (strcmp(prevWord, ":") == 0)
        return COLOR_WORD;
    return COLOR_WRONG;
}

/**
 * Draws the tab window below the cursor, displaying suggestions based on the last entered word.
 *
 * @param lastLine The last line of the main window.
 * @param cursorPoint The current position of the cursor.
 */
void drawTabWin(char *lastLine, int curserPoint)
{
    char *word = strrchr(lastLine, ' ') + 1;
    if (tabWin != NULL)
    {
        delwin(tabWin);
        tabWin = NULL;
    }
    TabResult tb[TAB_SHOW];
    int numberOfTb = 0;
    for (int i = 0; i < keyWordCount; i++)
    {
        if (strncmp(word, keyWords[i], strlen(word)) == 0)
        {
            strcpy(tb[numberOfTb].name, keyWords[i]);
            strcat(tb[numberOfTb].name, "(KW)");
            tb[numberOfTb].kind = KEYWORD;
            numberOfTb++;
        }
    }
    if (curserPoint > xMaxStdScr - 20)
        curserPoint = xMaxStdScr - 20;
    tabWin = newwin(5, 20, line + 4, curserPoint);
    box(tabWin, 0, 0);
    int minTab = (tabStart + 5) < numberOfTb ? (tabStart + 5) : numberOfTb;
    for (int i = tabStart; i < minTab; i++)
    {
        if (i == tabSelect)
            wattron(tabWin, A_REVERSE);
        mvwprintw(tabWin, i - tabStart + 1, 0, tb[i].name);
        if (i == tabSelect)
            wattroff(tabWin, A_REVERSE);
    }
    wrefresh(tabWin);
}

/**
 * Custom implementation of the getch() function.
 *
 * @return The character read from the input.
 */
int getchCustom()
{
    return getch();
}

/**
 * Gets the last character entered within a specified timeout period.
 *
 * @return The last character entered.
 */
int getLastChar()
{
    timeout(50);
    int c = getch();
    timeout(-1);
    return c;
}