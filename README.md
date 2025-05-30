# ragdol
Ragdol is a tiny game engine & fantasy console written in C++20 and powered by SDL. It has the following (tentative) virtual limitations:
- a 16 color palette
- a 128 x 128 screen
- 8 button inputs (4 directional, 3 free, 1 menu/start)

## Quickstart
### Requirements
- SDL2
- C++20 compiler (currently using `g++`)

```
# build project
$ mkdir build
$ cd build
$ cmake ..
$ make

# run scripts
$ ./ragdol test/fib.rd
```

## Interpreter
Currently ragdol uses a custom dynamic scripting language (that resembles an amalgamation of C and Python) with a custom interpreter that uses the visitor pattern. This is heavily a work in progress at the moment, and the language currently has support for:
- Dynamic creation and assignment/reassignment of variables (int, float, boolean, string)
- Scoping/Nesting of variables & logic
- Full expressions with order of operations from logical AND/OR all the way down to add/subtract
- Function definition & calling with optional returns and arguments
- Control flow with if/elif/else
- Loops (Currently only while loop)
- Builtin basic functions (Print, open window)
- Builtin game engine functions (process input, render graphics, init window)

Below is a basic example of an update function in a ragdol script polling input & drawing a moving player character & text to the screen.

![](/repo/helloworld.gif)

## WIP Features
Ragdol is a WIP engine and currently is looking to add the following:
- Arrays & Objects/tables
- Expansive builtin math library
- Game engine utilities (including for sound, levels, OOP, vectors & vector math, and more)
- Built in sprite editor & custom sprite loading
- General optimizations to interpreter

## Basic Ragdol Script Example
```
fn fb(n) {
    if (n <= 1) { return n }

    return fib(n - 1) + fib(n - 2)
}

var i = 0
while (i < 10) {
    i = i + 1
    print(fib(i))
}
```
