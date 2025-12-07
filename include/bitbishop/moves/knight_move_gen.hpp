#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <vector>

/**
 * @brief Generates knight moves.
 *
 * Knights can move in an 'L' shape.
 * This namespace provides functions for generating both pseudo-legal and legal knights moves.
 */
namespace KnightMoveGenerator {

/**
 * @brief Generates all pseudo-legal knights moves for a given side.
 *
 * Pseudo-legal moves follow piece movement rules but may leave the king in check.
 * These moves must be validated separately to ensure legality.
 *
 * @param moves Vector to append generated moves to
 * @param board Current board state
 * @param side Color of the side to generate moves for
 */
void generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

}  // namespace KnightMoveGenerator
