# Debugging Move Generation with Perft

## TL;DR

**Perft discrepancies between Stockfish 17.1 and Bitbishop revealed a subtle bug in en passant move generation**.

By drilling down leaf nodes recursively, one can identify that **early filtering by check-mask and pin-mask** was **removing legal en passant moves**.

➡️ This document demonstrates a step-by-step approach to debug move generation using perft.

## Step 1 – Start with the position

Position:

```text
8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1
```

### Run Stockfish perft at depth 5

```text
Stockfish 17.1 by the Stockfish developers (see AUTHORS file)
uci
id name Stockfish 17.1
...
uciok
position fen 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1
go perft 5
...
e2e3: 45326
g2g3: 14747
a5a6: 59028
e2e4: 36889
g2g4: 53895
b4b1: 69665
b4b2: 48498
b4b3: 59719
b4a4: 45591
b4c4: 63781
b4d4: 59574
b4e4: 54192
b4f4: 10776
a5a4: 52943

Nodes searched: 674624
```

### Run Bitbishop perft at depth 5

```text
a5a4: 52943
a5a6: 59028
b4b1: 69653
b4b2: 48486
b4b3: 59708
b4a4: 45580
b4c4: 63770
b4d4: 59563
b4e4: 54181
b4f4: 10774
e2e3: 45326
e2e4: 36889
g2g3: 14747
g2g4: 53895

Nodes searched: 674543
```

### Compare perft outputs at depth 5

- Move print order differs (not an issue).
- Total node count differs (Stockfish: 674624 vs Bitbishop: 674543), suggesting a bug in legal move generation.
- Individual move counts differ:

```text
e2e3: 45326 ok
g2g3: 14747 ok
a5a6: 59028 ok
e2e4: 36889 ok
g2g4: 53895 ok
b4b1: 69665 vs 69653 in Bitbishop
b4b2: 48498 vs 48486 in Bitbishop
b4b3: 59719 vs 59708 in Bitbishop
b4a4: 45591 vs 45580 in Bitbishop
b4c4: 63781 vs 63770 in Bitbishop
b4d4: 59574 vs 59563 in Bitbishop
b4e4: 54192 vs 54181 in Bitbishop
b4f4: 10776 vs 10774 in Bitbishop
a5a4: 52943 vs 52943 in Bitbishop
```

There is a strong likelihood that these discrepancies are connected and that solving one will resolve the others.

## Step 2 – Drill down problematic moves

Start with `b4b1`.

The new position is:

```text
8/2p5/3p4/KP5r/5p1k/8/4P1P1/1R6 b - - 0 1
```

### Run Stockfish perft at depth 4

```text
f4f3: 4969
d6d5: 3833
c7c6: 4605
c7c5: 3791
h5b5: 965
h5c5: 4395
h5d5: 4195
h5e5: 4910
h5f5: 4320
h5g5: 4709
h5h6: 4104
h5h7: 4413
h5h8: 5020
h4g3: 4739
h4g5: 5342
h4g4: 5355

Nodes searched: 69665
```

### Run Bitbishop perft at depth 4

```text
h4g3: 4738
h4g4: 5354
h4g5: 5341
h5b5: 965
h5c5: 4395
h5d5: 4194
h5e5: 4909
h5f5: 4319
h5g5: 4708
h5h6: 4103
h5h7: 4412
h5h8: 5019
f4f3: 4968
d6d5: 3832
c7c6: 4605
c7c5: 3791

Nodes searched: 69653
```

### Compare perft outputs at depth 4

```text
f4f3: 4969 vs 4968 in Bitbishop
d6d5: 3833 vs 3832 in Bitbishop
c7c6: 4605 ok
c7c5: 3791 ok
h5b5: 965  ok
h5c5: 4395 ok
h5d5: 4195 vs 4194 in Bitbishop
...
h4g4: 5355 vs 5354 in Bitbishop
```

- Small differences appear across several moves, requiring deeper inspection.

### Drill down at depth 3

Using the move `f4f3`, the new position is now:

```text
8/2p5/3p4/KP5r/7k/5p2/4P1P1/1R6 w - - 0 1
```

#### Run Stockfish perft at depth 3

```text
e2e3: 276
g2g3: 67
a5a6: 350
e2e4: 278
g2g4: 302
e2f3: 217
g2f3: 235
b1a1: 320
b1c1: 366
b1d1: 352
b1e1: 270
b1f1: 312
b1g1: 270
b1h1: 58
b1b2: 254
b1b3: 293
b1b4: 35
a5a4: 364
a5b4: 350

Nodes searched: 4969
```

#### Run Bitbishop perft at depth 3

```text
a5a4: 364
a5b4: 349
a5a6: 350
b1a1: 320
b1c1: 366
b1d1: 352
b1e1: 270
b1f1: 312
b1g1: 270
b1h1: 58
b1b2: 254
b1b3: 293
b1b4: 35
e2e3: 276
e2e4: 278
e2f3: 217
g2g3: 67
g2g4: 302
g2f3: 235

Nodes searched: 4968
```

#### Compare perft outputs at depth 3

```text
a5b4: 350 vs 349 in Bitbishop
```

- Result differs, continue debugging

### Drill down at depth 2

Using move `a5b4`, the new position is:

```text
New position: 8/2p5/3p4/1P5r/1K5k/5p2/4P1P1/1R6 b - - 0 1
```

#### Run Stockfish perft at depth 2

```text
f3f2: 20
d6d5: 22
c7c6: 23
c7c5: 7
f3e2: 18
f3g2: 18
h5b5: 5
h5c5: 20
h5d5: 22
h5e5: 22
h5f5: 22
h5g5: 22
h5h6: 22
h5h7: 22
h5h8: 22
h4g3: 20
h4g5: 22
h4g4: 21

Nodes searched: 350
```

#### Run Bitbishop perft at depth 2

```text
h4g3: 20
h4g4: 21
h4g5: 22
h5b5: 5
h5c5: 20
h5d5: 22
h5e5: 22
h5f5: 22
h5g5: 22
h5h6: 22
h5h7: 22
h5h8: 22
f3f2: 20
f3e2: 18
f3g2: 18
d6d5: 22
c7c6: 23
c7c5: 6

Nodes searched: 349
```

#### Compare perft outputs at depth 2

```text
c7c5: 7 vs 6 in Bitbishop
```

## Step 3 – Identify the bug

Drilling down to `c7c5` leaf, the new position is:

```text
8/8/3p4/1Pp4r/1K5k/5p2/4P1P1/1R6 w - c6 0 1
```

### Run Stockfish perft at depth 1

```text
b5c6: 1
b4a3: 1
b4b3: 1
b4c3: 1
b4a4: 1
b4c4: 1
b4a5: 1

Nodes searched: 7
```

### Run Bitbishop perft at depth 1

```text
b4a3: 1
b4b3: 1
b4c3: 1
b4a4: 1
b4c4: 1
b4a5: 1

Nodes searched: 6
```

### Compare perft outputs at depth 1

- `b5c6` en passant move is missing in Bitbishop.

Enter the FEN used at depth 1 into a fen viewer and you'll be able to analyze which move is missing, telling you the rule that's wrong and therefore the probable location in the codebase.

### Find the bug

Original en passant generation code:

```cpp
inline void generate_en_passant(std::vector<Move>& moves, Square from, Color us, const Board& board, Square king_sq,
                                const Bitboard& check_mask, const Bitboard& pin_mask) {
  const std::optional<Square> epsq_opt = board.en_passant_square();

  if (!epsq_opt || !can_capture_en_passant(from, epsq_opt.value(), us)) {
    return;
  }

  const Color them = ColorUtil::opposite(us);
  Square epsq = epsq_opt.value();
  auto bb = Bitboard(epsq);
  bb &= check_mask;
  bb &= pin_mask;

  if (!bb) {
    return;
  }

  Square cap_sq = (us == Color::WHITE) ? Square(epsq.flat_index() - Const::BOARD_WIDTH)
                                       : Square(epsq.flat_index() + Const::BOARD_WIDTH);

  Board tmp(board);
  tmp.remove_piece(cap_sq);
  tmp.move_piece(from, epsq);

  Bitboard attackers = generate_attacks(tmp, them);

  if (!attackers.test(king_sq)) {
    moves.emplace_back(Move::make_en_passant(from, epsq));
  }
}
```

🔎 In the original code, the **`check_mask` and `pin_mask` are applied before actually making the en passant capture**. They are only used for early return afterwards.

❌ This can incorrectly block legal en passant moves because, until the move is executed, the king might appear to be in check or the pawn in a pin, even though performing the en passant would resolve the check or unpin the pawn.

💡 In particular, when the pawn that created the en passant square is giving a check, filtering too early causes the move to be ignored, even though it is fully legal once executed.

By **removing the early king-safety check**, we ensure that only truly illegal moves are excluded, and all valid en passant captures are generated.

### Fix the code

```cpp
inline void generate_en_passant(std::vector<Move>& moves, Square from, Color us, const Board& board, Square king_sq,
                                const Bitboard& check_mask, const Bitboard& pin_mask) {
  const std::optional<Square> epsq_opt = board.en_passant_square();

  if (!epsq_opt || !can_capture_en_passant(from, epsq_opt.value(), us)) {
    return;
  }

  const Color them = ColorUtil::opposite(us);
  Square epsq = epsq_opt.value();
  Square cap_sq = (us == Color::WHITE) ? Square(epsq.flat_index() - Const::BOARD_WIDTH)
                                       : Square(epsq.flat_index() + Const::BOARD_WIDTH);

  Board tmp(board);
  tmp.remove_piece(cap_sq);
  tmp.move_piece(from, epsq);

  Bitboard attackers = generate_attacks(tmp, them);

  if (!attackers.test(king_sq)) {
    moves.emplace_back(Move::make_en_passant(from, epsq));
  }
}
```

➡️ The **unnecessary early filtering is removed**, allowing all legal en passant moves to be generated.

## **Conclusion**

Using perft as a **debugging tool**, we:

1. Compared total node counts between engines.
2. Drilled down moves using the discrepancies.
3. Recursively examined leaf nodes until the missing move was found.
4. Corrected early filtering in en passant generation.

➡️ Perft counts now match Stockfish, and legal move generation is correct.
