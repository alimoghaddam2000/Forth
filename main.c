#include "./graphic.h"
#include "./forthLogic.h"
#include "./gui.h"

int main()
{
    initialization();
    char *lastLine = (char *)calloc(MAX_COL, sizeof(char));
    char *lastLineColor = (char *)calloc(MAX_COL, sizeof(char));
    char input[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH];
    clearChar(lastLine, MAX_COL);
    clearChar(lastLineColor, MAX_COL);
    int sizeOfLastLine = 0, c, curserPoint = 0;
    while (true)
    {
        c = getch();
        switch (c)
        {
        case KEY_LEFT:
            if (curserPoint > 0)
                curserPoint--;
            break;
        case KEY_RIGHT:
            if (curserPoint < sizeOfLastLine)
                curserPoint++;
            break;
        case KEY_UP:
            break;
        case KEY_DOWN:
            break;
        case '\n':
            if (sizeOfLastLine == 0)
                break;
            line++;
            if (line > yMaxMain - 2 - 1)
            {
                row++;
                line--;
            }
            int count = spaceWord(lastLine, input);
            validateLastLine(lastLine, lastLineColor);
            for (int i = 0; i < strlen(prompt); i++)
            {
                insertChar(lastLine, prompt[i], i);
                insertChar(lastLineColor, COLOR_NORMAL, 0);
            }
            strcpy(buffer[endLine], lastLine);
            strcpy(bufferColor[endLine], lastLineColor);
            clearChar(lastLine, MAX_COL);
            clearChar(lastLineColor, MAX_COL);
            clearChar(buffer[endLine + 1], MAX_COL);
            clearChar(bufferColor[endLine + 1], MAX_COL);
            endLine++;
            curserPoint = col = sizeOfLastLine = 0;
            /// Execute///
            enum Err err = execute(input, count);
            /// Execute///
            showStack(mainStack);
            if (mainState == OFF)
            {
                char message[MAX_COMMAND_LENGTH];
                char colorCode;
                char temp[MAX_COMMAND_LENGTH];
                switch (err)
                {
                case NOERR:
                    strcpy(message, "OK");
                    colorCode = COLOR_KEYWORD;
                    break;
                case OVERFLOW_ERRCODE:
                    strcpy(message, "Err:Stack overflow");
                    colorCode = COLOR_WRONG;
                    break;
                case OUT_OF_RANGE:
                    strcpy(message, "Err:Item is out of range");
                    colorCode = COLOR_WRONG;
                    break;
                case DIVISION_BY_ZERO:
                    strcpy(message, "Err:Division by zero");
                    colorCode = COLOR_WRONG;
                    break;
                case UNKNOWN_VAR:
                    strcpy(message, "Err:Wrong Variable Name");
                    colorCode = COLOR_WRONG;
                    break;
                case RESERVED_NAME:
                    strcpy(message, "Err:using a reserved name");
                    colorCode = COLOR_WRONG;
                    break;
                case NOT_A_NUMBER_ERR:
                    strcpy(message, "Err:Not a number");
                    colorCode = COLOR_WRONG;
                    break;
                case UNDERFLOW_ERRCODE:
                    strcpy(message, "Err:Stack UnderFlow");
                    colorCode = COLOR_WRONG;
                    break;
                case UNKNOWN_WORD:
                    strcpy(message, "Err:Unknown Word");
                    colorCode = COLOR_WRONG;
                    break;
                default:
                    break;
                }
                strcat(buffer[endLine], message);
                for (int i = 0; i < strlen(message); i++)
                    temp[i] = colorCode;
                strcat(bufferColor[endLine], temp);
                endLine++;
                clearChar(buffer[endLine + 1], MAX_COL);
                clearChar(bufferColor[endLine + 1], MAX_COL);
                line++;
                if (line > yMaxMain - 2 - 1)
                {
                    row++;
                    line--;
                }
            }
            break;
        case KEY_BACKSPACE:
        case KEY_DC:
        case 127:
            if (curserPoint == 0)
                break;
            removeChar(lastLine, curserPoint - 1);
            sizeOfLastLine--;
            curserPoint--;
            break;
        default:
            if ((isspace(lastLine[curserPoint - 1]) || curserPoint == 0) && isspace(c))
                break;
            insertChar(lastLine, c, curserPoint);
            curserPoint++;
            sizeOfLastLine++;
        }
        if (curserPoint + strlen(prompt) > col + xMaxMain - 2)
            col++;
        if (curserPoint + strlen(prompt) <= col + xMaxMain - 2 && col > 0)
            col--;
        printMainWindow();
        printLastLine(lastLine, lastLineColor);
        // drawTabWin(lastLine, curserPoint);
        wmove(mainWin, line, curserPoint - col + strlen(prompt) + 1);
        wrefresh(mainWin);
    }
    getch();
    endwin();
}