# JackCompiler
My attempt at implementing a compiler for the Jack programming language, as specified in the [Nand To Tetris II](https://www.coursera.org/learn/nand2tetris2) MOOC. The implementation assumes that the given program is a correct.
## Usage
```sh
$ cd src
$ g++ *.cpp -o JackCompiler
$ JackCompiler [file | directory]
```
The translated VM files are put in the same directory as the input files.
