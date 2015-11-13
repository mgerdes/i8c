# i8c (mhmm tasty)

A second attempt at writing a compiler in C, to learn about parsing, lexical analysis, context free grammars, abstract syntax trees, code generation, and all sorts of other stuff involved with writing a compiler.

Compiles a subset of C to x86 assembly.

# Compiling / Running

Compile with `make` and then run `./i8c file.c` to compile the file using i8c. This creates an assembly file `file.s`, which can be assembled using GCC, it will also print out the AST created.

# Tests

There are some tests that I wrote to have some protection against regression faults. They can be run with `make test`.

# Not exactly C.

This does not actually conform to any official C language standard. Atleast for now it's more of a compiler for a 'C-like' language then exactly C. 

It's for learning purposes, not actual use.

# Things to add

* A Preprocessor
* Casting
* Arrays
* Typedefs
* Enums
* Lots more...

# References

[8cc](https://github.com/rui314/8cc)

[babyc](https://github.com/Wilfred/babyc)

[Incremental Approach To Compiler Construction](http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf)

[Unix Compiler Construction](http://www.amazon.com/Introduction-Compiler-Construction-Prentice-Hall-software/dp/0134743962)

[Dragon Book](https://en.wikipedia.org/wiki/Compilers:_Principles,_Techniques,_and_Tools)
