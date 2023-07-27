/**************************************************************************
 *   logic.c  --  This file is part of Forth project.                     *
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
 * Calculates the result of an arithmetic operation using the given operator and the top two values from the mainStack.
 *
 * @param operator The arithmetic operator to be used for the calculation.
 * @return The error code indicating the result of the calculation.
 */
enum Err calculate(char operator)
{
    DoubleWithErr x, y;  // Variables to store the top two values from the mainStack
    x = pop(&mainStack); // Get the top value from the mainStack and store it in x
    y = pop(&mainStack); // Get the second top value from the mainStack and store it in y

    // Check if the top value from the mainStack has an error
    if (x.err)
        return x.err; // Return the error code of x

    // Check if the second top value from the mainStack has an error
    if (y.err)
        return y.err; // Return the error code of y

    // Perform the arithmetic operation based on the given operator
    switch (operator)
    {
    case '+':
        push(&mainStack, x.d + y.d); // Push the result of addition to the mainStack
        break;
    case '-':
        push(&mainStack, y.d - x.d); // Push the result of subtraction to the mainStack
        break;
    case '*':
        push(&mainStack, x.d * y.d); // Push the result of multiplication to the mainStack
        break;
    case '/':
        if (x.d == 0)
            return DIVISION_BY_ZERO; // Return the error code DIVISION_BY_ZERO if x is zero
        push(&mainStack, y.d / x.d); // Push the result of division to the mainStack
        break;
    case '%':
        if (x.d == 0 || (int)x.d == 0)
            return DIVISION_BY_ZERO;           // Return the error code DIVISION_BY_ZERO if x is zero or not an integer
        push(&mainStack, (int)y.d % (int)x.d); // Push the result of modulo division to the mainStack
        break;
    default:
        break;
    }

    // Return the error code NOERR indicating a successful calculation
    return NOERR;
}

/**
 * execute given input
 *
 * @param  input given input to be execute.
 * @return The error code indicating the result of the calculation.
 */
enum Err executeOne(char input[])
{
    if (mainState == WAITING_FOR_WORD)
    {
        if (isValidWord(input) != -1 || isValidConst(input) != -1 || isValidKeyWord(input) || isValidVar(input) != -1)
        {
            mainState = OFF;
            return RESERVED_NAME;
        }
        DefinedWord d;
        strcpy(d.commandName, input);
        d.numberOfCommands = 0;
        mainWords[numberOfWords] = d;
        mainState = RECORDING_WORDS;
        return NOERR;
    }
    if (mainState == WAITING_FOR_VARIABLE)
    {
        mainState = OFF;
        if (isValidWord(input) != -1 || isValidConst(input) != -1 || isValidKeyWord(input) || isValidVar(input) != -1)
            return RESERVED_NAME;
        Variable v;
        strcpy(v.name, input);
        v.value = 0;
        v.ref = lastVariableRef;
        mainVariables[numberOfVariables] = v;
        numberOfVariables++;
        lastVariableRef++;
        return NOERR;
    }
    if (mainState == RECORDING_WORDS)
    {
        if (strcmp(input, ";") == 0)
        {
            numberOfWords++;
            mainState = OFF;
            return NOERR;
        }
        strcpy(mainWords[numberOfWords].commands[mainWords[numberOfWords].numberOfCommands++], input);
        return NOERR;
    }
    if (mainState == IF)
    {
        if (strcmp(input, "if") == 0)
        {
            ifTemp.ifNumbers++;
        }
        else if (strcmp(input, "then") == 0)
        {
            if (ifTemp.ifNumbers == 1)
            {
                mainState = OFF;
                DoubleWithErr x = pop(&mainStack);
                if (x.err == NOERR && x.d != 0)
                    return execute(ifTemp.commandIf, ifTemp.ifCommandNumber);
                return x.err;
            }
            else
            {
                ifTemp.ifNumbers--;
            }
        }
        else if (strcmp(input, "else") == 0)
        {
            if (ifTemp.ifNumbers == 1)
            {
                mainState = ELSE;
                return NOERR;
            }
        }
        strcpy(ifTemp.commandIf[ifTemp.ifCommandNumber++], input);
        return NOERR;
    }
    if (mainState == ELSE)
    {
        if (strcmp(input, "if") == 0)
        {
            ifTemp.ifNumbers++;
        }
        else if (strcmp(input, "then") == 0)
        {
            if (ifTemp.ifNumbers == 1)
            {
                mainState = OFF;
                DoubleWithErr x = pop(&mainStack);
                if (x.err == NOERR && x.d == 0)
                    return execute(ifTemp.commandElse, ifTemp.elseCommandNumber);
                if (x.err == NOERR && x.d != 0)
                    return execute(ifTemp.commandIf, ifTemp.ifCommandNumber);
                return x.err;
            }
            else
            {
                ifTemp.ifNumbers--;
            }
        }
        strcpy(ifTemp.commandElse[ifTemp.elseCommandNumber++], input);
        return NOERR;
    }
    if (mainState == FOR)
    {
        if (strcmp(input, "do") == 0)
        {
            forTemp.forNumbers++;
        }
        if (strcmp(input, "loop") == 0)
        {
            if (forTemp.forNumbers == 1)
            {
                mainState = OFF;
                forTemporary temp1 = forTemp;
                DoubleWithErr x = pop(&mainStack), y = pop(&mainStack);
                if (x.err != NOERR)
                    return x.err;
                if (y.err != NOERR)
                    return y.err;
                enum Err e1, e2, e3;
                double first = x.d, last = y.d;
                for (double i = first; i < last; i++)
                {
                    e1 = push(&returnStack, last - i + first);
                    e2 = push(&returnStack, i);
                    if (e1 != NOERR)
                        return e1;
                    if (e2 != NOERR)
                        return e2;
                    e3 = execute(temp1.commandFor, temp1.forCommandNumber);
                    if (e3 != NOERR)
                        return e3;
                    x = pop(&returnStack);
                    y = pop(&returnStack);
                    if (x.err != NOERR)
                        return x.err;
                    if (y.err != NOERR)
                        return y.err;
                }
                return NOERR;
            }
            else
            {
                forTemp.forNumbers--;
            }
        }
        else if (strcmp(input, "+loop") == 0)
        {
            if (forTemp.forNumbers == 1)
            {
                mainState = OFF;
                forTemporary temp1 = forTemp;
                DoubleWithErr x = pop(&mainStack), y = pop(&mainStack), z;
                if (x.err != NOERR)
                    return x.err;
                if (y.err != NOERR)
                    return y.err;
                enum Err e1, e2, e3;
                double first = x.d, last = y.d;
                double i = first;
                do
                {
                    e1 = push(&returnStack, last - i + first);
                    e2 = push(&returnStack, i);
                    if (e1 != NOERR)
                        return e1;
                    if (e2 != NOERR)
                        return e2;
                    e3 = execute(temp1.commandFor, temp1.forCommandNumber);
                    if (e3 != NOERR)
                        return e3;
                    x = pop(&returnStack);
                    y = pop(&returnStack);
                    if (x.err != NOERR)
                        return x.err;
                    if (y.err != NOERR)
                        return y.err;
                    z = pop(&mainStack);
                    if (z.err != NOERR)
                        return z.err;
                    i += z.d;
                } while (z.d > 0 ? i < last : i >= last);
                return NOERR;
            }
            else
            {
                forTemp.forNumbers--;
            }
        }
        strcpy(forTemp.commandFor[forTemp.forCommandNumber++], input);
        return NOERR;
    }
    if (mainState == WHILE)
    {
        if (strcmp(input, "begin") == 0)
        {
            whileTemp.whileNumbers++;
        }
        if (strcmp(input, "until") == 0)
        {
            if (whileTemp.whileNumbers == 1)
            {
                mainState = OFF;
                DoubleWithErr x;
                whileTemporary temp1 = whileTemp;
                enum Err e;
                do
                {
                    e = execute(temp1.commandWhile, temp1.whileCommandNumber);
                    if (e != NOERR)
                        return e;
                    x = pop(&mainStack);
                    if (x.err != NOERR)
                        return x.err;
                } while ((int)x.d != 0);
                return NOERR;
            }
            else
            {
                whileTemp.whileNumbers--;
            }
        }
        strcpy(whileTemp.commandWhile[whileTemp.whileCommandNumber++], input);
        return NOERR;
    }
    if (mainState == PRINT)
    {
        if (input[strlen(input) - 1] == '\"')
        {
            mainState = OFF;
            char t[MAX_CHAR_PRINT];
            strcpy(t, input);
            t[strlen(t) - 1] = '\0';
            customPrint(t);
            return NOERR;
        }
        customPrint(input);
        return NOERR;
    }
    if (mainState == WAITING_FOR_CONSTANT)
    {
        mainState = OFF;
        if (isValidWord(input) != -1 || isValidConst(input) != -1 || isValidKeyWord(input) || isValidVar(input) != -1)
            return RESERVED_NAME;
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        Constant c;
        strcpy(c.name, input);
        c.value = x.d;
        mainConstants[numberOfConstants] = c;
        numberOfConstants++;
        return NOERR;
    }
    if (mainState == COMMENT)
    {
        if (strcmp(input, ")") == 0)
            mainState = OFF;
        return NOERR;
    }
    if (strcmp(input, "(") == 0)
    {
        mainState = COMMENT;
        return NOERR;
    }
    if (strcmp(input, "sleep") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        sleep(x.d / 1000);
        return NOERR;
    }
    if (strcmp(input, "key") == 0)
    {
        char c = getchCustom();
        return push(&mainStack, (double)c);
    }
    if (strcmp(input, "last-key") == 0)
    {
        char c = getLastChar();
        return push(&mainStack, (double)c);
    }
    if (strcmp(input, "random") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        return push(&mainStack, (double)(rand() % ((int)x.d)));
    }
    if (strcmp(input, "allot") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        for (int i = 0; i < (int)x.d; i++)
        {
            mainVariables[numberOfVariables].ref = lastVariableRef;
            numberOfVariables++;
            lastVariableRef++;
        }
        return NOERR;
    }
    if (strcmp(input, "constant") == 0)
    {
        mainState = WAITING_FOR_CONSTANT;
        return NOERR;
    }
    if (strcmp(input, "=") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        DoubleWithErr y = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        if (y.err != NOERR)
            return y.err;
        return push(&mainStack, x.d == y.d ? -1 : 0);
    }
    if (strcmp(input, ">") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        DoubleWithErr y = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        if (y.err != NOERR)
            return y.err;
        return push(&mainStack, x.d > y.d ? -1 : 0);
    }
    if (strcmp(input, "<") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        DoubleWithErr y = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        if (y.err != NOERR)
            return y.err;
        return push(&mainStack, x.d < y.d ? -1 : 0);
    }
    if (strcmp(input, "and") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        DoubleWithErr y = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        if (y.err != NOERR)
            return y.err;
        return push(&mainStack, (x.d != 0 && y.d != 0) ? -1 : 0);
    }
    if (strcmp(input, "or") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        DoubleWithErr y = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        if (y.err != NOERR)
            return y.err;
        return push(&mainStack, (x.d != 0 || y.d != 0) ? -1 : 0);
    }
    if (strcmp(input, "invert") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        return push(&mainStack, x.d != 0 ? 0 : -1);
    }
    if (strcmp(input, "if") == 0)
    {
        mainState = IF;
        ifTemp.ifNumbers = 1;
        ifTemp.ifCommandNumber = 0;
        ifTemp.elseCommandNumber = 0;
        return NOERR;
    }
    if (strcmp(input, "do") == 0)
    {
        mainState = FOR;
        forTemp.forNumbers = 1;
        forTemp.forCommandNumber = 0;
        return NOERR;
    }
    if (strcmp(input, "begin") == 0)
    {
        mainState = WHILE;
        whileTemp.whileNumbers = 1;
        whileTemp.whileCommandNumber = 0;
        return NOERR;
    }
    if (strcmp(input, "i") == 0)
    {
        DoubleWithErr res = peek(&returnStack, returnStack.top);
        if (res.err != NOERR)
        {
            return res.err;
        }
        return push(&mainStack, res.d);
    }
    if (strcmp(input, "i'") == 0)
    {
        DoubleWithErr res = peek(&returnStack, returnStack.top - 1);
        if (res.err != NOERR)
        {
            return res.err;
        }
        return push(&mainStack, res.d);
    }
    if (strcmp(input, "j") == 0)
    {
        DoubleWithErr res = peek(&returnStack, returnStack.top - 2);
        if (res.err != NOERR)
        {
            return res.err;
        }
        return push(&mainStack, res.d);
    }
    if (strcmp(input, "dup") == 0)
    {
        DoubleWithErr x = peek(&mainStack, mainStack.top);
        if (x.err != NOERR)
        {
            return x.err;
        }
        return push(&mainStack, x.d);
    }
    if (strcmp(input, ".") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
        {
            return x.err;
        }
        char double2string[MAX_CHAR_PRINT];
        if (x.d != (int)x.d)
            snprintf(double2string, MAX_CHAR_PRINT, "%.1lf", x.d);
        else
            snprintf(double2string, MAX_CHAR_PRINT, "%d", (int)x.d);
        customPrint(double2string);
        return NOERR;
    }
    if (strcmp(input, "drop") == 0)
    {
        return pop(&mainStack).err;
    }
    if (strcmp(input, "swap") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        DoubleWithErr y = pop(&mainStack);
        if (x.err != NOERR)
        {
            return x.err;
        }
        if (y.err != NOERR)
        {
            return y.err;
        }
        push(&mainStack, x.d);
        return push(&mainStack, y.d);
    }
    if (strcmp(input, "over") == 0)
    {
        DoubleWithErr x = peek(&mainStack, mainStack.top - 1);
        if (x.err != NOERR)
        {
            return x.err;
        }
        return push(&mainStack, x.d);
    }
    if (strcmp(input, "rot") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        DoubleWithErr y = pop(&mainStack);
        DoubleWithErr z = pop(&mainStack);
        if (x.err != NOERR)
        {
            return x.err;
        }
        if (y.err != NOERR)
        {
            return y.err;
        }
        if (z.err != NOERR)
        {
            return z.err;
        }
        push(&mainStack, y.d);
        push(&mainStack, x.d);
        return push(&mainStack, z.d);
    }
    if (strcmp(input, "variable") == 0)
    {
        mainState = WAITING_FOR_VARIABLE;
        return NOERR;
    }
    if (strcmp(input, "@") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        for (int i = 0; i < numberOfVariables; i++)
        {
            if (mainVariables[i].ref == (int)x.d && x.d - (int)x.d == 0)
            {
                return push(&mainStack, mainVariables[i].value);
            }
        }
        return UNKNOWN_VAR;
    }
    if (strcmp(input, "!") == 0)
    {
        DoubleWithErr ref = pop(&mainStack);
        DoubleWithErr x = pop(&mainStack);
        if (ref.err != NOERR)
            return ref.err;
        if (x.err != NOERR)
            return x.err;
        for (int i = 0; i < numberOfVariables; i++)
        {
            if (mainVariables[i].ref == (int)ref.d && ref.d - (int)ref.d == 0)
            {
                mainVariables[i].value = x.d;
                if (i < PIXEL_NUM * PIXEL_NUM)
                    render();
                return NOERR;
            }
        }
        return UNKNOWN_VAR;
    }
    if (strcmp(input, "+!") == 0)
    {
        DoubleWithErr ref = pop(&mainStack);
        DoubleWithErr x = pop(&mainStack);
        if (ref.err != NOERR)
            return ref.err;
        if (x.err != NOERR)
            return x.err;
        for (int i = 0; i < numberOfVariables; i++)
        {
            if (mainVariables[i].ref == (int)ref.d && ref.d - (int)ref.d == 0)
            {
                mainVariables[i].value += x.d;
                if (i < PIXEL_NUM * PIXEL_NUM)
                    render();
                return NOERR;
            }
        }
        return UNKNOWN_VAR;
    }
    if (strcmp(input, "?") == 0)
    {
        char temp[][MAX_COMMAND_LENGTH] = {"@", "."};
        return execute(temp, 2);
    }
    if (strcmp(input, ":") == 0)
    {
        mainState = WAITING_FOR_WORD;
        return NOERR;
    }
    if (strcmp(input, "emit") == 0)
    {
        DoubleWithErr x = pop(&mainStack);
        if (x.err != NOERR)
            return x.err;
        char temp[MAX_CHAR_PRINT] = {(char)x.d, '\0'};
        customPrint(temp);
        return NOERR;
    }
    if (strcmp(input, "cr") == 0)
    {
        char temp[MAX_CHAR_PRINT] = {'\n', '\0'};
        customPrint(temp);
        return NOERR;
    }
    if (strcmp(input, ".\"") == 0)
    {
        mainState = PRINT;
        return NOERR;
    }
    if (strcmp(input, "*") == 0)
        return calculate('*');
    if (strcmp(input, "/") == 0)
        return calculate('/');
    if (strcmp(input, "+") == 0)
        return calculate('+');
    if (strcmp(input, "-") == 0)
        return calculate('-');
    if (strcmp(input, "%") == 0 || strcmp(input, "mod") == 0)
        return calculate('%');
    DoubleWithErr num = isValidDouble(input);
    if (num.err == NOERR)
        return push(&mainStack, num.d);
    int wordIndex = isValidWord(input);
    if (wordIndex != -1)
        return execute(mainWords[wordIndex].commands, mainWords[wordIndex].numberOfCommands);
    int varIndex = isValidVar(input);
    if (varIndex != -1)
        return push(&mainStack, (double)mainVariables[varIndex].ref);
    int constIndex = isValidConst(input);
    if (constIndex != -1)
        return push(&mainStack, mainConstants[constIndex].value);

    return UNKNOWN_WORD;
}

/**
 * Executes a series of commands stored in a 2D array of strings.
 *
 * @param input The 2D array of strings containing the commands to be executed.
 * @param size The number of commands in the input array.
 * @return The error code indicating the result of the execution.
 */
enum Err execute(char input[][MAX_COMMAND_LENGTH], int size)
{
    enum Err e;
    for (int i = 0; i < size; i++)
    {
        e = executeOne(input[i]);
        if (e != NOERR)
        {
            return e;
        }
    }
    return NOERR;
}
