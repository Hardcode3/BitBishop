#include <bitbishop/engine/evaluation.hpp>

int Eval::evaluate_material(const Board& board, Color side) noexcept {
  int score = 0;
  score += board.pawns(side).count() * MaterialValue::PAWN;
  score += board.knights(side).count() * MaterialValue::KNIGHT;
  score += board.bishops(side).count() * MaterialValue::BISHOP;
  score += board.rooks(side).count() * MaterialValue::ROOK;
  score += board.queens(side).count() * MaterialValue::QUEEN;
  // no assumption about the number of kings
  score += board.king(side).count() * MaterialValue::KING;
  return score;
}

int Eval::compute_score_from_psqt(const PieceSquareTable& psqt, const Bitboard& bitboard) noexcept {
  int score = 0;
  for (Square sq : bitboard) {
    // flip the index so that vector ordering is compatible with bitboard msb ordering
    score += psqt[flip_index_horizontally(sq.flat_index())];
  }
  return score;
}

int Eval::evaluate_psqt(const Board& board, Color side) noexcept {
  int score = 0;
  if (side == Color::WHITE) {
    score += compute_score_from_psqt(PAWN_PSQT_WHITE, board.pawns(side));
    score += compute_score_from_psqt(KNIGHT_PSQT_WHITE, board.knights(side));
    score += compute_score_from_psqt(BISHOP_PSQT_WHITE, board.bishops(side));
    score += compute_score_from_psqt(ROOK_PSQT_WHITE, board.rooks(side));
    score += compute_score_from_psqt(QUEEN_PSQT_WHITE, board.queens(side));
    score += compute_score_from_psqt(KING_MIDGAME_PSQT_WHITE, board.king(side));
  } else {
    score += compute_score_from_psqt(PAWN_PSQT_BLACK, board.pawns(side));
    score += compute_score_from_psqt(KNIGHT_PSQT_BLACK, board.knights(side));
    score += compute_score_from_psqt(BISHOP_PSQT_BLACK, board.bishops(side));
    score += compute_score_from_psqt(ROOK_PSQT_BLACK, board.rooks(side));
    score += compute_score_from_psqt(QUEEN_PSQT_BLACK, board.queens(side));
    score += compute_score_from_psqt(KING_MIDGAME_PSQT_BLACK, board.king(side));
  }
  return score;
}

int Eval::evaluate(const Board& board, Color side) noexcept {
  int score = 0;
  score += evaluate_material(board, side);
  score += evaluate_psqt(board, side);
  return (side == Color::WHITE) ? score : -score;
}
