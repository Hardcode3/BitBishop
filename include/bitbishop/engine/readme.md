# About the `engine/` directory

## The core rule

Engine = decision making

This layer decides which move to play, not which moves are legal.

## What exactly goes into `engine/`

### Definition

engine/ contains logic that:

- searches the game tree
- evaluates positions
- selects the best move

This layer may:

- call generate_legal_moves()
- make/unmake moves on the Board
- maintain search state

This layer must not:

- implement chess rules
- generate attacks directly
- contain UI or protocol code

Typical responsibilities

- search algorithms:
- Minimax
- Alpha-Beta
- MCTS
- evaluation functions
- move ordering
- transposition tables
- time management (search side)

### Examples (good)

```cpp
// engine/search.cpp
Move search_best_move(Board&, SearchLimits);

// engine/eval.cpp
int evaluate(const Board&);
```

### Examples (bad — belongs elsewhere)

```cpp
void generate_legal_rook_moves(...); // ❌ movegen
constexpr Bitboard ROOK_RAYS[64][4]; // ❌ lookups
void uci_loop(); // ❌ interface
```
