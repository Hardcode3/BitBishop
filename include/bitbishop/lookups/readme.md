# About the `lookups/` directory

## The core rule

Lookups = immutable, compile-time geometry

Attacks = runtime logic that depends on board state

## What exactly goes into `lookups/`

### Definition

lookups/ contains only data:

- constexpr
- no branching on board state
- no Board, no occupied
- no game rules

### Examples (good)

```cpp
// lookups/king_attacks.hpp
constexpr Bitboard KING_ATTACKS[64];

// lookups/rook_rays.hpp
constexpr Bitboard ROOK_RAYS[64][4];

// lookups/between_squares.hpp
constexpr Bitboard BETWEEN[64][64];
```

### Examples (bad — belongs elsewhere)

```cpp
Bitboard bishop_attacks(Square, Bitboard); // ❌ logic
if (occupied & mask) { ... } // ❌ runtime state
```
