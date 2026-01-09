#pragma once

#include <bitbishop/move_execution.hpp>

class MoveBuilder {
 private:
  MoveExecution effects;

  const Move& move;
  const Board& board;

  Piece final_piece = Pieces::WHITE_KING;   // place holder
  Piece moving_piece = Pieces::WHITE_KING;  // place holder
  std::optional<Piece> opt_captured_piece;
  BoardState prev_state, next_state;

 public:
  MoveBuilder() = delete;
  MoveBuilder(const Board& board, const Move& move);
  MoveExecution build();

 private:
  // utilities
  void revoke_castling_if_rook_at(Square sq);
  void revoke_castling_if_king_at(Square sq);

  void prepare_base_state();
  void prepare_next_state();

  // move execution builder steps
  void remove_moving_piece();
  void place_final_piece();
  void handle_regular_capture();
  void handle_en_passant_capture();
  void handle_promotion();
  void handle_rook_castling();
  void update_castling_rights();
  void update_en_passant_square();
  void commit_state();
  void update_half_move_clock();
  void update_full_move_number();
  void flip_side_to_move();
  void reset_en_passant_square();
};
