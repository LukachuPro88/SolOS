# AGENTS.md
# Codex Agent Instructions for C/C++ Projects

## Purpose
This file provides instructions for the Codex AI to maintain consistent coding style, formatting, and best practices in C and C++ projects.

---

## General Rules for C++
- Always follow the **Google C++ Style Guide**: https://google.github.io/styleguide/cppguide.html
- Indentation: **2 spaces per level**.
- Braces:
  - Opening brace `{` goes at the end of the control statement line.
  - Closing brace `}` goes on its own line.
- Line length: max **80 characters** (soft wrap at 100 if necessary).
- Use **PascalCase for class names** and **camelCase for functions and variables**.
- Always include **header guards** or `#pragma once` in header files.
- Constants: Use `kConstName` format.

---

## General Rules for C
- Always follow the **Google C Style Guide**
- Indentation: **2 spaces per level**.
- Braces:
  - Opening brace `{` goes at the end of the control statement line.
  - Closing brace `}` goes on its own line.
- Line length: max **80 characters** (soft wrap at 100 if necessary).
- Use **PascalCase for struct/typedef names** and **snake_case for functions and variables**.
- Always include **header guards** or `#pragma once` in header files.
- Constants: Use `kConstName` format.

---

## Functions
- Use descriptive function names.
- Keep functions **short and single-purpose** (<=50 lines ideally).
- Include **doxygen-style comments** for every function:
```cpp
/// Brief description of the function
/// @param param_name description
/// @return description