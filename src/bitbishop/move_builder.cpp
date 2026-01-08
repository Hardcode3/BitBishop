#include <bitbishop/move_builder.hpp>
#include <constants.hpp>

MoveBuilder::MoveBuilder(const Board& board, const Move& move) : m_board(board), m_move(move) {
  m_moving_piece = *board.get_piece(move.from);
  m_final_piece = m_moving_piece;
  m_captured = board.get_piece(move.to);

  m_prev_state = board.get_state();
  m_next_state = board.get_state();
}

void MoveBuilder::remove_moving_piece() { m_effects.add(MoveEffect::remove(m_move.from, m_moving_piece)); }

void MoveBuilder::regular_capture() {
  if (m_captured && !m_move.is_en_passant) {
    m_effects.add(MoveEffect::place(m_move.to, *m_captured));
  }
}

void MoveBuilder::en_passant_capture() {
  if (m_move.is_en_passant) {
    ;  // todo
  }
}

void MoveBuilder::handle_promotion() {
  if (m_move.promotion) {
    m_final_piece = *m_move.promotion;
  }
}

void MoveBuilder::place_final_piece() { m_effects.add(MoveEffect::place(m_move.to, m_final_piece)); }

void MoveBuilder::handle_rook_castling() {
  using namespace Const;

  if (m_move.is_castling) {
    const bool is_kingside = m_move.to.value() > m_move.from.value();
    const int from_rank = m_move.from.rank();
    Color color = (m_prev_state.m_is_white_turn) ? Color::WHITE : Color::BLACK;
    Piece rook_piece = Piece(Piece::Type::ROOK, color);

    if (is_kingside) {
      Square rook_from = Square(FILE_H_IND, from_rank);
      Square rook_to = Square(FILE_F_IND, from_rank);

      m_effects.add(MoveEffect::remove(rook_from, rook_piece));
      m_effects.add(MoveEffect::place(rook_to, rook_piece));
    } else {
      Square rook_from = Square(FILE_A_IND, from_rank);
      Square rook_to = Square(FILE_D_IND, from_rank);

      m_effects.add(MoveEffect::remove(rook_from, rook_piece));
      m_effects.add(MoveEffect::place(rook_to, rook_piece));
    }
  }
}

void MoveBuilder::update_castling_rights() {
  const bool is_king_moving = m_moving_piece.type() == Piece::Type::KING;
  if (is_king_moving) {
    if (m_prev_state.m_is_white_turn) {
      m_next_state.m_white_castle_kingside = false;
      m_next_state.m_white_castle_queenside = false;
    } else {
      m_next_state.m_black_castle_kingside = false;
      m_next_state.m_black_castle_queenside = false;
    }
  }

  const bool is_rook_moving = m_moving_piece.type() == Piece::Type::ROOK;
  const bool is_rook_captured = m_captured ? m_captured.type() == Piece::Type::ROOK : false;
  if (is_rook_moving || is_rook_captured) {
    // todo
  }
}

void MoveBuilder::add_en_passant_square() {
  const bool is_pawn_moving = m_moving_piece.type() == Piece::Type::PAWN;

  if (is_pawn_moving) {
    const int dr = m_move.to.rank() - m_move.from.rank();
    if (dr == 2) {
      m_next_state.m_en_passant_sq = Square(m_move.from.file(), m_move.from.rank() - 1);
    } else if (dr == -2) {
      m_next_state.m_en_passant_sq = Square(m_move.from.file(), m_move.from.rank() + 1);
    }
  }
}

void MoveBuilder::commit_state() { effects.add(MoveEffect::state_change(m_prev_state, m_next_state)); }

void MoveBuilder::update_half_move_clock() {
  const bool is_pawn_moving = m_moving_piece.type() == Piece::Type::PAWN;  // make it common?

  if (m_move.is_capture || is_pawn_moving) {
    m_next_state.m_halfmove_clock = 0;
  } else {
    m_next_state.m_halfmove_clock++;
  }
}

void MoveBuilder::update_full_move_number() {
  if (!m_prev_state.m_is_white_turn) {
    m_next_state.m_fullmove_number++;
  }
}

void MoveBuilder::flip_side_to_move() { m_next_state.m_is_white_turn = !m_prev_state.m_is_white_turn; }

void MoveBuilder::reset_en_passant_square() { m_next_state.m_en_passant_sq = std::nullopt; }