#include <bitbishop/constants.hpp>
#include <bitbishop/moves/move_builder.hpp>

MoveBuilder::MoveBuilder(const Board& board, const Move& move) : board(board), move(move) {
  moving_piece = *board.get_piece(move.from);
  opt_captured_piece = board.get_piece(move.to);

  final_piece = moving_piece;

  prev_state = board.get_state();
  next_state = board.get_state();
}

MoveExecution MoveBuilder::build() {
  prepare_base_state();

  remove_moving_piece();
  handle_regular_capture();
  handle_en_passant_capture();
  handle_promotion();
  place_final_piece();

  prepare_next_state();

  return effects;
}

void MoveBuilder::prepare_base_state() {
  flip_side_to_move();
  update_half_move_clock();
  update_full_move_number();
  reset_en_passant_square();
}

void MoveBuilder::prepare_next_state() {
  handle_rook_castling();
  update_castling_rights();
  update_en_passant_square();
  commit_state();
}

void MoveBuilder::remove_moving_piece() { effects.add(MoveEffect::remove(move.from, moving_piece)); }

void MoveBuilder::handle_regular_capture() {
  if (opt_captured_piece && !move.is_en_passant) {
    effects.add(MoveEffect::remove(move.to, *opt_captured_piece));
  }
}

void MoveBuilder::handle_en_passant_capture() {
  if (!move.is_en_passant) {
    return;
  }

  using namespace Const;
  int direction = prev_state.m_is_white_turn ? -BOARD_WIDTH : +BOARD_WIDTH;
  Square en_passant_sq(move.to.value() + direction);
  Piece captured_piece = *board.get_piece(en_passant_sq);
  effects.add(MoveEffect::remove(en_passant_sq, captured_piece));
}

void MoveBuilder::handle_promotion() {
  if (move.promotion) {
    final_piece = *move.promotion;
  }
}

void MoveBuilder::place_final_piece() { effects.add(MoveEffect::place(move.to, final_piece)); }

void MoveBuilder::handle_rook_castling() {
  if (!move.is_castling) {
    return;
  }

  using namespace Const;
  const bool is_kingside = move.to.value() > move.from.value();
  const int from_rank = move.from.rank();
  Color color = (prev_state.m_is_white_turn) ? Color::WHITE : Color::BLACK;
  Piece rook_piece = Piece(Piece::Type::ROOK, color);

  if (is_kingside) {
    Square rook_from = Square(FILE_H_IND, from_rank);
    Square rook_to = Square(FILE_F_IND, from_rank);

    effects.add(MoveEffect::remove(rook_from, rook_piece));
    effects.add(MoveEffect::place(rook_to, rook_piece));
  } else {
    Square rook_from = Square(FILE_A_IND, from_rank);
    Square rook_to = Square(FILE_D_IND, from_rank);

    effects.add(MoveEffect::remove(rook_from, rook_piece));
    effects.add(MoveEffect::place(rook_to, rook_piece));
  }
}

void MoveBuilder::revoke_castling_if_rook_at(Square sq) {
  using namespace Squares;

  if (sq == A1) {
    next_state.m_white_castle_queenside = false;
  }
  if (sq == H1) {
    next_state.m_white_castle_kingside = false;
  }
  if (sq == A8) {
    next_state.m_black_castle_queenside = false;
  }
  if (sq == H8) {
    next_state.m_black_castle_kingside = false;
  }
}

void MoveBuilder::revoke_castling_if_king_at(Square sq) {
  using namespace Squares;

  if (sq == E1) {
    next_state.m_white_castle_queenside = false;
    next_state.m_white_castle_kingside = false;
  }
  if (sq == E8) {
    next_state.m_black_castle_queenside = false;
    next_state.m_black_castle_kingside = false;
  }
}

void MoveBuilder::update_castling_rights() {
  if (moving_piece.is_king()) {
    revoke_castling_if_king_at(move.from);
  }

  if (moving_piece.is_rook()) {
    revoke_castling_if_rook_at(move.from);
  }

  const bool is_rook_captured = opt_captured_piece ? (*opt_captured_piece).is_rook() : false;
  if (is_rook_captured) {
    revoke_castling_if_rook_at(move.to);
  }
}

void MoveBuilder::update_en_passant_square() {
  if (!moving_piece.is_pawn()) {
    return;
  }

  const int delta_rank = move.to.rank() - move.from.rank();
  if (delta_rank == 2) {
    next_state.m_en_passant_sq = Square(move.from.file(), move.from.rank() + 1);
  } else if (delta_rank == -2) {
    next_state.m_en_passant_sq = Square(move.from.file(), move.from.rank() - 1);
  }
}

void MoveBuilder::commit_state() { effects.add(MoveEffect::state_change(prev_state, next_state)); }

void MoveBuilder::update_half_move_clock() {
  if (move.is_capture || moving_piece.is_pawn()) {
    next_state.m_halfmove_clock = 0;
  } else {
    next_state.m_halfmove_clock++;
  }
}

void MoveBuilder::update_full_move_number() {
  if (!next_state.m_is_white_turn) {
    next_state.m_fullmove_number++;
  }
}

void MoveBuilder::flip_side_to_move() { next_state.m_is_white_turn = !prev_state.m_is_white_turn; }

void MoveBuilder::reset_en_passant_square() { next_state.m_en_passant_sq = std::nullopt; }
