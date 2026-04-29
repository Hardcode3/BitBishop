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

namespace MoveGen {
enum class Scope : std::uint8_t {
  AllMoves,
  CapturesOnly,
};
}  // namespace MoveGen

inline void generate_legal_moves_with_scope(std::vector<Move>& moves, const Board& board, MoveGen::Scope scope) {
  const bool captures_only = scope == MoveGen::Scope::CapturesOnly;

  Color us = board.get_state().m_is_white_turn ? Color::WHITE : Color::BLACK;
  Color them = ColorUtil::opposite(us);

  Square king_sq = board.king_square(us).value();

  Bitboard checkers = compute_checkers(board, king_sq, them);
  Bitboard check_mask = compute_check_mask(king_sq, checkers, board);
  PinResult pins = compute_pins(king_sq, board, us);
  Bitboard enemy_attacks = generate_attacks(board, them);
  Bitboard enemy = board.enemy(us);
  Bitboard allowed_targets = captures_only ? enemy : Bitboard::Ones();

  generate_legal_king_moves(moves, board, us, king_sq, enemy_attacks, allowed_targets);

  if (!captures_only) {
    generate_castling_moves(moves, board, us, checkers, enemy_attacks);
  }

  if (checkers.count() > 1) {
    return;
  }

  generate_knight_legal_moves(moves, board, us, check_mask, pins, allowed_targets);
  generate_bishop_legal_moves(moves, board, us, check_mask, pins, allowed_targets);
  generate_rook_legal_moves(moves, board, us, check_mask, pins, allowed_targets);
  generate_queen_legal_moves(moves, board, us, check_mask, pins, allowed_targets);
  generate_pawn_legal_moves(moves, board, us, king_sq, check_mask, pins, captures_only);
}

/**
 * @brief Generates all legal moves for the side to move.
 */
inline void generate_legal_moves(std::vector<Move>& moves, const Board& board) {
  generate_legal_moves_with_scope(moves, board, MoveGen::Scope::AllMoves);
}

/**
 * @brief Generates only legal capture moves for the side to move.
 *
 * Includes king captures, piece captures, pawn captures and legal en passant.
 * Excludes non-capture moves (quiet king moves, pawn pushes, castling, ...).
 */
inline void generate_legal_capture_moves(std::vector<Move>& moves, const Board& board) {
  generate_legal_moves_with_scope(moves, board, MoveGen::Scope::CapturesOnly);
}
