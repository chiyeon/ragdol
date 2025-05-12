# ragdol
Ragdol is a tiny game engine & fantasy console written in C++20 and powered by SDL. It has the following (tentative) virtual limitations:
- a 16 color palette
- a 128 x 128 screen
- 8 button inputs (4 directional, 3 free, 1 menu/start)

## Interpreter
Currently ragdol uses a custom dynamic scripting language (that resembles an amalgamation of C and Python) with a custom interpreter. This is heavily a work in progress at the moment, and the language currently has support for:
- Dynamic creation and assignment/reassignment of variables (int, float, boolean, string)
- Scoping/Nesting of variables & logic
- Full expressions with order of operations from logical AND/OR all the way down to add/subtract
- Function definition & calling with optional returns and arguments
- Control flow with if/elif/else
- Loops (Currently only while loop)
- Builtin functions (Print, open window)

## Ragdol Example
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
