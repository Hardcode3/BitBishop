# About the `attacks/` directory

## The core rule

Lookups = immutable, compile-time geometry

Attacks = runtime logic that depends on board state

## What exactly goes into `attacks/`

### Definition

attacks/ contains functions that:

- consume lookup tables
- take runtime state (occupied)
- compute attack bitboards dynamically

### Examples (good)

```cpp
// attacks/bishop_attacks.cpp
Bitboard bishop_attacks(Square from, Bitboard occupied);

// attacks/attackers_to.cpp
Bitboard attackers_to(Square sq, const Board&, Color);
```

### Examples (bad — belongs elsewhere)

```cpp
constexpr Bitboard KING_ATTACKS[64]; // ❌ data only
generate_legal_moves(...) // ❌ rules layer
```
