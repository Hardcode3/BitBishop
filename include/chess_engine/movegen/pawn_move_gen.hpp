#pragma once

#include <chess_engine/attacks/pawn.hpp>
#include <chess_engine/movegen/piece_move_gen.hpp>

class PawnMoveGenerator : public PieceMoveGenerator {
 public:
  std::vector<Move> generate_pseudo_legal_moves(const Board& board, Color side) const override;
  std::vector<Move> generate_legal_moves(const Board& board, Color side) const override;

  /**
   * @brief Adds pawn promotion moves to existing moves.
   * @param moves Moves array to add promotions moves into
   * @param from Square from which the piece moves
   * @param to Square to which the piece moves
   * @param side Side to move color
   * @param is_capture Whether the move is a capture or not
   */
  static void add_pawn_promotions(std::vector<Move>& moves, Square from, Square to, Color side, bool is_capture);

  /**
   * @brief Tells if a square of a given color is on the starting rank.
   * @param sq Square to test
   * @param c Side to move color
   * @return true if the square is on the starting rank, false otherwise
   */
  static constexpr bool is_starting_rank(Square sq, Color c) {
    return c == Color::WHITE ? (sq.rank() == 1) : (sq.rank() == 6);
  }

  /**
   * @brief Tells if a square of a given color is on the promotion rank.
   * @param sq Square to test
   * @param c Side to move color
   * @return true if the square is on the promotion rank, false otherwise
   */
  static constexpr bool is_promotion_rank(Square sq, Color c) {
    return c == Color::WHITE ? (sq.rank() == 7) : (sq.rank() == 0);
  }

  /**
   * @brief Tells if the capture is a valid en-passant capture.
   * @param from Pawn's square that could capture
   * @param epsq Available en-passant square
   * @param Color Side to move color
   * @return true if the capture is a valid en-passant capture
   */
  static constexpr bool can_capture_en_passant(Square from, Square epsq, Color side) noexcept {
    int df = std::abs(int(from.file()) - int(epsq.file()));
    if (df != 1) return false;
    if (side == Color::WHITE && from.rank() == 4 && epsq.rank() == 5) return true;
    if (side == Color::BLACK && from.rank() == 3 && epsq.rank() == 2) return true;
    return false;
  }

  /**
   * @brief Retrieves pawn single push moves based on it's color.
   * @param side Side to move color
   * @return Array of Bitboard objects, corresponding to single pushes.
   *
   * For each square, contains a bitboard with the destination square if a white or black pawn moves one square forward.
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
   * @brief Retrieves pawn double push moves based on it's color.
   * @param side Side to move color
   * @return Array of Bitboard objects, corresponding to double pushes.
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
   * @brief Retrieves pawn diagonal captures based on it's color.
   * @param side Side to move color
   * @return Array of Bitboard objects, each one for an available capture.
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
