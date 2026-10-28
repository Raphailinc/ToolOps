# ToolOps

[![CI](https://github.com/Raphailinc/ToolOps/actions/workflows/ci.yml/badge.svg)](https://github.com/Raphailinc/ToolOps/actions/workflows/ci.yml)

Мини-клиент для учёта оборудования и заявок на обслуживание. Хранит данные в TSV, работает через CLI и собирается CMake-ом без зависимостей.

## Что нового
- Полный рефактор: вместо тяжёлого Qt проекта — чистый C++17 с библиотекой `toolops` и CLI `toolops_cli`.
- Простая модель и файловое хранилище (`data/machines.tsv`, `data/requests.tsv`), директория настраивается переменной `TOOLOPS_DATA`.
- Команды: `list`, `add-machine`, `add-request`, `set-status`, `close-request`.
- Тесты на лёгком мини-фреймворке (single-header) + CTest.
- Современная сборка через CMake, сборка как библиотеки + исполняемого файла.

## Быстрый старт
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# пример работы
./build/toolops_cli add-machine "Lathe A" "Shop1" running
./build/toolops_cli add-request 1 "Replace belt"
./build/toolops_cli list
./build/toolops_cli set-status 1 maintenance
./build/toolops_cli close-request 1
```

Данные по умолчанию лежат в `data/`. Можно переопределить:
```bash
export TOOLOPS_DATA=/tmp/toolops_data
./build/toolops_cli list
```

## Команды CLI
- `list` — показать машины и заявки.
- `add-machine <name> <location> [status]`
- `add-request <machine_id> <description>`
- `set-status <machine_id> <status>`
- `close-request <request_id>`

## Тесты
```bash
cmake --build build --target toolops_tests
ctest --test-dir build
```

## Структура
- `src/domain.hpp/.cpp` — модели и утилиты.
- `src/storage.hpp/.cpp` — хранение в TSV.
- `src/main.cpp` — CLI.
- `tests/` — минимальный тестовый раннер и проверки хранилища.
