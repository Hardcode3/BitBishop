# Continuous Integration (CI)

This repository uses GitHub Actions to validate pull requests and publish
coverage after changes land on `main`.

## Quick PR Checks (default)

When a pull request targets `main`, the baseline checks run automatically.

- Linux build and tests on `ubuntu-24.04-arm` using `clang_debug` with
  `quick-validation-clang-debug`.
- `clang-tidy` linting on `ubuntu-24.04-arm` when the PR is updated.

These fast checks are the minimum expectation before review.

## Full Pre-Merge Checks (label-triggered)

When the change is ready for broader validation, add the label
`run-pre-merge-checks` to the pull request. This triggers the full
cross-platform matrix. The first labeled run focuses on release builds, and
subsequent pushes with the label present also include the Linux `clang_debug`
job.

Linux:

- `ubuntu-24.04` x64, `clang_release`, `quick-validation-clang-release`
- `ubuntu-24.04-arm` arm64, `clang_release`, `quick-validation-clang-release`
- `ubuntu-24.04-arm` arm64, `clang_debug`, `quick-validation-clang-debug`
  (runs on pushes while the label is present)

macOS:

- `macos-26-intel` intel, `clang_release`, `quick-validation-clang-release`
- `macos-26` arm64, `clang_release`, `quick-validation-clang-release`

Windows:

- `windows-2022` x64, `msvc_release`, `quick-validation-msvc-release`
- `windows-2025` x64, `msvc_release`, `quick-validation-msvc-release`
- `windows-11-arm` arm64, `msvc_release_arm64`, `quick-validation-msvc-release-arm64`

To stop the extra runs, remove the label.

## After Merge (main branch)

On every push (PR merge) to `main`, the coverage workflow runs on `ubuntu-24.04-arm` using
`clang_debug` and `quick-validation-clang-debug`. It publishes the HTML report
and shields badges to GitHub Pages, which feed the README coverage badges.

## Where to Look

- Workflow definitions live in `.github/workflows/`.
- The Actions tab in GitHub shows logs and artifacts for each run.
