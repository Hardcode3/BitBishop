#pragma once

#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>
#include <cctype>
#include <format>
#include <optional>
#include <stdexcept>
#include <string>

/**
 * @struct Move
 * @brief Represents a chess move, including metadata about its properties.
 *
 * This struct encapsulates all information required to describe a move in chess:
 * - The starting and target squares.
 * - Optional promotion piece (for pawn promotions).
 * - Flags indicating special move types (capture, en passant, castling).
 */
struct Move {
  Square from;                     ///< The starting square of the move.
  Square to;                       ///< The target square of the move.
  std::optional<Piece> promotion;  ///< The piece to promote to (if applicable, e.g., for pawns).
  bool is_capture;                 ///< True if the move captures an opponent's piece.
  bool is_en_passant;              ///< True if the move is an en passant capture.
  bool is_castling;                ///< True if the move is a castling move (kingside or queenside)

  static CX_INLINE std::size_t UCI_MOVE_CHAR_REPR_SIZE = 4;
  static CX_INLINE std::size_t UCI_MOVE_PROMOTION_CHAR_REPR_SIZE = 5;

  /**
   * @brief Converts move to UCI notation.
   * @return String in UCI format (e.g., "e2e4", "e7e8q")
   */
  [[nodiscard]] std::string to_uci() const;

  /**
   * @brief Creates a move from its UCI string notation.
   * @return Move instance.
   *
   * UCI move string has 4 or 5 characters.
   * - The two first characters correspond to the square where the piece is moving from.
   * - Next two chars correspond to the square where the piece is moving to.
   * - The final optional character corresponds to the piece type if the move depicted is a promotion.
   *
   * Note that promotion piece is always lowercase because it's color is implicitely described by the destination
   * square.
   *
   * @see https://www.chessprogramming.org/Algebraic_Chess_Notation
   */
  static Move from_uci(const std::string& str);

  /**
   * @brief Creates a normal (non-special) move.
   * @param from The starting square of the move.
   * @param to The target square of the move.
   * @param is_capture True if the move captures an opponent's piece; false otherwise.
   * @return A Move instance representing the normal move.
   */
  static Move make(Square from, Square to, bool is_capture = false);

  /**
   * @brief Creates a pawn promotion move.
   * @param from The starting square of the pawn.
   * @param to The target square where the promotion occurs.
   * @param piece The piece type to promote the pawn into.
   * @param is_capture True if the move captures an opponent's piece; false otherwise.
   * @return A Move instance representing the promotion move.
   */
  static Move make_promotion(Square from, Square to, Piece piece, bool is_capture = false);

  /**
   * @brief Creates an en passant capture move.
   * @param from The starting square of the capturing pawn.
   * @param to The target square where the capturing pawn lands.
   * @return A Move instance representing the en passant capture.
   */
  static Move make_en_passant(Square from, Square to);

  /**
   * @brief Creates a castling move.
   * @param from The starting square of the king.
   * @param to The target square of the king during castling.
   * @return A Move instance representing the castling move.
   */
  static Move make_castling(Square from, Square to);
};
