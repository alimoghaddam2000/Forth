/**************************************************************************
 *   stack.c  --  This file is part of Forth project.                     *
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
 * Checks if the given stack is full.
 *
 * @param s The stack to be checked.
 * @return True if the stack is full, false otherwise.
 */
bool isFull(Stack *s)
{
    return s->top >= STACK_SIZE - 1 ? true : false;
}

/**
 * Checks if the given stack is empty.
 *
 * @param s The stack to be checked.
 * @return True if the stack is empty, false otherwise.
 */
bool isEmpty(Stack *s)
{
    return s->top <= -1 ? true : false;
}

/**
 * Pushes a double value onto the stack.
 *
 * @param s The stack to push the value onto.
 * @param n The double value to be pushed.
 * @return The error code indicating the result of the push operation.
 */
enum Err push(Stack *s, double n)
{
    if (isFull(s))
        return OVERFLOW_ERRCODE; // Return the error code OVERFLOW_ERRCODE if the stack is full
    s->stack[++s->top] = n;      // Increment the top index and assign the value to the stack
    return NOERR;                // Return the error code NOERR indicating a successful push operation
}

/**
 * Pops the top value from the stack.
 *
 * @param s The stack to pop the value from.
 * @return DoubleWithErr structure containing the popped value and an error code.
 */
DoubleWithErr pop(Stack *s)
{
    DoubleWithErr result; // Structure to store the result
    if (isEmpty(s))
        result.err = UNDERFLOW_ERRCODE; // Set the error code to UNDERFLOW_ERRCODE if the stack is empty
    else
        result.err = NOERR;                         // Set the error code to NOERR if the stack is not empty
    result.d = result.err ? 0 : s->stack[s->top--]; // Assign the popped value to the result structure
    return result;                                  // Return the result structure
}

/**
 * Retrieves the value at the specified index from the stack.
 *
 * @param s The stack to retrieve the value from.
 * @param i The index of the value to be retrieved.
 * @return DoubleWithErr structure containing the retrieved value and an error code.
 */
DoubleWithErr peek(Stack *s, int i)
{
    DoubleWithErr res; // Structure to store the result
    res.err = NOERR;   // Set the error code to NOERR
    if (s->top == -1)
    {
        res.err = UNDERFLOW_ERRCODE; // Set the error code to UNDERFLOW_ERRCODE if the stack is empty
        return res;                  // Return the result structure
    }
    if (i > s->top || i < 0)
    {
        res.err = OUT_OF_RANGE; // Set the error code to OUT_OF_RANGE if the specified index is out of range
        return res;             // Return the result structure
    }
    res.d = s->stack[i]; // Assign the retrieved value to the result structure
    return res;          // Return the result structure
}
