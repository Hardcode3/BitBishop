# CMake Presets Guide

This project uses **CMake Presets (v6)** to enforce consistent workflows (configure, build, test) across platforms. It abstracts platform differences (Clang/Ninja on Unix, MSVC on Windows) and enforces a tiered testing strategy.

## ⚡️ Cheat Sheet: Basic Usage

If you are new to CMake Presets, these are the commands you will use 99% of the time.

### Listing Available Presets

Before running anything, you can see what is available on your machine:

```bash
# List EVERYTHING (Configure, Build, Test, Package)
cmake --list-presets

# List only Configure presets (Debug, Release, etc.)
cmake --list-presets=configure

# List only Test presets (Quick, Deep, etc.)
ctest --list-presets

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

## 📂 Directory Layout

Presets enforce an **out-of-source** build structure.

```text
build/
  ├── clang_debug/    # Artifacts for Clang Debug
  ├── clang_release/  # Artifacts for Clang Release
  ├── msvc_debug/     # Artifacts for MSVC Debug
  ├── msvc_release/   # Artifacts for MSVC Release
  └── install/        # Staged install artifacts

```

## ⚙️ Available Configure Presets

These presets handle compiler selection, generator choice (Ninja vs VS), and toolchain injection.

| Preset              | Platform | Generator | Description                                        |
| ------------------- | -------- | --------- | -------------------------------------------------- |
| **`clang_debug`**   | Unix     | Ninja     | **Dev Mode.** `-O0`, `-g`, static archive forcing. |
| **`clang_release`** | Unix     | Ninja     | **Production.** Optimized build.                   |
| **`msvc_debug`**    | Windows  | VS 2022   | **Dev Mode.** Uses `x64-windows` triplet.          |
| **`msvc_release`**  | Windows  | VS 2022   | **Production.** Uses `x64-windows` triplet.        |

## 🧪 Test Presets Reference

The testing system is "Tiered". You select a preset based on **how much time you have** (Tier) and **which build you want to test** (Config).

### Naming Convention

Test presets follow this pattern:
`[tier]-validation-[configure-preset]`

### 1. The Tiers (How deep to test?)

| Tier Prefix            | Target Audience | Time  | Description                                                   |
| ---------------------- | --------------- | ----- | ------------------------------------------------------------- |
| **`quick-...`**        | Inner Loop      | ~Secs | Runs unit tests & basic smoke tests. **Run this constantly.** |
| **`intermediate-...`** | Pre-Commit      | ~Mins | Runs standard validation (e.g., standard Perft depths).       |
| **`deep-...`**         | CI / Nightly    | ~Hrs  | Runs exhaustive tests.                                        |
| **`full-suite-...`**   | Release         | Var   | Runs **all** tests (no filter).                               |

### 2. Available Presets List

#### Linux / macOS (Clang)

| Preset Name                             | Usage                                           |
| --------------------------------------- | ----------------------------------------------- |
| `quick-validation-clang-debug`          | **Daily Driver.** Fast checks on debug build.   |
| `intermediate-validation-clang-debug`   | deeper checks on debug build.                   |
| `deep-validation-clang-debug`           | Heavy validation (slow) on debug build.         |
| `full-suite-clang-debug`                | Run everything on debug.                        |
| `quick-validation-clang-release`        | Fast checks on optimized build.                 |
| `intermediate-validation-clang-release` | **Pre-Push.** Standard checks on release build. |
| `deep-validation-clang-release`         | **CI.** Exhaustive checks on release build.     |

#### Windows (MSVC)

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
3. Assigned the label **`tier_quick`**.

**How to add a simple unit test:**
Just create `tests/bitbishop/test_new_feature.cpp`. It will automatically run in the `quick-validation` presets.

### 2. Splitting Complex Tests (Overrides)

For heavy tests (like `test_perft`) that contain both fast and slow cases, we split the executable into multiple test entries using Google Test filters.

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

To create installable artifacts (creates `.zip`, `.tar.gz`, or installers depending on OS), use the release build preset with the `package` target.

```bash
# Unix
cmake --build --preset clang_release --target package

# Windows
cmake --build --preset msvc_release --target package

```

Artifacts are staged in `build/install/`.
