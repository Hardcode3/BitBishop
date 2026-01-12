#pragma once

#include <bitbishop/moves/move_execution.hpp>

/**
 * @brief Constructs a sequence of low-level effects for a move.
 *
 * MoveBuilder takes a high-level Move and a Board, and generates a
 * MoveExecution that includes piece placements, removals, promotions,
 * castling, en passant, and board state updates.
 */
class MoveBuilder {
 private:
  MoveExecution effects;  ///< Stores the sequence of effects for the move

  const Move& move;    ///< Reference to the move being built
  const Board& board;  ///< Reference to the board on which the move occurs

  Piece final_piece = Pieces::WHITE_KING;   ///< Piece to place at destination
  Piece moving_piece = Pieces::WHITE_KING;  ///< Piece moving from origin
  std::optional<Piece> opt_captured_piece;  ///< Optional captured piece
  BoardState prev_state, next_state;        ///< Board states before and after the move

 public:
  MoveBuilder() = delete;

  /**
   * @brief Constructs a MoveBuilder.
   * @param board Board on which the move is applied
   * @param move Move to build
   */
  MoveBuilder(const Board& board, const Move& move);

  /**
   * @brief Generates and returns the MoveExecution.
   * @return MoveExecution representing all low-level effects of the move
   */
  MoveExecution build();

 private:
  // Utilities for castling rights
  void revoke_castling_if_rook_at(Square sq);  ///< Revoke castling if rook moves or is captured
  void revoke_castling_if_king_at(Square sq);  ///< Revoke castling if king moves

  // Board state preparation
  void prepare_base_state();  ///< Flip side, reset en passant, update half/full move counters
  void prepare_next_state();  ///< Handle castling, en passant, commit state

  // Move effect steps
  void remove_moving_piece();        ///< Removes moving piece from origin
  void place_final_piece();          ///< Places final piece at destination
  void handle_regular_capture();     ///< Adds effect for normal captures
  void handle_en_passant_capture();  ///< Adds effect for en passant capture
  void handle_promotion();           ///< Updates piece if promotion occurs
  void handle_rook_castling();       ///< Handles rook movement during castling
  void update_castling_rights();     ///< Updates castling rights if king/rook moves or rook captured
  void update_en_passant_square();   ///< Sets en passant target square if pawn moved two squares
  void commit_state();               ///< Records board state change in effects
  void update_half_move_clock();     ///< Updates half-move counter for 50-move rule
  void update_full_move_number();    ///< Updates full move number if black has moved
  void flip_side_to_move();          ///< Flips active player
  void reset_en_passant_square();    ///< Clears en passant square if move doesn't set one
};
