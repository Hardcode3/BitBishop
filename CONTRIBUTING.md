# Contributing to BitBishop

Thanks for your interest in contributing.

BitBishop is primarily a learning project, but outside contributions are very
welcome when they are clear, focused, and aligned with the project structure.
Good contributions are not limited to code: tests, documentation, bug reports,
and design feedback are all valuable.

## Before You Start

For small fixes, typo corrections, documentation updates, or narrowly scoped
tests, feel free to open a pull request directly.

For larger work, please open an issue first so the design and scope can be
aligned before implementation. This is especially helpful for:

- search changes
- evaluation heuristics
- architecture refactors
- new protocol features
- build system or CI changes

## Development Setup

### Prerequisites

- CMake 3.27 or newer
- vcpkg with `VCPKG_ROOT` configured
- Clang plus Ninja on Unix-like systems, or Visual Studio 2022 on Windows

Optional tools:

- `clang-format`
- `clang-tidy`
- `llvm-cov`
- `llvm-profdata`

### Configure and Build

Unix-like systems:

```bash
cmake --preset clang_debug
cmake --build --preset clang_debug
```

Windows:

```powershell
cmake --preset msvc_debug
cmake --build --preset msvc_debug
```

For the full preset matrix, coverage workflow, and packaging notes, see
[docs/cmake.md](./docs/cmake.md).

## Running Tests

Run the quick validation suite before opening a pull request.

Unix-like systems:

```bash
ctest --preset quick-validation-clang-debug
```

Windows:

```powershell
ctest --preset quick-validation-msvc-debug
```

Depending on the kind of change, you may also want to run deeper validation:

- `intermediate-validation-*` for standard perft coverage
- `deep-validation-*` for exhaustive validation
- `full-suite-*` when you want the broadest confidence

## CI Validation

Pull requests targeting `main` run a baseline CI set on GitHub Actions:

- Linux `clang_debug` build with `quick-validation-clang-debug` on `ubuntu-24.04-arm`
- `clang-tidy` linting on `ubuntu-24.04-arm` when the PR is updated

When you are close to merge, add the label `run-pre-merge-checks` to the pull
request to trigger the full cross-platform suite (Linux, macOS, and Windows
release builds). The label stays active, so new pushes will re-run the full
matrix until it is removed.

After the PR is merged, pushes to `main` publish coverage to GitHub Pages and
refresh the README badges.

For the full matrix and workflow definitions, see [docs/ci.md](./docs/ci.md).

### What to Test

- Changes in `attacks/`, `lookups/`, `movegen/`, or `moves/` should usually
  come with targeted unit tests.
- Changes affecting legal move generation should be validated with perft when
  possible.
- Interface changes should include or update UCI-facing tests when relevant.
- Documentation-only changes do not require tests, but links and examples
  should still be checked carefully.

## Project Architecture

BitBishop is intentionally structured by responsibility. Before making changes,
please read the architecture guide:

- [include/bitbishop/readme.md](./include/bitbishop/readme.md)

As a rule of thumb:

- `lookups/` is for compile-time geometry
- `attacks/` is for occupancy-aware attack queries
- `movegen/` is for legal move generation
- `moves/` is for applying and reverting chosen moves
- `engine/` is for evaluation and search
- `interface/` is for UCI and external control
- `tools/` is for developer-facing validation helpers

If a change crosses these boundaries, please explain why in the pull request.

## Contribution Guidelines

### Keep Pull Requests Focused

Please prefer small, reviewable pull requests over large mixed changes.

Good examples:

- one bug fix with tests
- one refactor in a single subsystem
- one documentation update tied to a specific feature

Less helpful examples:

- unrelated cleanup mixed with functional changes
- formatting-only edits across the whole repository in a feature PR
- architecture changes without a written explanation

### Match the Existing Style

- Follow the existing naming and file layout conventions.
- Keep comments useful and concise.
- Prefer readable, explicit code over clever but opaque code.
- Preserve the current architectural boundaries unless there is a strong reason
  to change them.

If formatting or linting is available in your environment, please run:

```bash
cmake --build --preset <build-preset> --target clang-format
cmake --build --preset <build-preset> --target clang-tidy
```

Example:

```bash
cmake --build --preset clang_debug --target clang-format
cmake --build --preset clang_debug --target clang-tidy
```

## Documentation Expectations

Please update documentation when your change affects:

- public behavior
- build or test workflow
- architecture or folder responsibilities
- protocol behavior
- debugging or contributor workflow

Relevant documentation lives in:

- [README.md](./README.md)
- [docs/](./docs/)
- [include/bitbishop/](./include/bitbishop/)

## Pull Request Checklist

Before requesting review, please make sure that:

- the change is scoped to one main concern
- the code builds on the relevant preset
- the relevant tests pass locally
- new behavior is covered by tests when appropriate
- documentation is updated when needed
- the pull request description explains the motivation and the validation steps

The repository already provides a pull request template in
[.github/pull_request_template.md](./.github/pull_request_template.md).

## Reporting Bugs and Suggesting Features

If you are not ready to contribute code, issues are still very helpful.

For bugs, please include:

- reproduction steps
- a FEN when relevant
- the expected behavior
- the actual behavior
- platform and compiler details when they matter

If you believe the bug has a security impact, please do not file it publicly.
Use [SECURITY.md](./SECURITY.md) instead.

For features, please describe:

- the goal
- the subsystem affected
- the intended behavior
- any constraints or tradeoffs you already see

The existing issue templates under [`.github/ISSUE_TEMPLATE/`](./.github/ISSUE_TEMPLATE/)
are a good starting point.

## Code of Conduct

By participating in this project, you agree to follow the
[Code of Conduct](./CODE_OF_CONDUCT.md).
