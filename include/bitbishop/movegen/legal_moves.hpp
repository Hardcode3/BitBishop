#pragma once

#include <bitbishop/attacks/checkers.hpp>
#include <bitbishop/attacks/generate_attacks.hpp>
#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/attackers.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/bishop_moves.hpp>
#include <bitbishop/movegen/castling_moves.hpp>
#include <bitbishop/movegen/check_mask.hpp>
#include <bitbishop/movegen/king_moves.hpp>
#include <bitbishop/movegen/knight_moves.hpp>
#include <bitbishop/movegen/pawn_moves.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/movegen/queen_moves.hpp>
#include <bitbishop/movegen/rook_moves.hpp>
#include <vector>

/**
 * @brief Generates all legal moves for the given side in the current position.
 *
 * This function computes the full set of legal moves for @p us, taking into
 * account checks, pins, enemy attacks, and special rules such as castling
 * and en passant. Move legality is enforced during generation rather than
 * by post-filtering.
 *
 * The generation pipeline is:
 *  - Detect checkers and compute the check resolution mask
 *  - Compute pinned pieces and their allowed movement rays
 *  - Generate legal king moves (always allowed)
 *  - Generate legal castling moves (only when not in check)
 *  - If in double check, stop after king moves
 *  - Otherwise, generate legal moves for all remaining pieces
 *
 * @param moves Vector to append generated legal moves to
 * @param board Current board position
 * @param us Color of the side to generate moves for
 *
 * @note The move list is appended to; it is not cleared by this function.
 * @note Assumes the board position is internally consistent and legal.
 */
void generate_legal_moves(std::vector<Move>& moves, const Board& board, Color us) {
  Square king_sq = board.king_square(us).value();
  Color them = ColorUtil::opposite(us);

  Bitboard checkers = compute_checkers(board, king_sq, them);
  Bitboard check_mask = compute_check_mask(king_sq, checkers, board);
  PinResult pins = compute_pins(king_sq, board, us);
  Bitboard enemy_attacks = generate_attacks(board, them);

  generate_legal_king_moves(moves, board, us, king_sq, enemy_attacks);

  generate_castling_moves(moves, board, us, checkers, enemy_attacks);

  if (checkers.count() > 1) {
    return;
  }

  generate_knight_legal_moves(moves, board, us, check_mask, pins);
  generate_bishop_legal_moves(moves, board, us, check_mask, pins);
  generate_rook_legal_moves(moves, board, us, check_mask, pins);
  generate_queen_legal_moves(moves, board, us, check_mask, pins);
  generate_pawn_legal_moves(moves, board, us, king_sq, check_mask, pins);
}
