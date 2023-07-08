# Introduction

This project is a minimal Interpreter for Forth programming language

# Setup

First you have to make sure you have git, gcc and make install on your machine
then you can clone this project:

```bash
git clone https://github.com/alimoghaddam2000/Forth
```

after cloning you must install sdl2 and ncurses. if you are in ubuntu22 you have to run following command otherwise you should lookup how to install libsdl2 on your specific OS in google.

```bash
sudo apt-get install libsdl2-2.0-0 libsdl2-dev libncursesw5-dev
```

then you could easily build the project by running:

```bash
make
```

and for executing the program you run `./Final` on your terminal

For an example you can copy and paste content on snake.txt on your terminal and enjoy playing the snake game.

# Features

## This interpreter supports following key word:

1. do, loop, +loop, begin, until, i, i', j
2. - +, \* , /, %, mod
3. if, else, then, =, <, >, and, or, invert
4. variable, constant, allot, @
5. dup, drop, swap, over, rot,
6. key, last-key - emit, cr, ."
7. Sleep, random - (, )

## These are possible Errors:

1. Stack underflow
2. Stack overflow
3. Division by zero
4. Unknown word
5. Unknown variable
6. Using reserved name

# TODOs

- [x] Adding syntax highlighting.
- [ ] Adding auto completion

# Contacts

if you want to cooperate to this open source project feel free to push your commit and reach me through my email:
<ali.1379.moghaddam@gmail.com>
