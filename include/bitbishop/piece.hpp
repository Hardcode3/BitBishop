#pragma once

#include <bitbishop/color.hpp>
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
 * - NO_PIECE represents an empty square
 */
class Piece {
 public:
  /** @brief Enum for piece types */
  enum Type : std::uint8_t { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

 private:
  Type m_type;
  Color m_color;
  char m_symbol;

 public:
  /**
   * @brief Constructs a piece from a type and color.
   * @param type Piece type
   * @param color Piece color
   */
  constexpr Piece(Type type, Color color) : m_type(type), m_color(color) { ; }

  /**
   * @brief Constructs a piece.
   * @param character Type of the piece to build in character notation
   * @throw std::invalid_argument when the Piece character is invalid
   */
  constexpr Piece(char character) {
    m_symbol = character;
    switch (character) {  // clang-format off
      case 'P': m_type = PAWN; m_color = Color::WHITE; break;
      case 'N': m_type = KNIGHT; m_color = Color::WHITE; break;
      case 'B': m_type = BISHOP; m_color = Color::WHITE; break;
      case 'R': m_type = ROOK; m_color = Color::WHITE; break;
      case 'Q': m_type = QUEEN; m_color = Color::WHITE; break;
      case 'K': m_type = KING; m_color = Color::WHITE; break;
      case 'p': m_type = PAWN; m_color = Color::BLACK; break;
      case 'n': m_type = KNIGHT; m_color = Color::BLACK; break;
      case 'b': m_type = BISHOP; m_color = Color::BLACK; break;
      case 'r': m_type = ROOK; m_color = Color::BLACK; break;
      case 'q': m_type = QUEEN; m_color = Color::BLACK; break;
      case 'k': m_type = KING; m_color = Color::BLACK; break;
                  // clang-format on
      default:
        const std::string msg = std::format("Invalid piece character {}", character);
        throw std::invalid_argument(msg);
    }
  }

  /**
   * @brief Returns the underlying enum type of the piece.
   * @return Type of the piece
   */
  [[nodiscard]] constexpr Type type() const { return m_type; }

  /**
   * @brief Returns the underlying enum color of the piece.
   * @return Color of the piece
   */
  [[nodiscard]] constexpr Color color() const { return m_color; }

  /**
   * @brief Checks if the piece is white.
   * @return true if the piece is white, false otherwise
   */
  [[nodiscard]] constexpr bool is_white() const { return m_color == Color::WHITE; }

  /**
   * @brief Checks if the piece is black.
   * @return true if the piece is black, false otherwise
   */
  [[nodiscard]] constexpr bool is_black() const { return m_color == Color::BLACK; }

  /**
   * @brief Converts the piece to a printable character.
   * @return 'P','N',...,'k' for pieces, '.' for NO_PIECE
   */
  [[nodiscard]] constexpr char to_char() const { return m_symbol; };

  constexpr bool operator==(const Piece& other) const { return m_type == other.m_type && m_color == other.m_color; }
  constexpr bool operator!=(const Piece& other) const { return m_type != other.m_type || m_color != other.m_color; }
};

namespace Pieces {
#define DEFINE_PIECE(name, ch) \
  constexpr inline Piece name { ch }

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
