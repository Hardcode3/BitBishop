#pragma once

#include <bitbishop/attacks/generate_attacks.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
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

/**
 * @brief Generates all single-square pawn pushes from the given square.
 *
 * Applies occupancy, check mask, and pin mask filters. If the target square
 * is a promotion rank, all promotion moves are added.
 *
 * @param moves Vector to append generated moves to
 * @param from Source square of the pawn
 * @param us Color of the pawn
 * @param occupied Bitboard of all occupied squares
 * @param check_mask Bitboard mask to restrict moves under check
 * @param pin_mask Bitboard mask to restrict moves due to pins
 */
inline void generate_single_push(std::vector<Move>& moves, Square from, Color us, const Bitboard& occupied,
                                 const Bitboard& check_mask, const Bitboard& pin_mask) {
  const auto& single_push = Lookups::PAWN_SINGLE_PUSH[ColorUtil::to_index(us)];

  Bitboard bb = single_push[from.flat_index()];
  bb &= ~occupied;
  bb &= check_mask;
  bb &= pin_mask;

  if (bb) {
    Square to = bb.pop_lsb().value();
    if (is_promotion_rank(to, us)) {
      add_pawn_promotions(moves, from, to, us, false);
    } else {
      moves.emplace_back(from, to, std::nullopt, false, false, false);
    }
  }
}

/**
 * @brief Generates all double-square pawn pushes from the given square.
 *
 * Only allowed from the starting rank. Ensures the intermediate square is empty
 * and respects occupancy, check mask, and pin mask constraints.
 *
 * @param moves Vector to append generated moves to
 * @param from Source square of the pawn
 * @param us Color of the pawn
 * @param occupied Bitboard of all occupied squares
 * @param check_mask Bitboard mask to restrict moves under check
 * @param pin_mask Bitboard mask to restrict moves due to pins
 */
inline void generate_double_push(std::vector<Move>& moves, Square from, Color us, const Bitboard& occupied,
                                 const Bitboard& check_mask, const Bitboard& pin_mask) {
  const auto& single_push = Lookups::PAWN_SINGLE_PUSH[ColorUtil::to_index(us)];
  const auto& double_push = Lookups::PAWN_DOUBLE_PUSH[ColorUtil::to_index(us)];

  if (!is_starting_rank(from, us)) {
    return;
  }

  Bitboard bb = double_push[from.flat_index()];
  bb &= ~occupied;
  bb &= check_mask;
  bb &= pin_mask;

  Bitboard single_bb = single_push[from.flat_index()] & occupied;
  if (single_bb.empty() && bb) {
    Square to = bb.pop_lsb().value();
    moves.emplace_back(from, to, std::nullopt, false, false, false);
  }
}

/**
 * @brief Generates all pawn capture moves from the given square.
 *
 * Only captures squares occupied by enemy pieces. Applies check mask and pin mask.
 * Automatically adds promotion moves if the destination is on the promotion rank.
 *
 * @param moves Vector to append generated moves to
 * @param from Source square of the pawn
 * @param us Color of the pawn
 * @param enemy Bitboard of enemy pieces
 * @param check_mask Bitboard mask to restrict moves under check
 * @param pin_mask Bitboard mask to restrict moves due to pins
 */
inline void generate_captures(std::vector<Move>& moves, Square from, Color us, const Bitboard& enemy,
                              const Bitboard& check_mask, const Bitboard& pin_mask) {
  const auto& captures = Lookups::PAWN_ATTACKS[ColorUtil::to_index(us)];

  Bitboard bb = captures[from.flat_index()];
  bb &= enemy;
  bb &= check_mask;
  bb &= pin_mask;

  for (Square to : bb) {
    if (is_promotion_rank(to, us)) {
      add_pawn_promotions(moves, from, to, us, true);
    } else {
      moves.emplace_back(from, to, std::nullopt, true, false, false);
    }
  }
}

/**
 * @brief Generates a pawn en passant move from the given square, if legal.
 *
 * Verifies en passant square exists, the pawn can capture, and that
 * performing the move does not leave the king in check.
 *
 * @param moves Vector to append generated moves to
 * @param from Source square of the pawn
 * @param us Color of the pawn
 * @param board Current board position
 * @param king_sq Square of the king for this side
 * @param check_mask Bitboard mask to restrict moves under check
 * @param pin_mask Bitboard mask to restrict moves due to pins
 */
inline void generate_en_passant(std::vector<Move>& moves, Square from, Color us, const Board& board, Square king_sq,
                                const Bitboard& check_mask, const Bitboard& pin_mask) {
  const std::optional<Square> epsq_opt = board.en_passant_square();

  if (!epsq_opt || !can_capture_en_passant(from, epsq_opt.value(), us)) {
    return;
  }

  const Color them = ColorUtil::opposite(us);
  Square epsq = epsq_opt.value();
  auto bb = Bitboard(epsq);
  bb &= check_mask;
  bb &= pin_mask;

  if (!bb) {
    return;
  }

  Square cap_sq = (us == Color::WHITE) ? Square(epsq.flat_index() - Const::BOARD_WIDTH)
                                       : Square(epsq.flat_index() + Const::BOARD_WIDTH);

  Board tmp(board);
  tmp.remove_piece(cap_sq);
  tmp.move_piece(from, epsq);

  Bitboard attackers = generate_attacks(tmp, them);

  if (!attackers.test(king_sq)) {
    moves.emplace_back(from, epsq, std::nullopt, true, true, false);
  }
}

/**
 * @brief Generates all legal pawn moves for the given side.
 *
 * Includes single pushes, double pushes, captures, promotions, and en passant,
 * taking into account occupancy, checks, and pins. Iterates over all pawns of
 * the specified color.
 *
 * @param moves Vector to append generated moves to
 * @param board Current board position
 * @param us Color of the side to generate moves for
 * @param king_sq Square of the king for this side
 * @param check_mask Bitboard mask to restrict moves under check
 * @param pins Pin result structure indicating which pieces are pinned
 */
void generate_pawn_legal_moves(std::vector<Move>& moves, const Board& board, Color us, Square king_sq,
                               const Bitboard& check_mask, const PinResult& pins) {
  const Bitboard enemy = board.enemy(us);
  const Bitboard occupied = board.occupied();
  Bitboard pawns = board.pawns(us);

  while (auto from_sq = pawns.pop_lsb()) {
    Square from = from_sq.value();
    const bool is_pinned = pins.pinned.test(from);
    const Bitboard pin_mask = is_pinned ? pins.pin_ray[from.flat_index()] : Bitboard::Ones();

    generate_single_push(moves, from, us, occupied, check_mask, pin_mask);
    generate_double_push(moves, from, us, occupied, check_mask, pin_mask);
    generate_captures(moves, from, us, enemy, check_mask, pin_mask);
    generate_en_passant(moves, from, us, board, king_sq, check_mask, pin_mask);
  }
}
