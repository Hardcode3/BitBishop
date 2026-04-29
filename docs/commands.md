# BitBishop Command Reference

This document lists the commands currently implemented by BitBishop's text
interface (UCI + project-specific extensions).

The engine reads one command per line from `stdin` and writes responses to
`stdout`.

## Quick Start

Example interactive session:

```text
uci
isready
position startpos moves e2e4 e7e5
go depth 4
quit
```

## General Parsing Rules

- Commands are tokenized using whitespace splitting.
- Extra spaces/tabs/carriage returns are tolerated.
- Empty lines are ignored.
- Commands are case-sensitive (`uci` works, `UCI` does not).
- A command is only processed when a newline is received.
- Unknown commands are ignored silently.

## Startup Output

When the loop starts, BitBishop prints a startup banner:

```text
BitBishop <version> by <authors>.
```

This line is an engine banner, not a standard UCI response.

## UCI Commands

### `uci`

Initial UCI handshake.

Syntax:

```text
uci
```

Response:

```text
id name BitBishop
id author Hardcode (Baptiste Penot)
uciok
```

### `isready`

Readiness probe.

Syntax:

```text
isready
```

Response:

```text
readyok
```

### `ucinewgame`

Resets internal board state to the standard starting position.

Syntax:

```text
ucinewgame
```

Response: none

### `position`

Sets the current board state.

Syntax:

```text
position startpos [moves <move1> <move2> ...]
position fen <piece-placement> <side-to-move> <castling> <en-passant> <halfmove> <fullmove> [moves <move1> <move2> ...]
```

Examples:

```text
position startpos
position startpos moves e2e4 e7e5 g1f3
position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
```

Behavior notes:

- If `startpos` or `fen` is missing/invalid at parse level, the command is ignored.
- After `moves`, moves are applied left-to-right.
- If one move is invalid, processing stops at that move (earlier moves stay applied).
- No explicit success/error output is emitted.

### `go`

Starts search on the current position.

Syntax:

```text
go [depth <n>] [movetime <ms>] [wtime <ms>] [btime <ms>] [winc <ms>] [binc <ms>] [infinite]
```

Implemented behavior (current state):

- `depth <n>`: fixed-depth search.
- `infinite`: iterative deepening until `stop`.
- `movetime <ms>`: iterative deepening search limited by a per-move budget in milliseconds.
- Non-positive `movetime` values are clamped to `1` ms.
- `movetime` takes precedence over `wtime`/`btime`/`winc`/`binc`.
- `infinite` takes precedence over every other limit, including `movetime` and `depth`.
- If both `depth` and a time-based limit are provided, the current implementation runs the timed search path.
- When `movetime` is not provided, time is estimated from the side-to-move clock:
  roughly remaining time divided across future moves, with increment added and a safety reserve kept aside.

- If no argument is provided behind `go`, search defaults to infinite mode.
- If `depth` is not provided and no time control is provided either, search defaults to infinite mode.

Response when search ends:

```text
bestmove <uci-move>
```

### `stop`

Requests the current search to stop.

Syntax:

```text
stop
```

Behavior:

- If a search is running, the worker is stopped and returns `bestmove ...`.
- If no search is running, nothing is printed.

### `quit`

Stops search (if any) and exits the engine loop.

Syntax:

```text
quit
```

Response: none

## BitBishop Extended Commands (Non-UCI)

### `d`

Displays board diagnostics for the current position.

Syntax:

```text
d
```

Output includes:

- ASCII board
- FEN notation
- Zobrist hash

### `help`

Prints a project help/about message.

Syntax:

```text
help
```

Response: multiline informational text.

### `bench`

Runs a benchmark search on the current position (non-UCI extension).

Syntax:

```text
bench [depth <n>] [movetime <ms>] [wtime <ms>] [btime <ms>] [winc <ms>] [binc <ms>] [infinite]
```

Implemented behavior (current state):

- Uses the same limit parser as `go`.
- `depth <n>` runs a fixed-depth benchmark.
- `movetime <ms>` runs a time-limited benchmark using the same semantics as `go`.
- If `infinite` is provided, benchmark mode internally converts to `depth 10`.
- A bare `bench` command (no limits) is parsed as `infinite`, then converted internally to `depth 10`.
- `stop` can still interrupt a running benchmark early.

Response when benchmark ends:

```text
bench nodes <total> negamax_nodes <negamax> quiescence_nodes <quiescence> time(s) <seconds>s nps <nps>
```

## Options Support Status

### UCI `setoption`

Not implemented yet.

- The engine currently does not expose configurable UCI options.
- No `option name ...` lines are emitted in `uci` response.
