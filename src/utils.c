/**************************************************************************
 *   utils.c  --  This file is part of Forth project.                     *
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
 * Inserts a character into a given position in an input array.
 *
 * @param input The input array where the character is to be inserted.
 * @param c The character to be inserted.
 * @param index The index position where the character is to be inserted.
 */
void insertChar(char input[], char c, int index)
{
    for (int i = strlen(input); i >= index; i--)
    {
        input[i + 1] = input[i]; // Shift character to the right
    }
    // Assign the desired character c to the specified index position in the input array.
    input[index] = c;
}

/**
 * Removes a character from a given position in an input array.
 *
 * @param input The input array from which the character is to be removed.
 * @param index The index position of the character to be removed.
 */
void removeChar(char input[], int index)
{
    for (int i = index; i < strlen(input); i++)
    {
        input[i] = input[i + 1]; // Shift character to the left
    }
}

/**
 * Clears a given input array by setting all elements to null characters ('\0').
 *
 * @param input The input array to be cleared.
 * @param size The size of the input array.
 */
void clearChar(char input[], int size)
{
    for (int i = 0; i < size; i++)
    {
        input[i] = '\0';
    }
}

/**
 * Splits a line of text into words, separated by spaces, and stores them in a 2D array.
 *
 * @param line The input line of text.
 * @param wordLine The 2D array to store the individual words.
 * @return The number of words found in the line.
 */
int spaceWord(char *line, char wordLine[MAX_NUMBER_COMMAND][MAX_COMMAND_LENGTH])
{
    int i1 = 0, i2 = 0;

    // Traverse the input line
    for (int i = 0; i < strlen(line); i++)
    {
        // Check if the current character is a space
        if (isspace(line[i]))
        {
            // If i2 is not zero, it means we have encountered a complete word
            if (i2 != 0)
            {
                // Terminate the word with a null character ('\0')
                wordLine[i1][i2] = '\0';

                // Reset i2 and move to the next row in the wordLine array
                i2 = 0;
                i1++;
                continue;
            }
            continue;
        }

        // Store the current non-space character in wordLine array
        wordLine[i1][i2] = line[i];
        i2++;
    }

    // Check if the last word was properly terminated
    if (i2 == 0)
    {
        // No additional word was found, return the current row index
        return i1;
    }

    // Terminate the last word with a null character ('\0')
    wordLine[i1][i2] = '\0';

    // Return the current row index plus one (since the index is zero-based)
    return i1 + 1;
}

/**
 * Checks if a given word is a valid keyword by comparing it with a list of predefined keywords.
 *
 * @param word The word to be checked.
 * @return true if the word is a valid keyword, false otherwise.
 */
bool isValidKeyWord(char *word)
{
    // Iterate over the list of predefined keywords
    for (int i = 0; i < keyWordCount; i++)
    {
        // Compare the given word with each keyword
        if (strcmp(word, keyWords[i]) == 0)
            return true; // The word is a valid keyword
    }

    // The word is not a valid keyword
    return false;
}

/**
 * Checks if a given input is a valid word by comparing it with a list of words.
 *
 * @param input The input word to be checked.
 * @return The index of the input word in the list of predefined words if it is valid, -1 otherwise.
 */
int isValidWord(char input[])
{
    // Iterate over the list of predefined words
    for (int i = 0; i < numberOfWords; i++)
    {
        // Compare the input word with each commandName in the mainWords array
        if (strcmp(input, mainWords[i].commandName) == 0)
            return i; // The input word is valid, return its index
    }

    // The input word is not valid
    return -1;
}

/**
 * Checks if a given input is a valid variable name by comparing it with a list of variable names.
 *
 * @param input The input variable name to be checked.
 * @return The index of the input variable name in the list of predefined variable names if it is valid, -1 otherwise.
 */
int isValidVar(char input[])
{
    // Iterate over the list of predefined variable names
    for (int i = 0; i < numberOfVariables; i++)
    {
        // Compare the input variable name with each name in the mainVariables array
        if (strcmp(input, mainVariables[i].name) == 0)
            return i; // The input variable name is valid, return its index
    }

    // The input variable name is not valid
    return -1;
}

/**
 * Checks if a given input is a valid constant name by comparing it with a list of constant names.
 *
 * @param input The input constant name to be checked.
 * @return The index of the input constant name in the list of predefined constant names if it is valid, -1 otherwise.
 */
int isValidConst(char input[])
{
    // Iterate over the list of predefined constant names
    for (int i = 0; i < numberOfConstants; i++)
    {
        // Compare the input constant name with each name in the mainConstants array
        if (strcmp(input, mainConstants[i].name) == 0)
            return i; // The input constant name is valid, return its index
    }

    // The input constant name is not valid
    return -1;
}

/**
 * Checks if a given input is a valid double by converting it to a double using strtod function.
 *
 * @param input The input string to be checked.
 * @return DoubleWithErr structure containing the converted double value and an error code.
 */
DoubleWithErr isValidDouble(char input[])
{
    char *endPt;       // Pointer to the end of the converted string
    double x;          // Variable to store the converted double value
    DoubleWithErr res; // Structure to store the result

    // Convert the input string to a double using strtod function
    x = strtod(input, &endPt);

    // Store the converted double value in the result structure
    res.d = x;

    // Check if the end pointer is not pointing to the end of the string or if the input string is empty
    if (*endPt != '\0' || strlen(input) == 0)
        res.err = NOT_A_NUMBER_ERR; // Set the error code to NOT_A_NUMBER_ERR
    else
        res.err = NOERR; // Set the error code to NOERR

    // Return the result structure
    return res;
}