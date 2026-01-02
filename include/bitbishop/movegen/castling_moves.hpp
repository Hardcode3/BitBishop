#pragma once

#include <bitbishop/attacks/generate_attacks.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <utility>
#include <vector>

/**
 * @brief Generates all legal castling moves for the given side.
 *
 * Adds king-side and queen-side castling moves if and only if:
 * - The side is not currently in check
 * - The corresponding castling right is available
 * - The king and rook are on their initial squares
 * - All squares between the king and rook are empty
 * - The squares the king passes through and lands on are not attacked
 *
 * This function does not move any pieces. Castling is emitted as a king move
 * with the castling flag set; rook movement is handled during move execution.
 *
 * @param moves Vector to append generated castling moves to
 * @param board Current board position
 * @param us Color of the side to generate castling moves for
 * @param checkers Bitboard of pieces currently checking the king
 * @param enemy_attacks Bitboard of squares attacked by the opponent
 */
void generate_castling_moves(std::vector<Move>& moves, const Board& board, Color us, const Bitboard& checkers,
                             const Bitboard& enemy_attacks) {
  using namespace Squares;

  if (checkers.any()) {
    return;
  }

  const Square king_from = (us == Color::WHITE) ? E1 : E8;

  if (board.can_castle_kingside(us)) {
    Square f = (us == Color::WHITE) ? F1 : F8;
    Square g = (us == Color::WHITE) ? G1 : G8;

    if (!enemy_attacks.test(f) && !enemy_attacks.test(g)) {
      moves.emplace_back(king_from, g, std::nullopt, false, false, true);
    }
  }

  if (board.can_castle_queenside(us)) {
    Square d = (us == Color::WHITE) ? D1 : D8;
    Square c = (us == Color::WHITE) ? C1 : C8;

    if (!enemy_attacks.test(d) && !enemy_attacks.test(c)) {
      moves.emplace_back(king_from, c, std::nullopt, false, false, true);
    }
  }
}
