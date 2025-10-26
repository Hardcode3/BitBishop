# BitBishop

A modern chess engine written in C++23, designed for learning low-level programming techniques and chess engine development.

## About

BitBishop is a personal project created to explore low-level manipulations in modern C++.
While primarily a learning endeavor, the codebase is structured and documented to benefit anyone interested in chess engine development or modern C++ practices.

This project is currently 🚧 **under active development** 🚧 and aims to implement the [Universal Chess Interface (UCI) protocol](https://www.chessprogramming.org/UCI), making it compatible with popular chess GUIs and analysis tools.

## Project Goals

- 🎯 **Educational**: Learn and demonstrate low-level C++ techniques
- 📚 **Well-documented**: Extensive inline documentation and guides for educational purposes
- 🏗️ **Clean architecture**: Clear, structured codebase following modern C++ best practices
- 🧪 **Well-tested**: Extensive testing using Google Tests
- ♟️ **UCI compliance**: Full implementation of the UCI protocol for chess engines
- 🔧 **Modern tooling**: Built with C++23, CMake, and vcpkg

## Features

### Current

- Modern C++23 implementation
- CMake build system with vcpkg dependency management
- FEN notation support for position representation
- Structured documentation and guides

### Planned

- Complete UCI protocol implementation
- Move generation and validation
- Position evaluation
- Search algorithms (minimax, alpha-beta pruning)
- Opening book support
- Endgame tablebases

## Getting Started

### Prerequisites

- C++23 compatible compiler (GCC 12+, Clang 16+, or MSVC 2022+)
- CMake 3.20 or higher
- vcpkg package manager
- [clang format]
- [clang tidy]

### Building

- [CMake Guide](./docs/cmake.md) - Build system configuration and usage
  
## Documentation

The project includes comprehensive documentation to support learning.
These docs are a summary of what i learned while going through the chess world during engine's implementation.

Internal refs:

- [UCI Protocol](./docs/uci_protocol.md) - Universal Chess Interface specification
- [FEN Notation](./docs/fen_notation.md) - Forsyth–Edwards Notation for position representation

External refs:

- [Andrew Healey - Building My Own Chess Engine](https://healeycodes.com/building-my-own-chess-engine)
- [Chess Programming Wiki](https://www.chessprogramming.org/Getting_Started)

## Project Structure

```
ChessEngine/
├── build/                       # Build artifacts
├── cmake/                       # Cmake scripts
├── docs/                        # Documentation and guides
├── src/chess_engine/            # Sources (cpp)
├── include/chess_engine/        # Headers (hpp)
├── tests/chess_engine           # Unit tests
├── CMakeLists.txt               # Build configuration
└── ...
```

## Contributing

While this is primarily a personal learning project, suggestions and constructive feedback are welcome! Feel free to:

- Open issues for bugs or suggestions
- Submit pull requests with improvements
- Share your own learning experiences

## License

- [MIT License](./LICENSE)

## Acknowledgments

This project draws inspiration from the chess programming community and various resources on chess engine development.

## Status

🚧 **In Development** - This engine is not yet functional for actual play. Stay tuned for updates!

---

**Repository**: [github.com/Hardcode3/ChessEngine](https://github.com/Hardcode3/ChessEngine)
