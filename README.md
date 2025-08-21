# GamersForge

## Overview

- GamersForge is a program for vex pros which simplifies registering button actions.

## Installation

- To install GamersForge run these program in integrated terminal in vex pros.

```
pros c add-depot GamersForge https://raw.githubusercontent.com/GamerVerse722/GamersForge/refs/heads/depot/stable.json
pros c apply GamersForge
```

## Developer Notes

### Build Instruction

- To build a template, run:

  ```
  pros make template
  ```

### Makefile Configuration

- Customize the `Makefile` by modifying the following variables:

- **`IS_LIBRARY`** – Defines if the program is a library.

  - Example: `IS_LIBRARY:=1` _(1 = true, 0 = false)_

- **`LIBNAME`** – Sets the library name.
  - Example: `LIBNAME:=BMapper`
- **`VERSION`** – Specifies the current version of the library.
  - Example: `VERSION:=1.0.0`
