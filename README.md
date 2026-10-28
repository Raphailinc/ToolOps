# ToolOps

[![CI](https://github.com/Raphailinc/ToolOps/actions/workflows/ci.yml/badge.svg)](https://github.com/Raphailinc/ToolOps/actions/workflows/ci.yml)

[English](README.md) · [Русский](README.ru.md)

A small C++ application for recording equipment and maintenance requests.

## What it does

- Add machines and maintenance requests.
- Update machine status and close requests.
- Persist records in TSV files under a configurable data directory.
- Build the domain library and CLI with CMake.

## Run locally

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/toolops_cli list
```

## Checks

```bash
ctest --test-dir build --output-on-failure
```

## Scope and implementation notes

Requires a C++17 compiler and CMake. `TOOLOPS_DATA` selects the storage directory. TSV storage keeps the example simple; it is designed as a local CLI rather than a concurrent multi-user database.
