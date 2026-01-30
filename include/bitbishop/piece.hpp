#pragma once

#include <bitbishop/color.hpp>
#include <bitbishop/config.hpp>
#include <cctype>
#include <format>
#include <stdexcept>

/**
 * @brief Represents a chess piece.
 *
 * This class wraps a lightweight enum and provides utility methods
 * for querying piece color, converting to/from characters, and comparison.
 *
 * By convention:
 * - White pieces are uppercase (P, N, B, R, Q, K)
 * - Black pieces are lowercase (p, n, b, r, q, k)
 */
class Piece {
 public:
  /**
   * @brief Enum of all supported piece types.
   *
   * Stored in a compact underlying type to ensure `Piece` remains lightweight.
   */
  enum Type : std::uint8_t { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

  /**
   * @brief Number of distinct piece types.
   *
   * Useful for array sizing and iteration without magic numbers.
   */
  static CX_INLINE std::size_t TYPE_COUNT = 6;

  /**
   * @brief Distinct piece types.
   */
  static CX_INLINE std::array<Type, TYPE_COUNT> ALL_TYPES = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};

 private:
  /**
   * @brief The type of the piece (pawn, knight, etc.).
   */
  Type m_type;

  /**
   * @brief The color of the piece (white or black).
   */
  Color m_color;

 public:
  /**
   * @brief Constructs a piece from a type and color.
   * @param type Piece type
   * @param color Piece color
   */
  CX_FN Piece(Type type, Color color) : m_type(type), m_color(color) {}

  /**
   * @brief Constructs a piece from a character representation.
   *
   * Uppercase indicates white pieces, lowercase indicates black pieces.
   *
   * @param character Character representing the piece ('P', 'n', etc.)
   * @throw std::invalid_argument If the character does not represent a valid piece
   */
  CX_FN Piece(char character) {
    m_type = Piece::type_from_char(character);
    m_color = Piece::color_from_char(character);
  }

  /**
   * @brief Returns the piece type.
   * @return Type enum
   */
  [[nodiscard]] CX_FN Type type() const { return m_type; }

  /**
   * @brief Returns the piece color.
   * @return Color enum
   */
  [[nodiscard]] CX_FN Color color() const { return m_color; }

  /**
   * @brief Checks if the piece is white.
   * @return true if white, false otherwise
   */
  [[nodiscard]] CX_FN bool is_white() const { return m_color == Color::WHITE; }

  /**
   * @brief Checks if the piece is black.
   * @return true if black, false otherwise
   */
  [[nodiscard]] CX_FN bool is_black() const { return m_color == Color::BLACK; }

  /** @brief Checks if the piece is a pawn. */
  [[nodiscard]] CX_FN bool is_pawn() const { return m_type == Type::PAWN; }
  /** @brief Checks if the piece is a knight. */
  [[nodiscard]] CX_FN bool is_knight() const { return m_type == Type::KNIGHT; }
  /** @brief Checks if the piece is a bishop. */
  [[nodiscard]] CX_FN bool is_bishop() const { return m_type == Type::BISHOP; }
  /** @brief Checks if the piece is a rook. */
  [[nodiscard]] CX_FN bool is_rook() const { return m_type == Type::ROOK; }
  /** @brief Checks if the piece is a queen. */
  [[nodiscard]] CX_FN bool is_queen() const { return m_type == Type::QUEEN; }
  /** @brief Checks if the piece is a king. */
  [[nodiscard]] CX_FN bool is_king() const { return m_type == Type::KING; }

  /**
   * @brief Checks if the piece is a sliding piece (bishop, rook, queen).
   * @return true if sliding, false otherwise
   */
  [[nodiscard]] CX_FN bool is_slider() {
    return m_type == Piece::BISHOP || m_type == Piece::ROOK || m_type == Piece::QUEEN;
  }

  /**
   * @brief Returns the type associated with a character representation.
   *
   * @param character Piece character ('P', 'b', etc.)
   * @return Corresponding piece type
   * @throw std::invalid_argument If the character is invalid
   */
  [[nodiscard]] static CX_FN Type type_from_char(char character) {
    switch (character) {
      case 'P':
      case 'p':
        return PAWN;
      case 'N':
      case 'n':
        return KNIGHT;
      case 'B':
      case 'b':
        return BISHOP;
      case 'R':
      case 'r':
        return ROOK;
      case 'Q':
      case 'q':
        return QUEEN;
      case 'K':
      case 'k':
        return KING;
    }
    throw std::invalid_argument("Invalid piece character at compile time");
  }

  /**
   * @brief Extracts piece color from character representation.
   *
   * @param character Piece character ('P' = white, 'p' = black)
   * @return Detected color
   * @throw std::invalid_argument If the character is not alphabetic
   */
  [[nodiscard]] static CX_FN Color color_from_char(char character) {
    if (character >= 'A' && character <= 'Z') {
      return Color::WHITE;
    }
    if (character >= 'a' && character <= 'z') {
      return Color::BLACK;
    }
    throw std::invalid_argument("Invalid character at compile time");
  }

  /**
   * @brief Human-readable name of a piece type.
   *
   * @param type Piece type enum
   * @return C-string name ("pawn", "knight", etc.)
   */
  [[nodiscard]] static CX_FN const char* name(Type type) {
    switch (type) {
      case PAWN:
        return "pawn";
      case KNIGHT:
        return "knight";
      case BISHOP:
        return "bishop";
      case ROOK:
        return "rook";
      case QUEEN:
        return "queen";
      case KING:
        return "king";
    }
    return "unknown";
  }

  /**
   * @brief Converts a type and color to its representative character.
   *
   * @param type Piece type
   * @param color Piece color
   * @return Uppercase for white, lowercase for black (e.g., 'Q' or 'q')
   */
  [[nodiscard]] static CX_FN char to_char(Type type, Color color) {
    static CX_CONST std::array<char, TYPE_COUNT> whiteSymbols = {'P', 'N', 'B', 'R', 'Q', 'K'};
    static CX_CONST std::array<char, TYPE_COUNT> blackSymbols = {'p', 'n', 'b', 'r', 'q', 'k'};
    return (color == Color::WHITE) ? whiteSymbols[type] : blackSymbols[type];
  }

  /**
   * @brief Converts the stored piece to its character representation.
   * @return Character ('P', 'k', etc.)
   */
  [[nodiscard]] CX_FN char to_char() const { return Piece::to_char(m_type, m_color); }

  /**
   * @brief Equality operator.
   * @param other Piece to compare against
   * @return true if both type and color match
   */
  CX_FN bool operator==(const Piece& other) const { return m_type == other.m_type && m_color == other.m_color; }

  /**
   * @brief Inequality operator.
   * @param other Piece to compare against
   * @return true if type or color differ
   */
  CX_FN bool operator!=(const Piece& other) const { return !(*this == other); }
};

namespace Pieces {
#define DEFINE_PIECE(name, ch)                             \
  /** @brief Predefined piece constant for convenience. */ \
  CX_INLINE Piece name { ch }

// White pieces
DEFINE_PIECE(WHITE_PAWN, 'P');
DEFINE_PIECE(WHITE_KNIGHT, 'N');
DEFINE_PIECE(WHITE_BISHOP, 'B');
DEFINE_PIECE(WHITE_ROOK, 'R');
DEFINE_PIECE(WHITE_QUEEN, 'Q');
DEFINE_PIECE(WHITE_KING, 'K');

// Black pieces
DEFINE_PIECE(BLACK_PAWN, 'p');
DEFINE_PIECE(BLACK_KNIGHT, 'n');
DEFINE_PIECE(BLACK_BISHOP, 'b');
DEFINE_PIECE(BLACK_ROOK, 'r');
DEFINE_PIECE(BLACK_QUEEN, 'q');
DEFINE_PIECE(BLACK_KING, 'k');

#undef DEFINE_PIECE
}  // namespace Pieces
