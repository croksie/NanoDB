# NanoDB

NanoDB is a database management system written in C++.

The project is not finished yet, there are no optimizations and no indexing.
The sql language is not fully implemented, it only support "SELECT * FROM ...;" statements.

## Getting Started
Visual studio 2022 or later is recommended. The project use CMake so you can use any IDE, you just need CMake installed and a C++ compiler.

## How to build
### In Visual Studio
1. Select NanoDB.exe in target
2. Hit the run button

### In terminal
```bash
cmake --build build --config Debug
```

## How to run
```bash
./x64/Debug/NanoDB.exe
```