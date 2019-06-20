# tc-naf

A toy compiler, part of a university project.

## External tools used

- [re2c](re2c.org)
- [The LEMON Parser Generator](www.hwaci.com/sw/lemon)
- [SS-CE](github.com/Sima214/SS-CE)
- [Jemalloc](jemalloc.net)

## State

It's kind of working.
During semantic analysis when exiting a block I pop the stack for each declaration statement. However the dynamic cast segfaults. This is actually the first time I use dynamic casts in cpp, so I don't know what I did wrong. So I had to remove it.

Tokenization happens with re2c and syntax parsing with Lemon. Semantic analysis happens at the second pass(also called validate troughtout the code). A third pass finally generates the final representation(not executable just an AST dump).
