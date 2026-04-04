# About the `moves/` directory

## Purpose

`moves/` describes **how an already chosen `Move` changes the board** and **how to
undo that change safely**.

The boundary is:

- `movegen/` decides whether a move is legal
- `moves/` applies, records, and reverts that move

## Place in the architecture

```mermaid
flowchart TD

    Other1("`...`")
    Other2("`...`")
    MoveGen("`
        **Move Generation**
        Legal move generation and king safety
        -
        inlcude/bitbishop/movegen/*.hpp
    `")
    Moves("`
        **Move Execution**
        Reversible move execution and move history
        -
        inlcude/bitbishop/moves/*.hpp
    `")
    Engine("`
        **Search Engine**
        Evaluation and search
        -
        inlcude/bitbishop/engine/*.hpp
    `")
    Tools("`
        **Dev tools**
        Perft and other developer tools
        -
        inlcude/bitbishop/tools/*.hpp
    `")

    MoveGen --> Other1

    Moves --> MoveGen

    Engine --> MoveGen
    Engine --> Moves

    Tools --> MoveGen
    Tools --> Moves

    Other2 --> Engine

```

## Responsibilities

- **Expand a chosen `Move` into low-level board effects**
- **Apply and revert those effects deterministically**
- Preserve enough **history for search rollback and repetition tracking**

## Inputs

- `Board`, `Move`, `Piece`, `Square`, and `BoardState`
- `zobrist.hpp` for repetition tracking and position identity

## Outputs

- Reversible move executions
- Search-friendly position history
- Updated board state after application or rollback
