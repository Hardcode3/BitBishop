#include <bitbishop/zobrist.hpp>

void Zobrist::mutate_piece(Square square, Piece piece, Zobrist::Key& key) {
  const int sq_ind = square.flat_index();
  const int p_ind = piece_index(piece);
  key ^= tables.pieces[p_ind][sq_ind];
}

void Zobrist::mutate_side_to_move(Zobrist::Key& key) { key ^= tables.side; }

void Zobrist::mutate_castling_rights(const Board& board, Zobrist::Key& key) {
  int castling = 0;
  if (board.has_kingside_castling_rights(Color::WHITE)) {
    castling |= 1;
  }
  if (board.has_queenside_castling_rights(Color::WHITE)) {
    castling |= 2;
  }
  if (board.has_kingside_castling_rights(Color::BLACK)) {
    castling |= 4;
  }
  if (board.has_queenside_castling_rights(Color::BLACK)) {
    castling |= 8;  // NOLINT(readability-magic-numbers)
  }
  key ^= tables.castling[castling];
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

  mutate_castling_rights(board, key);

  auto epsq = board.en_passant_square();
  if (epsq) {
  }

  return key;
}
