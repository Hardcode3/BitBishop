#pragma once

#include <bitbishop/constants.hpp>
#include <bitbishop/square.hpp>
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
  enum Value : std::uint8_t {
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
   * @brief Construct a Square from a raw integer value (flattened index) with runtime validation.
   * @param square_index Integer in range [0, 63].
   * @throw std::invalid_argument if square_index is out of range.
   */
  explicit Square(int square_index) : m_value(static_cast<Value>(square_index)) {
    using namespace Const;

    if (square_index < 0 || square_index >= BOARD_SIZE) {
      const std::string msg = std::format("Invalid flattened square index {}, must stay in range [0,63]", square_index);
      throw std::invalid_argument(msg);
    }
  }

  /**
   * @brief Construct a Square from a raw integer value (flattened index).
   * @param square_index Integer in range [0, 63].
   * @param std::in_place_t
   *
   * The std::in_place_t parameter serves as a compile-time tag to disambiguate this constructor
   * from other constructors. It doesn't carry any data—it's an empty struct whose sole purpose is
   * to enable overload resolution:
   *
   * ```c++
   * // You'd call it like this:
   * Square sq(42, std::in_place);  // std::in_place is a constexpr instance of std::in_place_t
   *
   * // Versus a regular constructor:
   * Square sq(42);  // Calls a different constructor
   * ```
   */
  constexpr Square(int square_index, std::in_place_t tag) noexcept : m_value(static_cast<Value>(square_index)) {
    (void)tag;
  }

  /**
   * @brief Construct a Square directly from an enum value.
   * @param square_value A valid Value from the Square::Value enum.
   */
  constexpr explicit Square(Value square_value) : m_value(square_value) {}

  /**
   * @brief Construct a Square from file and rank coordinates.
   * @param file File index (0 = 'a', …, 7 = 'h').
   * @param rank Rank index (0 = '1', …, 7 = '8').
   * @throw std::invalid_argument if file or rank are out of range.
   */
  explicit Square(int file, int rank) {
    using namespace Const;

    // Rank refers to the eight horizontal rows on the board, labelled 1 to 8.
    // File refers to the eight vertical columns on the board, labelled a to h.
    if (file < FILE_A_IND || file > FILE_H_IND || rank < RANK_1_IND || rank > RANK_8_IND) {
      const std::string msg = std::format("Invalid file ({}) or rank({}), must stay within ranke [0,7].", file, rank);
      throw std::invalid_argument(msg);
    }
    m_value = static_cast<Value>((rank * BOARD_WIDTH) + file);
  }

  /**
   * @brief Construct a Square from algebraic notation.
   * @param str String like "e4" (case-insensitive).
   * @throw std::invalid_argument if the string is malformed or out of range.
   */
  explicit Square(const std::string& str) {
    using namespace Const;

    if (str.size() != 2) {
      const std::string msg = std::format("Invalid Square string '{}', must be built out of two characters", str);
      throw std::invalid_argument(msg);
    }

    const char file = std::tolower(str.at(0));
    const char rank = str.at(1);
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
      const std::string msg = std::format(
          "Invalid Square string '{}', must be built out of two characters, "
          "the first being the file (between a and h, got {}) and the second the rank (between 1 and 8, got {})",
          str, file, rank);
      throw std::invalid_argument(msg);
    }
    const int ifile = file - 'a';
    const int irank = rank - '1';
    m_value = static_cast<Value>((irank * BOARD_WIDTH) + ifile);
  }

  /**
   * @brief Get the underlying enum value.
   * @return Square::Value corresponding to this square.
   */
  [[nodiscard]] constexpr Value value() const { return m_value; }

  /**
   * @brief Get the underlying flqttened square index.
   * @return std::uint8_t Flattened index corresponding to this square.
   */
  [[nodiscard]] constexpr std::uint8_t flat_index() const { return static_cast<std::uint8_t>(m_value); }

  /**
   * @brief Get the file index (column).
   * @return Integer 0–7, where 0 = 'a' and 7 = 'h'.
   */
  [[nodiscard]] constexpr int file() const {
    using namespace Const;
    return static_cast<int>(m_value) % BOARD_WIDTH;
  }

  /**
   * @brief Get the rank index (row).
   * @return Integer 0–7, where 0 = '1' and 7 = '8'.
   */
  [[nodiscard]] constexpr int rank() const {
    using namespace Const;
    return static_cast<int>(m_value) / BOARD_WIDTH;
  }

  /**
   * @brief Tells if two squares lays on the same file.
   * @return True if two squares lays on the same file, false otherwise.
   */
  [[nodiscard]] constexpr bool same_file(const Square& other) const { return this->file() == other.file(); }

  /**
   * @brief Tells if two squares lays on the same rank.
   * @return True if two squares lays on the same rank, false otherwise.
   */
  [[nodiscard]] constexpr bool same_rank(const Square& other) const { return this->rank() == other.rank(); }

  /**
   * @brief Checks whether this square lies on the same NE–SW diagonal as another square.
   *
   * Two squares are aligned on a NE–SW diagonal if the difference between their
   * file and rank coordinates is identical. Along such a diagonal, (file − rank)
   * remains constant.
   *
   * This diagonal orientation corresponds to bishop movement in the NE–SW direction
   * and is commonly used when computing diagonal attacks, pins, and interposition
   * rays.
   *
   * @param other The square to compare against.
   * @return true if both squares share the same NE–SW diagonal, false otherwise.
   */
  [[nodiscard]] constexpr bool same_ne_sw_diag(const Square& other) const {
    return (this->file() - this->rank()) == (other.file() - other.rank());
  }

  /**
   * @brief Checks whether this square lies on the same NW–SE diagonal as another square.
   *
   * Two squares are aligned on a NW–SE diagonal if the sum of their file and rank
   * coordinates is identical. Along such a diagonal, (file + rank) remains constant.
   *
   * This diagonal orientation corresponds to bishop movement in the NW–SE direction
   * and is commonly used when computing diagonal attacks, pins, and interposition
   * rays.
   *
   * @param other The square to compare against.
   * @return true if both squares share the same NW–SE diagonal, false otherwise.
   */
  [[nodiscard]] constexpr bool same_nw_se_diag(const Square& other) const {
    return (this->file() + this->rank()) == (other.file() + other.rank());
  }

  /**
   * @brief Checks whether this square lies on the same diagonal as another square.
   *
   * Two squares are on the same diagonal if they share either:
   * - the same NE–SW diagonal, or
   * - the same NW–SE diagonal.
   *
   * This geometric property is used to determine bishop and queen alignment,
   * detect pins, compute interposition rays, and build diagonal attack lookups.
   *
   * @param other The square to compare against.
   * @return true if both squares are aligned on a common diagonal, false otherwise.
   */
  [[nodiscard]] constexpr bool same_diag(const Square& other) const {
    return same_ne_sw_diag(other) || same_nw_se_diag(other);
  }

  /**
   * @brief Convert the square to algebraic notation.
   * @return Lowercase string like "a1", "e4", "h8".
   */
  [[nodiscard]] std::string to_string() const { return {char('a' + file()), char('1' + rank())}; }

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

namespace Squares {
#define DEFINE_SQUARE(name) constexpr inline Square name(Square::name)

// clang-format off
// Rank 1
DEFINE_SQUARE(A1); DEFINE_SQUARE(B1); DEFINE_SQUARE(C1); DEFINE_SQUARE(D1);
DEFINE_SQUARE(E1); DEFINE_SQUARE(F1); DEFINE_SQUARE(G1); DEFINE_SQUARE(H1);

// Rank 2
DEFINE_SQUARE(A2); DEFINE_SQUARE(B2); DEFINE_SQUARE(C2); DEFINE_SQUARE(D2);
DEFINE_SQUARE(E2); DEFINE_SQUARE(F2); DEFINE_SQUARE(G2); DEFINE_SQUARE(H2);

// Rank 3
DEFINE_SQUARE(A3); DEFINE_SQUARE(B3); DEFINE_SQUARE(C3); DEFINE_SQUARE(D3);
DEFINE_SQUARE(E3); DEFINE_SQUARE(F3); DEFINE_SQUARE(G3); DEFINE_SQUARE(H3);

// Rank 4
DEFINE_SQUARE(A4); DEFINE_SQUARE(B4); DEFINE_SQUARE(C4); DEFINE_SQUARE(D4);
DEFINE_SQUARE(E4); DEFINE_SQUARE(F4); DEFINE_SQUARE(G4); DEFINE_SQUARE(H4);

// Rank 5
DEFINE_SQUARE(A5); DEFINE_SQUARE(B5); DEFINE_SQUARE(C5); DEFINE_SQUARE(D5);
DEFINE_SQUARE(E5); DEFINE_SQUARE(F5); DEFINE_SQUARE(G5); DEFINE_SQUARE(H5);

// Rank 6
DEFINE_SQUARE(A6); DEFINE_SQUARE(B6); DEFINE_SQUARE(C6); DEFINE_SQUARE(D6);
DEFINE_SQUARE(E6); DEFINE_SQUARE(F6); DEFINE_SQUARE(G6); DEFINE_SQUARE(H6);

// Rank 7
DEFINE_SQUARE(A7); DEFINE_SQUARE(B7); DEFINE_SQUARE(C7); DEFINE_SQUARE(D7);
DEFINE_SQUARE(E7); DEFINE_SQUARE(F7); DEFINE_SQUARE(G7); DEFINE_SQUARE(H7);

// Rank 8
DEFINE_SQUARE(A8); DEFINE_SQUARE(B8); DEFINE_SQUARE(C8); DEFINE_SQUARE(D8);
DEFINE_SQUARE(E8); DEFINE_SQUARE(F8); DEFINE_SQUARE(G8); DEFINE_SQUARE(H8);
// clang-format on

#undef DEFINE_SQUARE
}  // namespace Squares
