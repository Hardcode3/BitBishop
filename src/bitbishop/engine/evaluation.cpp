#include <bitbishop/engine/evaluation.hpp>

int Eval::evaluate_material(const Board& board, Color side) noexcept {
  int score = 0;
  score += board.pawns(side).count() * MaterialValue::PAWN;
  score += board.knights(side).count() * MaterialValue::KNIGHT;
  score += board.bishops(side).count() * MaterialValue::BISHOP;
  score += board.rooks(side).count() * MaterialValue::ROOK;
  score += board.queens(side).count() * MaterialValue::QUEEN;
  // no assumption about the number of kings, should aways be one
  score += board.king(side).count() * MaterialValue::KING;
  return score;
}

int Eval::compute_psqt_from_table(const PieceSquareTable& psqt, const Bitboard& bitboard, Color side) noexcept {
  int score = 0;
  for (Square sq : bitboard) {
    score += psqt[sq.flat_index()];
  }
  return score;
}

int Eval::evaluate_psqt(const Board& board, Color side) noexcept {
  int score = 0;
  if (side == Color::WHITE) {
    score += compute_psqt_from_table(PAWN_PSQT_WHITE, board.pawns(side), side);
    score += compute_psqt_from_table(KNIGHT_PSQT_WHITE, board.knights(side), side);
    score += compute_psqt_from_table(BISHOP_PSQT_WHITE, board.bishops(side), side);
    score += compute_psqt_from_table(ROOK_PSQT_WHITE, board.rooks(side), side);
    score += compute_psqt_from_table(QUEEN_PSQT_WHITE, board.queens(side), side);
    score += compute_psqt_from_table(KING_MIDGAME_PSQT_WHITE, board.king(side), side);
  } else {
    score += compute_psqt_from_table(PAWN_PSQT_BLACK, board.pawns(side), side);
    score += compute_psqt_from_table(KNIGHT_PSQT_BLACK, board.knights(side), side);
    score += compute_psqt_from_table(BISHOP_PSQT_BLACK, board.bishops(side), side);
    score += compute_psqt_from_table(ROOK_PSQT_BLACK, board.rooks(side), side);
    score += compute_psqt_from_table(QUEEN_PSQT_BLACK, board.queens(side), side);
    score += compute_psqt_from_table(KING_MIDGAME_PSQT_BLACK, board.king(side), side);
  }
  return score;
}

int Eval::evaluate(const Board& board, Color side) noexcept {
  int score = 0;
  score += evaluate_material(board, side);
  score += evaluate_psqt(board, side);
  return (side == Color::WHITE) ? score : -score;
}
