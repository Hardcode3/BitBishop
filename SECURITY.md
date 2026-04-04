# Security Policy

## Supported Versions

BitBishop is under active development and does not currently maintain multiple supported release lines.

Security fixes, when available, are provided on a best-effort basis for:

- the latest state of the default branch

Older commits, forks, and unpublished local modifications should generally be considered unsupported.

## Reporting a Vulnerability

Please do **not** report security vulnerabilities through public GitHub issues, pull requests, or discussions.

Instead, report them privately to the project maintainer:

- GitHub: [@Hardcode3](https://github.com/Hardcode3)

If GitHub private vulnerability reporting is enabled for the repository, prefer using that channel. Otherwise, contact the maintainer privately through GitHub.

## What to Include

When reporting a vulnerability, please include as much of the following as you can:

- a clear description of the issue
- affected files, components, or workflows
- steps to reproduce
- proof of concept, logs, or screenshots when appropriate
- impact assessment
- any suggested mitigation or fix, if you have one

If the issue depends on a particular environment, please also include:

- operating system
- compiler and version
- CMake preset or build configuration
- dependency or toolchain details if relevant

## Response Expectations

This project is maintained on a best-effort basis. There is currently no formal SLA for triage or remediation, but reports will be reviewed as promptly as possible.

The maintainer may:

- ask follow-up questions
- confirm whether the issue is in scope
- work with you on a fix and disclosure timeline
- decide that the report is low-risk or not applicable to the project

## Disclosure Guidelines

Please allow time for investigation and, when appropriate, a fix before making the issue public.

Responsible disclosure helps protect users, contributors, and downstream consumers of the project.

## Scope

Security-sensitive reports may include issues involving:

- memory safety problems
- undefined behavior leading to exploitable crashes
- unsafe parsing or malformed input handling
- vulnerabilities in the UCI-facing interface
- supply-chain or dependency concerns in the build workflow
- CI or packaging issues that could affect artifact integrity

General correctness bugs, strength regressions, illegal move generation, or engine evaluation mistakes should usually be reported through the normal issue tracker unless they also create a genuine security impact.
