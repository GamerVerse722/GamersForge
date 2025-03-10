# BMapping (WIP)
- ⚠ Work in progress - Not Functional Yet ⚠

## Overview
- BMapping is a program for vex pros which similifies registering button actions.

## Installation
- To install BMapping run these program in intergrated terminal in vex pros.

    ```
    pros c add-depot BMapper https://example.com
    pros c apply BMapper
    ```

## Developer Notes

### Build Instruction
- To build a template, run:

    ```
    pros make temple
    ```

### Makefile Configuration  
- Customize the `Makefile` by modifying the following variables:

- **`IS_LIBRARY`** – Defines if the program is a library.
  - Example: `IS_LIBRARY:=1` *(1 = true, 0 = false)*

- **`LIBNAME`** – Sets the library name.
  - Example: `LIBNAME:=BMapper`
  
- **`VERSION`** – Specifies the current version of the library.
  - Example: `VERSION:=1.0.0`
