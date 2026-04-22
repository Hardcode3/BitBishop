#pragma once

#include <bitbishop/board.hpp>
#include <limits>
#include <optional>
#include <stop_token>

class Position;

namespace Search {

/**
 * @brief Contains statistics about a best move search.
 */
struct SearchStats {
  uint64_t negamax_nodes;     ///< Number of explored negamax nodes
  uint64_t quiescence_nodes;  ///< Number of explored quiescence nodes
};

// We implement negamax with alpha-beta by flipping the window at each ply:
//   score = -negamax(child, depth-1, -beta, -alpha, ...)
// That requires negating `alpha`/`beta`. Negating `INT_MIN` is undefined behaviour in C++,
// so we never use it as the initial alpha bound:
// In Two's Complement, the range of signed integers is asymmetric:
// [-2^(n-1), 2^(n-1) - 1].
// Negating the most negative value (e.g., -2147483648) results in a value
// that exceeds INT_MAX (2147483647). In C++, signed integer overflow
// is Undefined Behavior (UB). We use min() + 1 to ensure that -alpha
// is always representable within the bounds of a standard int.
CX_INLINE int ALPHA_INIT = std::numeric_limits<int>::min() + 1;
CX_INLINE int BETA_INIT = std::numeric_limits<int>::max();

struct BestMove {
  /**
   * @brief Best move to play, only meaningfful at root
   */
  std::optional<Move> move;

  /**
   * @brief Evaluation for the current side to move
   */
  int score = ALPHA_INIT;
};

/**
 * @brief Finds the score using only quiet moves.
 *
 * @param position Current position (board + history)
 * @param alpha Best score the current side can guarantee
 * @param beta Best score the opponent side can guarantee
 * @param stats Statistics about the search process
 *
 * @return Score from the perspective of the side to move
 *
 * @note megamax depends on quiescence search
 *
 * What quiescence search is doing:
 * - Used in negamax when recursion depth has been reached
 * - Instead of evaluating directly at this depth, we enter a quiescence search
 * - It seaches quiet (non-capture) moves
 * - Avoids horizon effect
 *
 * @see https://www.chessprogramming.org/Quiescence_Search:
 *
 * """
 * Consider the situation where the last move you consider is QxP. If you stop there and evaluate, you might think that
 * you have won a pawn. But what if you were to search one move deeper and find that the next move is PxQ? You didn't
 * win a pawn, you actually lost a queen. Hence the need to make sure that you are evaluating only quiescent (quiet)
 * positions.
 * """
 */
[[nodiscard]] int quiesce(Position& position, int alpha, int beta, SearchStats& stats,
                          std::atomic<bool>* stop_flag = nullptr);

/**
 * @brief Finds the best achievable move for the side to move assuming an optimal play on both sides.
 *
 * @param position Current position (board + history)
 * @param depth Remaining search depth
 * @param alpha Lower bound, aka. minimum score we already guaranteed to get.
 * @param beta Upper bound, aka. maximum score the opponent is willing to let us have.
 * @param ply Number of half-moves from root used for mate distance
 * @param stats Statistics about the search process
 *
 * @return Move and score in a BestMove object
 *
 * Negamax is essentially a clever mathematical shortcut for the Minimax algorithm. While Minimax alternates between
 * "I want the highest score" and "My opponent wants the lowest score," Negamax uses a single rule: "I want to
 * maximize my score, and my opponent's gain is my loss." Because of the identity max(alpha, beta) = -min(-alpha,
 * -beta), we can simplify the code by negating the scores and swapping the roles of alpha and beta at every level.
 *
 * @see https://www.chessprogramming.org/Alpha-Beta
 * @see https://www.dogeystamp.com/chess2/
 */
[[nodiscard]] BestMove negamax(Position& position, std::size_t depth, int alpha, int beta, int ply, SearchStats& stats,
                               std::atomic<bool>* stop_flag = nullptr);

}  // namespace Search
