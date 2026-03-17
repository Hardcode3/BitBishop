#include <bitbishop/zobrist.hpp>

void Zobrist::mutate_piece(Square square, Piece piece, Zobrist::Key& key) {
  const int sq_ind = square.flat_index();
  const int p_ind = piece_index(piece);
  key ^= tables.pieces[p_ind][sq_ind];
}

void Zobrist::mutate_side_to_move(Zobrist::Key& key) { key ^= tables.side; }

void Zobrist::mutate_castling_rights(const BoardState& state, Zobrist::Key& key) {
  int castling = 0;
  if (state.m_white_castle_kingside) {
    castling |= 1;
  }
  if (state.m_white_castle_queenside) {
    castling |= 2;
  }
  if (state.m_black_castle_kingside) {
    castling |= 4;
  }
  if (state.m_black_castle_queenside) {
    castling |= 8;  // NOLINT(readability-magic-numbers)
  }
  key ^= tables.castling[castling];
}

void Zobrist::mutate_board_state_diff(const BoardState& prev, const BoardState& next, Zobrist::Key& key) {
  if (prev.m_is_white_turn != next.m_is_white_turn) {
    mutate_side_to_move(key);
  }

  // 4-bits castling rights, currently not implemented like this in BoardState (booleans for the moment: TODO)
  // [ bq | bk | wq | wk ]
  //    3    2    1    0
  int prev_castle =
      (static_cast<int>(prev.m_white_castle_kingside) << 0) | (static_cast<int>(prev.m_white_castle_queenside) << 1) |
      (static_cast<int>(prev.m_black_castle_kingside) << 2) | (static_cast<int>(prev.m_black_castle_queenside) << 3);

  int next_castle =
      (static_cast<int>(next.m_white_castle_kingside) << 0) | (static_cast<int>(next.m_white_castle_queenside) << 1) |
      (static_cast<int>(next.m_black_castle_kingside) << 2) | (static_cast<int>(next.m_black_castle_queenside) << 3);

  if (prev_castle != next_castle) {
    mutate_castling_rights(prev, key);
    mutate_castling_rights(next, key);
  }

  if (prev.m_en_passant_sq != next.m_en_passant_sq) {
    if (prev.m_en_passant_sq) {
      mutate_en_passant_square(*prev.m_en_passant_sq, key);
    }
    if (next.m_en_passant_sq) {
      mutate_en_passant_square(*next.m_en_passant_sq, key);
    }
  }
}

void Zobrist::mutate_en_passant_square(Square epsq, Zobrist::Key& key) { key ^= tables.en_passant[epsq.file()]; }

Zobrist::Key Zobrist::compute_hash(const Board& board) {
  using namespace Zobrist;

  Key key = 0;

  for (int sq = 0; sq < Const::BOARD_SIZE; ++sq) {
    Square square(sq);
    std::optional<Piece> piece = board.get_piece(square);
    if (piece) {
      mutate_piece(square, *piece, key);
    }
  }

  if (board.get_side_to_move() == Color::BLACK) {
    mutate_side_to_move(key);
  }

  mutate_castling_rights(board.get_state(), key);

  auto epsq = board.en_passant_square();
  if (epsq) {
    mutate_en_passant_square(*epsq, key);
  }

  return key;
}
