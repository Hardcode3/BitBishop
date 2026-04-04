# About the `engine/` directory

## Purpose

`engine/` **explores and ranks best moves** once legal moves already exist.

This layer **evaluates positions**, **searches the game tree**, and **returns the best
move it can find**.

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
    Interface("`
        **Interface**
        UCI protocol and search orchestration
        -
        inlcude/bitbishop/interface/*.hpp
    `")

    MoveGen --> Other1

    Moves --> MoveGen

    Engine --> Moves
    Engine --> MoveGen

    Interface --> Engine

    Other2 --> Interface
```

## Responsibilities

- **Evaluate positions**
- **Search the game tree**
- **Choose the best move available within the current limits**

## Inputs

- `movegen/` for legal moves
- `moves/` for apply/revert history during search
- `Board` and evaluation data from the core headers

## Outputs

- Best-move decisions
- Scores used by the protocol layer or analysis tools
