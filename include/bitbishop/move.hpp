#pragma once

#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>
#include <optional>

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
  bool is_castling;                ///< True if the move is a castling move (kingside or queenside).
};
