#pragma once
#include <chess_engine/attacks/pawn.hpp>
#include <chess_engine/movegen/piece_move_gen.hpp>
#include <utility>

/**
 * @brief Generates pawn moves including pushes, captures, promotions, and en passant.
 *
 * Pawns have unique movement rules in chess:
 * - Move forward one square (or two from starting position)
 * - Capture diagonally one square
 * - Promote when reaching the opposite end of the board
 * - Can capture en passant under specific conditions
 */
class PawnMoveGenerator : public PieceMoveGenerator {
 public:
  /**
   * @brief Generates all pseudo-legal pawn moves for a given side.
   *
   * Pseudo-legal means the move is valid according to piece movement rules,
   * but may leave the king in check. These moves must be validated separately.
   *
   * @param board Current board state
   * @param side Color of the side to generate moves for
   * @return Vector of all pseudo-legal pawn moves
   */
  std::vector<Move> generate_pseudo_legal_moves(const Board& board, Color side) const override;

  /**
   * @brief Generates all legal pawn moves for a given side.
   *
   * Legal moves are pseudo-legal moves that don't leave the king in check.
   *
   * @param board Current board state
   * @param side Color of the side to generate moves for
   * @return Vector of all legal pawn moves
   */
  std::vector<Move> generate_legal_moves(const Board& board, Color side) const override;

  /**
   * @brief Adds all four promotion moves (Queen, Rook, Bishop, Knight) to the move list.
   *
   * When a pawn reaches the opposite end of the board, it must promote to another piece.
   * This function adds all four possible promotions for a single pawn move.
   *
   * @param moves Vector to append promotion moves to
   * @param from Source square of the promoting pawn
   * @param to Destination square (must be on promotion rank)
   * @param side Color of the promoting pawn
   * @param is_capture Whether the promotion involves capturing an enemy piece
   */
  static void add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, Color side, bool is_capture);

  /**
   * @brief Checks if a square is on the pawn's starting rank.
   *
   * Pawns start on rank 2 (index 1) for White and rank 7 (index 6) for Black.
   * This is used to determine if a pawn can perform a double push.
   *
   * @param sq Square to check
   * @param c Color of the pawn
   * @return true if the square is on the starting rank for the given color
   */
  static constexpr bool is_starting_rank(Square sq, Color c) {
    return c == Color::WHITE ? (sq.rank() == 1) : (sq.rank() == 6);
  }

    /**
     * @brief Checks if a square is on the pawn's promotion rank.
     *
     * Pawns promote on rank 8 (index 7) for White and rank 1 (index 0) for Black.
     *
     * @param sq Square to check
     * @param c Color of the pawn
     * @return true if the square is on the promotion rank for the given color
     */
    static constexpr bool is_promotion_rank(Square sq, Color c) {
      return c == Color::WHITE ? (sq.rank() == 7) : (sq.rank() == 0);
    }

    /**
     * @brief Validates if an en passant capture is legal based on square positions.
     *
     * En passant is valid when:
     * - The capturing pawn is on rank 5 for White (index 4) or rank 4 for Black (index 3)
     * - The en passant square is one file adjacent and one rank forward
     * - The en passant square is on rank 6 for White (index 5) or rank 3 for Black (index 2)
     *
     * Note: This only checks geometric validity. The caller must verify that en passant
     * is actually available (i.e., an enemy pawn just performed a double push).
     *
     * @param from Square of the capturing pawn
     * @param epsq Available en passant target square
     * @param side Color of the capturing pawn
     * @return true if the capture geometry is valid for en passant
     */
    static constexpr bool can_capture_en_passant(Square from, Square epsq, Color side) noexcept {
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
     * if a pawn of the given color pushes forward one rank (White: +1, Black: -1).
     *
     * Note: This returns theoretical destinations regardless of occupancy.
     * Caller must check that the destination square is empty.
     *
     * @param side Color of the pawns
     * @return Array of 64 bitboards, indexed by source square
     */
    static std::array<Bitboard, 64> single_push(Color side) {
      switch (side) {
        case Color::WHITE:
          return Attacks::WHITE_PAWN_SINGLE_PUSH;
        case Color::BLACK:
          return Attacks::BLACK_PAWN_SINGLE_PUSH;
      }
      std::unreachable();
    }

    /**
     * @brief Returns pre-computed double push destinations for all squares.
     *
     * For each of the 64 squares, provides a bitboard with the destination square
     * if a pawn of the given color pushes forward two ranks from its starting position.
     *
     * Note: This returns theoretical destinations. Caller must verify:
     * - The pawn is on its starting rank
     * - Both the intermediate square and destination square are empty
     *
     * @param side Color of the pawns
     * @return Array of 64 bitboards, indexed by source square
     */
    static std::array<Bitboard, 64> double_push(Color side) {
      switch (side) {
        case Color::WHITE:
          return Attacks::WHITE_PAWN_DOUBLE_PUSH;
        case Color::BLACK:
          return Attacks::BLACK_PAWN_DOUBLE_PUSH;
      }
      std::unreachable();
    }

    /**
     * @brief Returns pre-computed diagonal capture destinations for all squares.
     *
     * For each of the 64 squares, provides a bitboard with all diagonal squares
     * a pawn of the given color could capture to (one square diagonally forward).
     *
     * Note: This returns theoretical capture squares regardless of occupancy.
     * Caller must check that an enemy piece actually occupies the destination.
     *
     * @param side Color of the pawns
     * @return Array of 64 bitboards, indexed by source square
     */
    static std::array<Bitboard, 64> captures(Color side) {
      switch (side) {
        case Color::WHITE:
          return Attacks::WHITE_PAWN_ATTACKS;
        case Color::BLACK:
          return Attacks::BLACK_PAWN_ATTACKS;
      }
      std::unreachable();
    }
};
