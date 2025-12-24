# About the `interface/` directory

## The core rule

Interface = communication with the outside world

This layer translates between the engine and humans or GUIs.

## What exactly goes into interface/

### Definition

interface/ contains logic that:

- parses input commands
- formats output
- manages clocks and sessions
- interacts with GUIs or users

This layer may:

- call engine entry points
- read/write text or UI events

This layer must not:

- implement search logic
- implement chess rules
- manipulate bitboards directly

Typical responsibilities

- UCI / CECP protocol handling
- GUI hooks
- command parsing
- time control management
- logging

### Examples (good)

```cpp
// interface/uci.cpp
void uci_loop();

// interface/cli.cpp
void run_cli();
```

### Examples (bad — belongs elsewhere)

```cpp
Bitboard attackers_to(...); // ❌ attacks
int alpha_beta(...); // ❌ engine
generate_legal_moves(...); // ❌ movegen
```
