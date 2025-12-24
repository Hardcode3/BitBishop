# About the `moves/` directory (legacy / deprecated)

## Status

⚠️ Legacy code — kept intentionally

This directory contains an older pseudo-legal move generation system, followed by post-filtering for legality.

It is no longer the primary move generation path, but is preserved for:

- correctness reference
- debugging
- performance comparison
- educational purposes

## What this folder represents

`moves/` implements the classic two-step approach: 1. Generate pseudo-legal moves 2. Filter out illegal moves by:

- making the move
- checking king safety
- undoing the move

This approach is simple and correct, but:

- slower
- harder to optimize
- less suitable for high-performance engines

## Why this folder still exists

This code is intentionally kept because it:

- is known to work
- provides a correctness baseline
- allows perft and move comparison against the new generator
- is useful for profiling and regression testing

The new optimized legal move generator lives in `movegen/`.

## What exactly goes into `moves/`

### Definition

This folder contains logic that:

- generates pseudo-legal moves per piece
- filters moves via make/unmake
- does not use pins or check masks
- prioritizes simplicity over performance

It may:

- use Board
- generate Move objects
- temporarily allow illegal states

It must not:

- be used in the main engine search loop
- be mixed with the optimized movegen
- grow with new features

## Migration note

All new development related to move generation must go into:

`movegen/`

This folder is frozen unless a bug fix or reference improvement is required.
