# ToolOps

[![CI](https://github.com/Raphailinc/ToolOps/actions/workflows/ci.yml/badge.svg)](https://github.com/Raphailinc/ToolOps/actions/workflows/ci.yml)

[English](README.md) · [Русский](README.ru.md)

Небольшое C++17-приложение учёта оборудования и заявок на обслуживание.

## Возможности

- Добавление оборудования и заявок.
- Изменение состояния оборудования и закрытие заявок.
- Хранение в TSV-файлах в настраиваемом каталоге.
- Сборка библиотеки предметной области и CLI через CMake.

## Локальный запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/toolops_cli list
```

## Проверки

```bash
ctest --test-dir build --output-on-failure
```

## Условия запуска и границы проекта

Нужны компилятор C++17 и CMake. TOOLOPS_DATA задаёт каталог данных. TSV упрощает учебный пример; это локальная CLI-программа, а не многопользовательская БД с конкурентной записью.
