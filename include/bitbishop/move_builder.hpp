#pragma once

#include <bitbishop/move_execution.hpp>

class MoveBuilder {
 private:
  MoveExecution m_effects;

  Move m_move;
  Board m_board;

  Piece m_final_piece;
  Piece m_moving_piece;
  std::optional<Piece> m_captured;
  BoardState m_prev_state, m_next_state;

 public:
  MoveBuilder() = delete;
  MoveBuilder(const Board& board, const Move& move);

  MoveExecution build() const;

  void remove_moving_piece();
  void regular_capture();
  void en_passant_capture();
  void handle_promotion();
  void place_final_piece();
  void handle_rook_castling();
  void update_castling_rights();
  void add_en_passant_square();
  void commit_state();
  void update_half_move_clock();
  void update_full_move_number();
  void flip_side_to_move();
  void reset_en_passant_square();
};
