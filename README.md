<div align=center>
    <a href="LICENSE"><img src="https://img.shields.io/github/license/Hardcode3/BitBishop?labelColor=333333&color=00AAEE&logo=opensourceinitiative&logoColor=white" alt="License"></a>
    <a href="https://github.com/Hardcode3/BitBishop/actions"><img src="https://img.shields.io/github/actions/workflow/status/Hardcode3/BitBishop/linux_build_test.yaml?label=Ubuntu&logo=ubuntu&labelColor=E95420&logoColor=white" alt="Ubuntu Build"></a>
    <a href="https://github.com/Hardcode3/BitBishop/actions/workflows/windows_build_test.yaml"><img src="https://img.shields.io/github/actions/workflow/status/Hardcode3/BitBishop/windows_build_test.yaml?label=Windows&logo=windows&labelColor=0078D4&logoColor=white" alt="Windows Build"></a>
    <a href="https://github.com/Hardcode3/BitBishop/actions/workflows/linux_build_test.yaml"><img src="https://img.shields.io/github/actions/workflow/status/Hardcode3/BitBishop/linux_build_test.yaml?label=Tests&logo=githubactions&labelColor=222222&logoColor=white" alt="Tests"></a>
    <a href="https://github.com/Hardcode3/BitBishop/actions/workflows/clang_tidy.yaml"><img src="https://img.shields.io/github/actions/workflow/status/Hardcode3/BitBishop/clang_tidy.yaml?label=Linting&logo=llvm&labelColor=593D9C&logoColor=white" alt="Linting"></a>
    <a href="https://Hardcode3.github.io/BitBishop/html/index.html"><img src="https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/Hardcode3/BitBishop/gh-pages/shieldsio_coverage_badge.json&logo=codecov&logoColor=white" alt="Coverage"></a>
</div>

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
├── src/bitbishop/            # Sources (cpp)
├── include/bitbishop/        # Headers (hpp)
├── tests/bitbishop           # Unit tests
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
