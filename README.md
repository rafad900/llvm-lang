# RafLang
![Build](https://img.shields.io/badge/build-passing-brightgreen)
> RafLang — a tiny work-in-progress language front-end using Flex, Bison and LLVM.

This repository contains a small compiler front-end for the RafLang language. It uses
- Flex for lexical scanning
- Bison for parsing
- LLVM (via clang/llvm-config) to generate and print IR

Contents
- `scanner.lex` — Flex lexer definitions
- `parser.ypp` / `parser.tab.cpp` — Bison grammar and generated parser
- `nodes.cpp` / `nodes.hpp` — AST node implementations
- `llvmvisitor.cpp` — LLVM codegen visitor that prints IR
- `main.cpp` — driver that runs the parser and emits IR

Prerequisites
- clang / clang++ (or g++)
- llvm and llvm-config (matching version used in the code)
- flex
- bison

On Debian/Ubuntu you can install the tools with:
```bash
sudo apt update
sudo apt install build-essential clang llvm llvm-dev llvm-tools-flex bison flex
```
If you prefer a specific LLVM release use the `apt.llvm.org` packages and the matching `llvm-config-XX` binary.

Build steps
1. Generate the parser and header (Bison):
```bash
bison -d -o parser.tab.cpp parser.ypp
```

2. Generate the scanner (Flex):
```bash
flex -o scanner.c scanner.lex
```

3. Compile and link (example using `clang++` and `llvm-config`):
```bash
clang++ -std=c++17 $(llvm-config --cxxflags) \
  parser.tab.cpp scanner.c llvmvisitor.cpp nodes.cpp main.cpp \
  -o raflang $(llvm-config --ldflags --libs core --system-libs)
```

Notes:
- If your environment has versioned llvm-config (e.g. `llvm-config-14`) replace `llvm-config` with the proper binary.
- You can also use `g++` instead of `clang++`, but for generating/linking against LLVM it is convenient to use `clang++` and `llvm-config`.

Run
- The produced binary reads input (source file) and prints the generated IR or emits the compiled output depending on the visitor implementation. Example:
```bash
./raflang < source.ect
# or if the program writes IR to a file via the Visitor:
./raflang input.raf
```

Debugging & development tips
- Build with debug symbols while developing:
```bash
clang++ -g -O0 $(llvm-config --cxxflags) parser.tab.cpp scanner.c llvmvisitor.cpp nodes.cpp main.cpp \
  -o raflang-debug $(llvm-config --ldflags --libs core --system-libs)
```
- Use `llvm::verifyModule(*mod, &llvm::errs());` in the codegen to verify generated IR.
- Run under a debugger (`lldb`/`gdb`) or with sanitizers to find runtime issues:
```bash
clang++ -g -O1 -fsanitize=address,undefined $(llvm-config --cxxflags) ... -o raflang-sanitize $(llvm-config --ldflags --libs core --system-libs)
./raflang-sanitize < source.ect
```

Memory ownership notes
- The scanner and parser use Bison's `yylval` to pass semantic values. If the lexer allocates memory (e.g. `new std::string` or `strdup`) the parser should take ownership (move into AST nodes) or free it. Consider switching to Bison's C++ value API or use smart pointers in your AST for safer memory management.

License & contribution
- This is a work-in-progress — modify and experiment freely. If you want help adding features or cleaning up ownership, open an issue or ask in the repo.
