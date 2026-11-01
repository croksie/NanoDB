# NanoDB

NanoDB is a database management system written in C++.

>The project is not finished yet.

## Features
- **SQL Parser & Executor**: `CREATE TABLE`, `DROP TABLE`, `INSERT INTO`, `SELECT` (with projections and `WHERE` clauses).
- **Data Types**: `Number`, `Varchar`.
- **Storage**: Slotted-page format (4KB pages) with disk persistence.
- **Buffer Manager**: In-memory cache with eviction policy.
- **System Relations**: Metadata tables (`Systable`, `Syscols`).
- **Console**: Interactive console (`.tables`, `.help`, `.exit`).

## Roadmap
- [ ] `UPDATE` and `DELETE` queries
- [ ] Reuse freed pages (Vaccum)
- [ ] Indexing
- [ ] Table joins (`JOIN`)
- [ ] Sorting and grouping (`ORDER BY`, `GROUP BY`)
- [ ] Transactions and WAL (ACID)
- [ ] More data types (Boolean, Float)

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
./bin/Debug/NanoDB.exe
```