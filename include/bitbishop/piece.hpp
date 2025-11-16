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
  enum Type : int { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE };

 private:
  Type m_type;
  Color m_color;
  char m_symbol;

 public:
  /**
   * @brief Constructs a piece from a type and color.
   * @param t Piece type
   * @param c Piece color
   */
  constexpr Piece(Type t, Color c) : m_type(t), m_color(c) { ; }

  /**
   * @brief Constructs a piece.
   * @param c Type of the piece to build in character notation
   * @throw std::invalid_argument when the Piece character is invalid
   *
   * Default Piece is a "NONE" piece with color WHITE (char '.').
   */
  constexpr Piece(char c = '.') {
    m_symbol = c;
    switch (c) {  // clang-format off
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
      case '.': m_type = NONE; m_color = Color::WHITE; break;  // clang-format on
      default:
        const std::string msg = std::format("Invalid piece character {}", c);
        throw std::invalid_argument(msg);
    }
  }

  /**
   * @brief Returns the underlying enum type of the piece.
   * @return Type of the piece
   */
  constexpr Type type() const { return m_type; }

  /**
   * @brief Returns the underlying enum color of the piece.
   * @return Color of the piece
   */
  constexpr Color color() const { return m_color; }

  /**
   * @brief Checks if the piece is white.
   * @return true if the piece is white, false otherwise
   */
  constexpr bool is_white() const { return m_color == Color::WHITE; }

  /**
   * @brief Checks if the piece is black.
   * @return true if the piece is black, false otherwise
   */
  constexpr bool is_black() const { return m_color == Color::BLACK; }

  /**
   * @brief Checks if the piece represents no piece.
   * @return true if NO_PIECE, false otherwise
   */
  constexpr bool is_none() const { return m_type == NONE; }

  /**
   * @brief Converts the piece to a printable character.
   * @return 'P','N',...,'k' for pieces, '.' for NO_PIECE
   */
  constexpr char to_char() const { return m_symbol; };

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

// Empty
DEFINE_PIECE(NONE, '.');

#undef DEFINE_PIECE
}  // namespace Pieces
