#pragma once
#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/pawn.hpp>
#include <bitbishop/move.hpp>
#include <utility>
#include <vector>

/**
 * @brief Generates pawn moves including pushes, captures, promotions, and en passant.
 *
 * Pawns have unique movement rules in chess:
 * - Move forward one square (or two squares from starting position)
 * - Capture diagonally forward one square
 * - Promote to Queen, Rook, Bishop, or Knight when reaching the opposite end
 * - Can capture en passant under specific conditions
 */
namespace PawnMoveGenerator {

constexpr std::array<Piece, 4> WHITE_PROMOTIONS = {Pieces::WHITE_QUEEN, Pieces::WHITE_ROOK, Pieces::WHITE_BISHOP,
                                                   Pieces::WHITE_KNIGHT};
constexpr std::array<Piece, 4> BLACK_PROMOTIONS = {Pieces::BLACK_QUEEN, Pieces::BLACK_ROOK, Pieces::BLACK_BISHOP,
                                                   Pieces::BLACK_KNIGHT};

/**
 * @brief Generates all pseudo-legal pawn moves for a given side.
 *
 * Pseudo-legal moves follow piece movement rules but may leave the king in check.
 * These moves must be validated separately to ensure legality.
 *
 * @param moves Vector to append generated moves to
 * @param board Current board state
 * @param side Color of the side to generate moves for
 */
void generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

/**
 * @brief Generates all legal pawn moves for a given side.
 *
 * Legal moves are pseudo-legal moves that have been validated to ensure
 * the king is not left in check after the move.
 *
 * @param moves Vector to append generated moves to
 * @param board Current board state
 * @param side Color of the side to generate moves for
 */
void generate_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

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
void add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, Color side, bool is_capture);

/**
 * @brief Checks if a square is on the pawn's starting rank.
 *
 * Pawns start on rank 2 (index 1) for White and rank 7 (index 6) for Black.
 * This is used to determine if a pawn is eligible to perform a double push.
 *
 * @param sq Square to check
 * @param c Color of the pawn
 * @return true if the square is on the starting rank for the given color, false otherwise
 */
constexpr bool is_starting_rank(Square sq, Color c) { return c == Color::WHITE ? (sq.rank() == 1) : (sq.rank() == 6); }

/**
 * @brief Checks if a square is on the pawn's promotion rank.
 *
 * Pawns promote when reaching rank 8 (index 7) for White or rank 1 (index 0) for Black.
 *
 * @param sq Square to check
 * @param c Color of the pawn
 * @return true if the square is on the promotion rank for the given color, false otherwise
 */
constexpr bool is_promotion_rank(Square sq, Color c) { return c == Color::WHITE ? (sq.rank() == 7) : (sq.rank() == 0); }

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
  int df = std::abs(int(from.file()) - int(epsq.file()));
  if (df != 1) return false;
  if (side == Color::WHITE && from.rank() == 4 && epsq.rank() == 5) return true;
  if (side == Color::BLACK && from.rank() == 3 && epsq.rank() == 2) return true;
  return false;
}

/**
 * @brief Returns pre-computed single push destinations for all squares.
 *
 * For each of the 64 squares, provides a bitboard with the destination square
 * if a pawn of the given color pushes forward one rank (White moves up/+1, Black moves down/-1).
 *
 * Note: This returns theoretical destinations regardless of board occupancy.
 * The caller must verify that the destination square is empty before making the move.
 *
 * @param side Color of the pawns
 * @return Array of 64 bitboards indexed by source square, each containing the single push destination
 */
constexpr std::array<Bitboard, 64> single_push(Color side) {
  switch (side) {
    case Color::WHITE:
      return Lookups::WHITE_PAWN_SINGLE_PUSH;
    case Color::BLACK:
      return Lookups::BLACK_PAWN_SINGLE_PUSH;
  }
  std::unreachable();
}

/**
 * @brief Returns pre-computed double push destinations for all squares.
 *
 * For each of the 64 squares, provides a bitboard with the destination square
 * if a pawn of the given color pushes forward two ranks from its starting position
 * (White: rank 2 to rank 4, Black: rank 7 to rank 5).
 *
 * Note: This returns theoretical destinations. The caller must verify:
 * - The pawn is on its starting rank
 * - Both the intermediate square and the destination square are empty
 *
 * @param side Color of the pawns
 * @return Array of 64 bitboards indexed by source square, each containing the double push destination
 */
constexpr std::array<Bitboard, 64> double_push(Color side) {
  switch (side) {
    case Color::WHITE:
      return Lookups::WHITE_PAWN_DOUBLE_PUSH;
    case Color::BLACK:
      return Lookups::BLACK_PAWN_DOUBLE_PUSH;
  }
  std::unreachable();
}

/**
 * @brief Returns pre-computed diagonal capture destinations for all squares.
 *
 * For each of the 64 squares, provides a bitboard with all squares a pawn of the
 * given color could potentially capture to (one square diagonally forward-left and
 * forward-right).
 *
 * Note: This returns theoretical capture squares regardless of board occupancy.
 * The caller must verify that an enemy piece or en passant target actually occupies
 * the destination square.
 *
 * @param side Color of the pawns
 * @return Array of 64 bitboards indexed by source square, each containing the diagonal capture squares
 */
constexpr std::array<Bitboard, 64> captures(Color side) {
  switch (side) {
    case Color::WHITE:
      return Lookups::WHITE_PAWN_ATTACKS;
    case Color::BLACK:
      return Lookups::BLACK_PAWN_ATTACKS;
  }
  std::unreachable();
}

}  // namespace PawnMoveGenerator
