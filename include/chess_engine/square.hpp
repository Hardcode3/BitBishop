#pragma once
#include <chess_engine/square.hpp>
#include <format>
#include <stdexcept>
#include <string>

/**
 * @brief Represents a square on the chess board.
 *
 * Internally wraps an enum with values 0–63 for squares A1–H8.
 * Provides convenient constructors, accessors, and comparison operators.
 */
class Square {
 public:
  /**
   * @brief Enum representing squares of a chess board.
   *
   * Mapping is row-major: A1 = 0 and H8 = 63.
   *
   * @note An unscoped enum is used because:
   * - Square values must be implicitly convertible to int
   * - This allows efficient use in bitboards and shifts
   * - Scoped enums (`enum class`) forbid implicit conversions
   */
  // clang-format off
  enum Value : int {
      A1, B1, C1, D1, E1, F1, G1, H1,
      A2, B2, C2, D2, E2, F2, G2, H2,
      A3, B3, C3, D3, E3, F3, G3, H3,
      A4, B4, C4, D4, E4, F4, G4, H4,
      A5, B5, C5, D5, E5, F5, G5, H5,
      A6, B6, C6, D6, E6, F6, G6, H6,
      A7, B7, C7, D7, E7, F7, G7, H7,
      A8, B8, C8, D8, E8, F8, G8, H8,
    };
  // clang-format on

 private:
  /// @brief The internal square value (0–63).
  Value m_value;

 public:
  /**
   * @brief Construct a Square from a raw integer value (flattened index).
   * @param v Integer in range [0, 63].
   * @throw std::invalid_argument if v is out of range.
   */
  explicit Square(int v) : m_value(static_cast<Value>(v)) {
    if (v < 0 || v > 63) {
      const std::string msg = std::format("Invalid flattened square index {}, must stay in range [0,63]", v);
      throw std::invalid_argument(msg);
    }
  }

  /**
   * @brief Construct a Square directly from an enum value.
   * @param v A valid Value from the Square::Value enum.
   */
  constexpr explicit Square(Value v) : m_value(v) {}

  /**
   * @brief Construct a Square from file and rank coordinates.
   * @param file File index (0 = 'a', …, 7 = 'h').
   * @param rank Rank index (0 = '1', …, 7 = '8').
   * @throw std::invalid_argument if file or rank are out of range.
   */
  explicit Square(int file, int rank) {
    // Rank refers to the eight horizontal rows on the board, labelled 1 to 8.
    // File refers to the eight vertical columns on the board, labelled a to h.
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
      const std::string msg = std::format("Invalid file ({}) or rank({}), must stay within ranke [0,7].", file, rank);
      throw std::invalid_argument(msg);
    }
    m_value = static_cast<Value>(rank * 8 + file);
  }

  /**
   * @brief Construct a Square from algebraic notation.
   * @param s String like "e4" (case-insensitive).
   * @throw std::invalid_argument if the string is malformed or out of range.
   */
  explicit Square(const std::string& s) {
    if (s.size() != 2) {
      const std::string msg = std::format("Invalid Square string '{}', must be built out of two characters", s);
      throw std::invalid_argument(msg);
    }

    const char file = std::tolower(s.at(0));
    const char rank = s.at(1);
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
      const std::string msg = std::format(
          "Invalid Square string '{}', must be built out of two characters, "
          "the first being the file (between a and h, got {}) and the second the rank (between 1 and 8, got {})",
          s, file, rank);
      throw std::invalid_argument(msg);
    }
    const int ifile = file - 'a';
    const int irank = rank - '1';
    m_value = static_cast<Value>(irank * 8 + ifile);
  }

  /**
   * @brief Get the underlying enum value.
   * @return Square::Value corresponding to this square.
   */
  constexpr Value value() const { return m_value; }

  /**
   * @brief Get the file index (column).
   * @return Integer 0–7, where 0 = 'a' and 7 = 'h'.
   */
  constexpr int file() const { return static_cast<int>(m_value) % 8; }

  /**
   * @brief Get the rank index (row).
   * @return Integer 0–7, where 0 = '1' and 7 = '8'.
   */
  constexpr int rank() const { return static_cast<int>(m_value) / 8; }

  /**
   * @brief Convert the square to algebraic notation.
   * @return Lowercase string like "a1", "e4", "h8".
   */
  std::string to_string() const { return {char('a' + file()), char('1' + rank())}; }

  /**
   * @brief Equality operator.
   * @param other Another Square.
   * @return true if both refer to the same square.
   */
  constexpr bool operator==(const Square& other) const { return m_value == other.m_value; }

  /**
   * @brief Inequality operator.
   * @param other Another Square.
   * @return true if the squares differ.
   */
  constexpr bool operator!=(const Square& other) const { return m_value != other.m_value; }
};
