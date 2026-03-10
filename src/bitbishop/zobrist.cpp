#include <bitbishop/zobrist.hpp>

Zobrist::Key Zobrist::compute_hash(const Board& board) {
  using namespace Zobrist;

  Key key = 0;

  /**
   * XOR the hash key for every piece on the board.
   *
   * Each piece contributes a unique random value based on:
   *   - its type and color (mapped via piece_index())
   *   - its square
   */
  for (int sq = 0; sq < Const::BOARD_SIZE; ++sq) {
    auto square = Square(sq);

    auto piece = board.get_piece(square);

    if (piece) {
      int p_ind = piece_index(*piece);
      key ^= tables.pieces[p_ind][sq];
    }
  }

  /**
   * Encode the side to move.
   *
   * Only one random key is required. By convention it is XORed when
   * black is to move.
   */
  if (board.get_side_to_move() == Color::BLACK) {
    key ^= tables.side;
  }

  /**
   * Encode castling rights.
   *
   * Castling rights are packed into a 32-bit integer where only lowest four bits are used.
   *
   * bit 0 (0001... = 1): white kingside
   * bit 1 (0010... = 2): white queenside
   * bit 2 (0100... = 4): black kingside
   * bit 3 (1000... = 8): black queenside
   *
   * This produces a value in the range [0, 15], which indexes the pre-generated castling Zobrist table.
   */
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

  /**
   * Encode en-passant availability.
   *
   * Only the file of the en-passant square matters because the rank
   * is implied by the side that performed the double pawn push.
   *
   * The file index (0–7) selects one of eight Zobrist keys.
   */
  auto epsq = board.en_passant_square();
  if (epsq) {
    key ^= tables.en_passant[epsq->file()];
  }

  return key;
}
