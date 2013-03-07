brainfuck interpreter
======================
a brainfuck interpreter by xatier & jserv

Usage
------
### compile ###

compile with gcc4.7 & C99 standard

`gcc -O2 int.c -o int`

### run ###

the interpreter takes standard input (stdin)

    $ ./int < mandelbrot.b
    $ time ./int < mandelbrot.b

optimizations
--------------

* basic
    + the basic brainfuck interpreter by the definition of the language
* loop
    + pre-process the brainfuck program and build it into cells
    + recursively interprete the program (take loops as sub-programs)
* compact
    + compact patterns like `++++` or `<<<<`
* zero
    + deduce zero pattern: `[-]`
    + the C pattern `while (*ptr) { --*ptr; }` is the same as `*ptr = 0`

* another
    + also compact `+++--++---+` or `<<<<<>>>><<<>` patterns
    + dummy patterns are common in some brainfuck code-generator (compilers)


Licence
----------
Licensed under [GPL license][GPL].
[GPL]: http://www.gnu.org/licenses/gpl.html
