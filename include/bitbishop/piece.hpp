#pragma once
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
  enum Type : int { P, N, B, R, Q, K, p, n, b, r, q, k, NO_PIECE };

 private:
  Type m_type;
  char m_symbol;

 public:
  /**
   * @brief Constructs a piece.
   * @param c Type of the piece to build
   * @throw std::invalid_argument when the Piece character is invalid
   */
  constexpr Piece(char c = '.') {
    m_symbol = c;
    switch (c) {  // clang-format off
      case 'P': m_type = P; break;
      case 'N': m_type = N; break;
      case 'B': m_type = B; break;
      case 'R': m_type = R; break;
      case 'Q': m_type = Q; break;
      case 'K': m_type = K; break;
      case 'p': m_type = p; break;
      case 'n': m_type = n; break;
      case 'b': m_type = b; break;
      case 'r': m_type = r; break;
      case 'q': m_type = q; break;
      case 'k': m_type = k; break;
      case '.': m_type = NO_PIECE; break;  // clang-format on
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
   * @brief Checks if the piece is white.
   * @return true if the piece is white, false otherwise
   */
  constexpr bool is_white() const { return m_type >= P && m_type <= K; }

  /**
   * @brief Checks if the piece is black.
   * @return true if the piece is black, false otherwise
   */
  constexpr bool is_black() const { return m_type >= p && m_type <= k; }

  /**
   * @brief Checks if the piece represents no piece.
   * @return true if NO_PIECE, false otherwise
   */
  constexpr bool is_none() const { return m_type == NO_PIECE; }

  /**
   * @brief Converts the piece to a printable character.
   * @return 'P','N',...,'k' for pieces, '.' for NO_PIECE
   */
  constexpr char to_char() const { return m_symbol; };

  constexpr bool operator==(const Piece& other) const { return m_type == other.m_type; }
  constexpr bool operator!=(const Piece& other) const { return m_type != other.m_type; }
};