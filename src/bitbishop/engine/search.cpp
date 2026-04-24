#include <bitbishop/engine/evaluation.hpp>
#include <bitbishop/engine/search.hpp>
#include <bitbishop/movegen/legal_moves.hpp>
#include <bitbishop/moves/position.hpp>

// https://www.chessprogramming.org/Quiescence_Search
int Search::quiesce(Position& position, int alpha, int beta, SearchStats& stats, std::atomic<bool>* stop_flag) {
  stats.quiescence_nodes++;

  if (stop_flag != nullptr && stop_flag->load()) {
    return alpha;
  }

  if (position.is_threefold_repetition()) {
    return 0;
  }

  const Board& board = position.get_board();

  if (board.get_state().m_halfmove_clock >= Const::MAX_HALF_MOVES_BEFORE_DRAW) {
    return 0;
  }

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
  generate_legal_capture_moves(moves, board);

  for (const Move& move : moves) {
    if (stop_flag != nullptr && stop_flag->load()) {
      return alpha;
    }
    position.apply_move(move);

    // Quiescence window flip: child is searched with (-beta, -alpha) and the returned score is negated.
    // This relies on `ALPHA_INIT` not being `INT_MIN` (see `include/bitbishop/engine/search.hpp`).
    int score = -quiesce(position, -beta, -alpha, stats, stop_flag);
    position.revert_move();

    if (stop_flag != nullptr && stop_flag->load()) {
      return alpha;
    }

    if (score >= beta) {
      return beta;
    }

    alpha = std::max(alpha, score);
  }
  return alpha;
}

Search::BestMove Search::negamax(Position& position, std::size_t depth, int alpha, int beta, int ply,
                                 SearchStats& stats, std::atomic<bool>* stop_flag) {
  stats.negamax_nodes++;

  const Board& board = position.get_board();

  BestMove best;
  std::vector<Move> moves;

  if (stop_flag != nullptr && stop_flag->load()) {
    best.score = 0;
    return best;
  }

  if (position.is_threefold_repetition()) {
    best.score = 0;
    return best;
  }

  if (depth == 0) {
    best.score = quiesce(position, alpha, beta, stats, stop_flag);
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

  if (board.get_state().m_halfmove_clock >= Const::MAX_HALF_MOVES_BEFORE_DRAW) {
    best.score = 0;
    return best;
  }

  int bestScore = ALPHA_INIT;
  for (const Move& move : moves) {
    if (stop_flag != nullptr && stop_flag->load()) {
      best.score = bestScore;
      return best;
    }
    position.apply_move(move);
    // Negamax window flip: child is searched with (-beta, -alpha) and the returned score is negated.
    // This relies on `ALPHA_INIT` not being `INT_MIN` (see `include/bitbishop/engine/search.hpp`).
    int score = -negamax(position, depth - 1, -beta, -alpha, ply + 1, stats, stop_flag).score;
    position.revert_move();

    if (stop_flag != nullptr && stop_flag->load()) {
      best.score = bestScore;
      return best;
    }

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
