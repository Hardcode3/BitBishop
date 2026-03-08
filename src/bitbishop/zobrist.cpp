#include <bitbishop/zobrist.hpp>

Zobrist::Key Zobrist::compute_hash(const Board& board) {
  using namespace Zobrist;

  Key key = 0;

  for (int sq = 0; sq < Const::BOARD_SIZE; ++sq) {
    Square square = Square(sq);

    auto piece = board.get_piece(square);

    if (piece) {
      int p = piece_index(*piece);
      key ^= tables.pieces[p][sq];
    }
  }

  if (board.get_side_to_move() == Color::BLACK) {
    key ^= tables.side;
  }

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
    castling |= 8;
  }

  key ^= tables.castling[castling];

  auto ep = board.en_passant_square();

  if (ep) {
    key ^= tables.enpassant[ep->file()];
  }

  return key;
}
