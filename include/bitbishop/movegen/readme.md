# About the `movegen/` directory

## The core rule

Movegen = chess rules + king safety

This layer is responsible for generating only legal moves, by construction.

It sits above raw attack computation and below search/evaluation.

## What exactly goes into `movegen/`

### Definition

movegen/ contains logic that:

- generates legal moves only
- enforces check, pins, and king safety
- understands chess rules
- emits Move objects

This layer may:

- depend on Board
- depend on attacks/ and lookups/
- branch on runtime board state

This layer must not:

- contain search logic
- contain evaluation logic
- contain UI / protocol logic

Typical responsibilities

- generate_legal_moves()
- per-piece legal move generation:
- generate_legal_bishop_moves
- generate_legal_rook_moves
- …
- check detection (checkers, check_mask)
- pin detection (pinned, pin_ray)
- special rules:
- castling
- en passant
- promotions

### Examples (good)

```cpp
// movegen/generate_legal_moves.cpp
void generate_legal_moves(MoveList&, const Board&, Color);

// movegen/bishop_moves.cpp
void generate_legal_bishop_moves(
    MoveList&,
    const Board&,
    Color,
    Bitboard check_mask,
    Bitboard pinned,
    const Bitboard pin_ray[64]
);
```

### Examples (bad — belongs elsewhere)

```cpp
int evaluate_position(const Board&); // ❌ engine
Bitboard bishop_attacks(Square, Bitboard); // ❌ attacks
constexpr Bitboard KING_ATTACKS[64]; // ❌ lookups
```
