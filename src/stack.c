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

bool isFull(Stack *s)
{
    return s->top >= STACK_SIZE - 1 ? true : false;
}

bool isEmpty(Stack *s)
{
    return s->top <= -1 ? true : false;
}

enum Err push(Stack *s, double n)
{
    if (isFull(s))
        return OVERFLOW_ERRCODE;
    s->stack[++s->top] = n;
    return NOERR;
}

DoubleWithErr pop(Stack *s)
{
    DoubleWithErr result;
    if (isEmpty(s))
        result.err = UNDERFLOW_ERRCODE;
    else
        result.err = NOERR;
    result.d = result.err ? 0 : s->stack[s->top--];
    return result;
}

DoubleWithErr peek(Stack *s, int i)
{
    DoubleWithErr res;
    res.err = NOERR;
    if (s->top == -1)
    {
        res.err = UNDERFLOW_ERRCODE;
        return res;
    }
    if (i > s->top || i < 0)
    {
        res.err = OUT_OF_RANGE;
        return res;
    }
    res.d = s->stack[i];
    return res;
}
