# CMake Presets Guide

This project uses **CMake Presets (v6)** to enforce consistent workflows (configure, build, test) across platforms. It abstracts platform differences (Clang/Ninja on Unix, MSVC on Windows) and enforces a tiered testing strategy.

## 📋 Cheat Sheet

If you are new to CMake Presets, these are the commands you will use 99% of the time.

### Listing Available Presets

Before running anything, you can see what is available on your operating system:

```bash
cmake --list-presets=configure
cmake --list-presets=build
ctest --list-presets=test
```

### The Standard Workflow

1. **Configure** (Initialize the build folder):

```bash
cmake --preset <configure-preset>
# Example: cmake --preset clang_debug
```

2. **Build** (Compile the code):

```bash
cmake --build --preset <configure-preset>
# Example: cmake --build --preset clang_debug
```

3. **Test** (Run the test suite):

```bash
ctest --preset <test-preset>
# Example: ctest --preset quick-validation-clang-debug
```

## 🗂️ Directory Layout

Each build preset have its dedicated build directory to enable easy switching between presets and prevent artifact contamination.

```text
build/
  ├── clang_debug/    # Artifacts for Clang Debug
  ├── clang_release/  # Artifacts for Clang Release
  ├── msvc_debug/     # Artifacts for MSVC Debug
  ├── msvc_release/   # Artifacts for MSVC Release
  └── install/        # Staged install artifacts
```

## Custom build targets overview

Several build targets are available to ease developer's life on many aspects.

Run a target using the command:

```bash
cmake --build --preset <build-preset> --target <target-name>
# Example: cmake --build --preset clang_debug --target coverage-html
```

| Cmake Build Target    | Platform | Build Preset  | Description                                                                                                                            |
| --------------------- | -------- | ------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| `clang-format`        | Any      | Any           | Runs the formatter on **all** project's files.                                                                                         |
| `clang-tidy`          | Any      | Any           | Runs the linter on **all** project's files.                                                                                            |
| `coverage-html`       | Unix     | `clang_debug` | Runs coverage and exports an **html coverage report** to the build files.                                                              |
| `coverage-json`       | Unix     | `clang_debug` | Runs coverage and exports an **json coverage report** to the build files.                                                              |
| `coverage-markdown`   | Unix     | `clang_debug` | Runs coverage and exports an **markdown coverage report** to the build files.                                                          |
| `coverage-shieldsio`  | Unix     | `clang_debug` | Runs coverage and **exports a shields.io coverage badge** (json format) with the correct color and values to the build files.          |
| `coverage-summary`    | Unix     | `clang_debug` | Runs coverage and **prints coverage report to the console**.                                                                           |
| `_coverage-merge`     | Unix     | `clang_debug` | Internal target to merge coverage files before reporting. Dependency of the `_coverage-*` targets.                                     |
| `_coverage-run-tests` | Unix     | `clang_debug` | Internal target to run quick tiered tests automatically before running merge and report steps. Dependency of `_coverage-merge` target. |

### Running code coverage non-quick tiered tests

In some rare scenarios, you may want to use the coverage associated to other [tests tiers](#-test-presets-reference).

Reconfigure cmake while passing the non-default `CTEST_PRESET`:

```bash
cmake --preset clang_debug -DCTEST_PRESET=<test-preset>
# Example: cmake --preset clang_debug -DCTEST_PRESET=intermediate-validation-clang-debug
```

Run the specific target as usual:

```bash
cmake --build --preset clang_debug --target <target-name>
# Example: cmake --build --preset clang_debug --target coverage-html
```

Note that coverage is only available in `clang_debug` configuration.

Associated [test tiers](#-test-presets-reference) are:

- `quick-validation-clang-debug`
- `intermediate-validation-clang-debug`
- `deep-validation-clang-debug`
- `full-suite-clang-debug`

## 🛠️ Cmake presets overview

### ⚙️ Configure Presets

These presets handle compiler selection, generator choice and toolchain injection.

| Preset              | Platform | Generator | Description                                                        |
| ------------------- | -------- | --------- | ------------------------------------------------------------------ |
| **`clang_debug`**   | Unix     | Ninja     | **Dev Mode** - Disables all compiler optimizations for easy debug. |
| **`clang_release`** | Unix     | Ninja     | **Production** Optimized build.                                    |
| **`msvc_debug`**    | Windows  | VS 2022   | **Dev Mode** Uses `x64-windows` triplet.                           |
| **`msvc_release`**  | Windows  | VS 2022   | **Production** Uses `x64-windows` triplet.                         |

### 👷 Build Presets

| Preset              | Platform | Generator | Description    |
| ------------------- | -------- | --------- | -------------- |
| **`clang_debug`**   | Unix     | Ninja     | **Dev Mode**   |
| **`clang_release`** | Unix     | Ninja     | **Production** |
| **`msvc_debug`**    | Windows  | VS 2022   | **Dev Mode**   |
| **`msvc_release`**  | Windows  | VS 2022   | **Production** |

### 🧪 Test Presets Reference

The testing system is "Tiered". You select a preset based on **how much time you have** (Tier) and **which build you want to test** (Config).

> [!TIP]
> Switching the active test preset in VSCode with the cmake extension will automatically update the tests discovery view and you'll only be able to run tests for the selected preset.

#### Naming Convention

Test presets follow this pattern:
`[tier]-validation-[configure-preset]`

#### 1. The Tiers (How deep to test?)

| Tier Prefix            | Target Audience | Time  | Description                                                   |
| ---------------------- | --------------- | ----- | ------------------------------------------------------------- |
| **`quick-...`**        | Inner Loop      | ~Secs | Runs unit tests & basic smoke tests. **Run this constantly.** |
| **`intermediate-...`** | Pre-Commit      | ~Mins | Runs standard validation (e.g., standard Perft depths).       |
| **`deep-...`**         | CI / Nightly    | ~Hrs  | Runs exhaustive tests.                                        |
| **`full-suite-...`**   | Release         | Var   | Runs **all** tests (no filter).                               |

#### 2. Available Presets List

##### Linux / macOS (Clang)

| Preset Name                             | Usage                                           |
| --------------------------------------- | ----------------------------------------------- |
| `quick-validation-clang-debug`          | **Daily Driver.** Fast checks on debug build.   |
| `intermediate-validation-clang-debug`   | deeper checks on debug build.                   |
| `deep-validation-clang-debug`           | Heavy validation (slow) on debug build.         |
| `full-suite-clang-debug`                | Run everything on debug.                        |
| `quick-validation-clang-release`        | Fast checks on optimized build.                 |
| `intermediate-validation-clang-release` | **Pre-Push.** Standard checks on release build. |
| `deep-validation-clang-release`         | **CI.** Exhaustive checks on release build.     |

##### Windows (MSVC)

| Preset Name                            | Usage                                           |
| -------------------------------------- | ----------------------------------------------- |
| `quick-validation-msvc-debug`          | **Daily Driver.** Fast checks on debug build.   |
| `intermediate-validation-msvc-debug`   | Deeper checks on debug build.                   |
| `full-suite-msvc-debug`                | Run everything on debug.                        |
| `quick-validation-msvc-release`        | Fast checks on optimized build.                 |
| `intermediate-validation-msvc-release` | **Pre-Push.** Standard checks on release build. |
| `deep-validation-msvc-release`         | **CI.** Exhaustive checks on release build.     |

## 🏗 Developer Guide: How Tests are Discovered

The project uses a custom registration system in `tests/CMakeLists.txt` to automatically assign Tiers.

### 1. Auto-Discovery (The Default)

Any file named `test_*.cpp` in `tests/` is automatically:

1. Compiled into an executable.
2. Registered as a CTest.
3. Assigned the labels **`unit` and`tier_quick`**.

**How to add a simple unit test?**

Just create `tests/bitbishop/test_new_feature.cpp` and reload cmake.
It will automatically run in the `quick-validation` presets.

### 2. Splitting Complex Tests (Overrides)

For heavy tests (like `test_perft`) that contain both fast and slow cases, executable are split into multiple test entries using Google Test filters.

This is configured in `tests/CMakeLists.txt` via `CTEST_ENTRIES`:

```cmake
# Format: "binary_name | gtest_filter | labels"
set(CTEST_ENTRIES
    # Map specific GTest cases to Tiers
    "test_perft|Smoke/PerftSmokeTest.*|perft,tier_quick"
    "test_perft|Validation/PerftValidationTest.*|perft,tier_intermediate"
    "test_perft|Exhaustive/PerftExhaustiveTest.*|perft,tier_deep"
)
```

**How this works:**

- `Smoke/...` tests run in **Quick** presets.
- `Validation/...` tests run in **Intermediate** presets.
- `Exhaustive/...` tests run only in **Deep** presets.

## 📦 Packaging

> [!WARNING]
> Packaging is not configured yet, it's a very naïve implementation as a placeholder for future presets.

To create installable artifacts (creates `.zip`, `.tar.gz`, or installers depending on OS), use the release build preset with the `package` target.

```bash
# Unix
cmake --build --preset clang_release --target package

# Windows
cmake --build --preset msvc_release --target package

```

Artifacts are staged in `build/install/`.
