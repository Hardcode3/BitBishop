#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/attackers.hpp>
#include <bitbishop/lookups/pawn_attacks.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <utility>
#include <vector>

constexpr std::array<Piece, 4> WHITE_PROMOTIONS = {Pieces::WHITE_QUEEN, Pieces::WHITE_ROOK, Pieces::WHITE_BISHOP,
                                                   Pieces::WHITE_KNIGHT};
constexpr std::array<Piece, 4> BLACK_PROMOTIONS = {Pieces::BLACK_QUEEN, Pieces::BLACK_ROOK, Pieces::BLACK_BISHOP,
                                                   Pieces::BLACK_KNIGHT};

/**
 * @brief Checks if a square is on the pawn's starting rank.
 *
 * Pawns start on rank 2 (index 1) for White and rank 7 (index 6) for Black.
 * This is used to determine if a pawn is eligible to perform a double push.
 *
 * @param square Square to check
 * @param color Color of the pawn
 * @return true if the square is on the starting rank for the given color, false otherwise
 */
constexpr bool is_starting_rank(Square square, Color color) {
  using namespace Const;
  return color == Color::WHITE ? (square.rank() == RANK_2_IND) : (square.rank() == RANK_7_IND);
}

/**
 * @brief Checks if a square is on the pawn's promotion rank.
 *
 * Pawns promote when reaching rank 8 (index 7) for White or rank 1 (index 0) for Black.
 *
 * @param square Square to check
 * @param color Color of the pawn
 * @return true if the square is on the promotion rank for the given color, false otherwise
 */
constexpr bool is_promotion_rank(Square square, Color color) {
  using namespace Const;
  return color == Color::WHITE ? (square.rank() == RANK_8_IND) : (square.rank() == RANK_1_IND);
}

/**
 * @brief Validates if an en passant capture is geometrically legal.
 *
 * En passant geometry is valid when:
 * - The capturing pawn is on rank 5 for White (index 4) or rank 4 for Black (index 3)
 * - The en passant target square is exactly one file adjacent (horizontally)
 * - The en passant target square is on rank 6 for White (index 5) or rank 3 for Black (index 2)
 *
 * Note: This only checks geometric validity. The caller must separately verify that
 * en passant is actually available (i.e., an enemy pawn just performed a double push
 * to the square behind the target square).
 *
 * @param from Square of the capturing pawn
 * @param epsq Available en passant target square
 * @param side Color of the capturing pawn
 * @return true if the capture geometry is valid for en passant, false otherwise
 */
constexpr bool can_capture_en_passant(Square from, Square epsq, Color side) noexcept {
  using namespace Const;

  // MSVC have not yet made std::abs() constexpr for C++ 23, forcing us to define a generic constexpr one...
  // For this, lets apply the abs() function manually. This is sad, but you know, MSVC...
  // The code should not adapt to the compiler for the same language, the compiler should...
  int dif = from.file() - epsq.file();
  dif = (dif < 0) ? -dif : dif;

  if (dif != 1) {
    return false;
  }
  if (side == Color::WHITE && from.rank() == RANK_5_IND && epsq.rank() == RANK_6_IND) {
    return true;
  }
  if (side == Color::BLACK && from.rank() == RANK_4_IND && epsq.rank() == RANK_3_IND) {
    return true;
  }
  return false;
}

/**
 * @brief Adds all four promotion moves (Queen, Rook, Bishop, Knight) to the move list.
 *
 * When a pawn reaches the opposite end of the board (rank 8 for White, rank 1 for Black),
 * it must promote to another piece. This function adds all four possible promotions
 * for a single pawn move.
 *
 * @param moves Vector to append promotion moves to
 * @param from Source square of the promoting pawn
 * @param to Destination square (must be on the promotion rank)
 * @param side Color of the promoting pawn
 * @param is_capture Whether the promotion involves capturing an enemy piece
 */
void add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, Color side, bool capture) {
  const auto& promotion_pieces = (side == Color::WHITE) ? WHITE_PROMOTIONS : BLACK_PROMOTIONS;

  for (auto piece : promotion_pieces) {
    moves.emplace_back(from, to, piece, capture, false, false);
  }
}

void generate_pawn_legal_moves(std::vector<Move>& moves, const Board& board, Color us, Square king_sq,
                               const Bitboard& check_mask, const PinResult& pins) {
  using namespace Lookups;

  const Bitboard own = board.friendly(us);
  const Bitboard enemy = board.enemy(us);
  const Bitboard occupied = board.occupied();
  Bitboard pawns = board.pawns(us);

  const auto& single_push = PAWN_SINGLE_PUSH[ColorUtil::to_index(us)];
  const auto& double_push = PAWN_DOUBLE_PUSH[ColorUtil::to_index(us)];
  const auto& captures = PAWN_ATTACKS[ColorUtil::to_index(us)];

  const std::optional<Square> epsq_opt = board.en_passant_square();

  // warning: this loop is destructive on Bitboard pawns
  while (auto from_sq = pawns.pop_lsb()) {
    Square from = from_sq.value();
    const bool is_pinned = pins.pinned.test(from);
    const Bitboard pin_mask = is_pinned ? pins.pin_ray[from.value()] : Bitboard::Ones();

    // Single push
    {
      Bitboard single_push_bb = single_push[from.value()];
      single_push_bb &= ~occupied;
      single_push_bb &= check_mask;
      single_push_bb &= pin_mask;

      if (single_push_bb) {
        Square to = single_push_bb.pop_lsb().value();

        if (is_promotion_rank(to, us)) {
          add_pawn_promotions(moves, from, to, us, false);
        } else {
          moves.emplace_back(from, to, std::nullopt, false, false, false);
        }
      }
    }

    // Double push
    {
      if (is_starting_rank(from, us)) {
        Bitboard double_push_bb = double_push[from.value()];
        double_push_bb &= ~occupied;
        double_push_bb &= check_mask;
        double_push_bb &= pin_mask;

        Bitboard single_push_bb = single_push[from.value()];
        single_push_bb &= occupied;

        if (single_push_bb.empty() && double_push_bb) {
          Square to = double_push_bb.pop_lsb().value();
          moves.emplace_back(from, to, std::nullopt, false, false, false);
        }
      }
    }

    // Captures
    {
      Bitboard captures_bb = captures[from.value()];
      captures_bb &= enemy;
      captures_bb &= check_mask;
      captures_bb &= pin_mask;

      for (Square to : captures_bb) {
        if (is_promotion_rank(to, us)) {
          add_pawn_promotions(moves, from, to, us, true);
        } else {
          moves.emplace_back(from, to, std::nullopt, true, false, false);
        }
      }
    }

    // En passant
    if (epsq_opt && can_capture_en_passant(from, epsq_opt.value(), us)) {
      Square epsq = epsq_opt.value();
      Bitboard en_passant_bb = Bitboard(epsq);
      en_passant_bb &= check_mask;
      en_passant_bb &= pin_mask;

      if (en_passant_bb) {
        Square cap_sq = (us == Color::WHITE) ? Square(epsq.flat_index() - Const::BOARD_WIDTH)
                                             : Square(epsq.flat_index() + Const::BOARD_WIDTH);

        Board tmp(board);
        tmp.remove_piece(cap_sq);
        tmp.move_piece(from, epsq_opt.value());

        if (!attackers_to(king_sq, us)) {
          moves.emplace_back(from, epsq_opt.value(), std::nullopt, true, true, false);
        }
      }
    }
  }
}
