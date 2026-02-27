#include <bitbishop/engine/evaluation.hpp>
#include <bitbishop/engine/search.hpp>
#include <bitbishop/movegen/legal_moves.hpp>
#include <bitbishop/moves/position.hpp>

// https://www.chessprogramming.org/Quiescence_Search
int Search::quiesce(Board& board, int alpha, int beta) {
  Position position(board);

  if (board.has_insufficient_material()) {
    return 0;
  }

  if (!position.is_in_check()) {
    int stand_pat = Eval::evaluate(board);
    if (stand_pat >= beta) {
      return beta;
    }
    alpha = std::max(alpha, stand_pat);
  }

  std::vector<Move> moves;
  generate_legal_moves(moves, board);
  // Optimization: implement generate_capture_moves(moves, board); instead
  // To generate only capture moves and not all moves top discard some in the end

  for (const Move& move : moves) {
    if (!move.is_capture) {
      continue;
    }
    position.apply_move(move);
    int score = -quiesce(board, -beta, -alpha);
    position.revert_move();

    if (score >= beta) {
      return beta;
    }

    alpha = std::max(alpha, score);
  }
  return alpha;
}

Search::BestMove Search::negamax(Board& board, std::size_t depth, int alpha, int beta, int ply) {
  BestMove best;
  Position position(board);
  std::vector<Move> moves;

  if (depth == 0) {
    best.score = quiesce(board, alpha, beta);
    return best;
  }

  generate_legal_moves(moves, board);

  if (board.has_insufficient_material()) {
    best.score = 0;
    return best;
  }

  if (moves.empty()) {
    if (position.is_in_check()) {
      best.score = -Eval::MATE_SCORE + ply;
    } else {  // stalemate
      best.score = 0;
    }
    return best;
  }

  int bestScore = std::numeric_limits<int>::min();
  for (const Move& move : moves) {
    position.apply_move(move);
    int score = -negamax(board, depth - 1, -beta, -alpha, ply + 1).score;
    position.revert_move();

    if (score > bestScore) {
      bestScore = score;
      best.move = move;
    }

    alpha = std::max(score, alpha);

    if (alpha >= beta) {
      best.score = beta;
      return best;
    }
  }

  best.score = bestScore;
  return best;
}
