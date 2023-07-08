#include "./gui.h"

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

void insertChar(char input[], char c, int index)
{
    for (int i = strlen(input); i >= index; i--)
    {
        input[i + 1] = input[i];
    }
    input[index] = c;
}

void removeChar(char input[], int index)
{
    for (int i = index; i < strlen(input); i++)
    {
        input[i] = input[i + 1];
    }
}

void clearChar(char input[], int size)
{
    for (int i = 0; i < size; i++)
        input[i] = '\0';
}

void drawBottomWin()
{
    box(bottomWin, 0, 0);
    mvwprintw(bottomWin, 3, xMaxStdScr / 2 - strlen(bottomPrompt) / 2, bottomPrompt);
}

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

int spaceWord(char *line, char wordLine[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH])
{
    int i1 = 0, i2 = 0;
    for (int i = 0; i < strlen(line); i++)
    {
        if (isspace(line[i]))
        {
            if (i2 != 0)
            {
                wordLine[i1][i2] = '\0';
                i2 = 0;
                i1++;
                continue;
            }
            continue;
        }
        wordLine[i1][i2] = line[i];
        i2++;
    }
    if (i2 == 0)
    {
        return i1;
    }
    wordLine[i1][i2] = '\0';
    return i1 + 1;
}

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