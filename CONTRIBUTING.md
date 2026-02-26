# Contributing to SolOS

Thank you for your interest in contributing to SolOS! We welcome contributions of all kinds, from bug fixes to new features and improvements. Before you start, please read the guidelines below.

---

### Source code location

Most of the source code is in the `src/` folder.

---

## Languages

SolOS is mainly written in **C** and **x86 assembly**. We encourage contributors to use these languages for new code.  

If you cannot use C or x86 assembly, we also allow **C++** and **Rust**, but please consider learning and using **C** and **x86 assembly** for compatibility and consistency with the rest of the OS.

---

## Coding Style

- Use **2 spaces** per indentation level.
- Follow the [`.clang-format`](.clang-format) rules (Google style) for formatting and more descriptive rules [`AGENTS.md`](AGENTS.md)
- Keep function and variable names **descriptive and consistent** with the existing code.

---

## Branch Naming

When creating a branch for your changes, use prefixes to indicate the type of work:

- `feature/` for new features  
  Example: `feature/keyboard-layout`
- `bugfix/` for bug fixes  
  Example: `bugfix/cursor-update`
- `doc/` for documentation changes  
  Example: `doc/README-update`
- `refactor/` for refactoring
  Example: `refactor/kernel.c-refactor`

This helps keep the repository organized and makes it easier to review PRs.

---

## Contributions Workflow

1. **Fork** the repository.
2. **Create a branch** using the appropriate prefix:  
   ```bash
   git checkout -b feature/my-feature
   ```
3. **Make your changes** on that branch.
4. **Commit** your changes with clear messages.
5. **Push** the branch to your fork:
    ```bash
    git push origin prefix/branch-name
    ```
6. **Open a Pull Request** against the ``main`` branch of SolOS

---

## Referenced files
- [README.md](README.md) - overview and getting started instructions.
- [LICENSE.md](LICENSE.md) - licensing terms.

## Code of Conduct
Please be respectful, constructive, and collaborative. SolOS is an open-source project for everyone.
