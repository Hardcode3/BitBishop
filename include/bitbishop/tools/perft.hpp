#pragma once

#include <bitbishop/board.hpp>

namespace Tools {

/**
 * @brief Perft (Performance Test) debug function to walk through the move generation tree
 *  of legal moves of a certain depth.
 *
 * @param board Reference to the board on which perft must be executed
 * @param depth Recursion depth
 *
 * @see https://www.chessprogramming.org/Perft
 */
uint64_t perft(Board& board, std::size_t depth);

/**
 * @brief Perft Divide (Performance Test) debug function to walk through the move generation tree
 *  of legal moves of a certain depth and print move count for each move.
 *
 * @param board Reference to the board on which perft divide must be executed
 * @param depth Recursion depth
 *
 * @see https://www.chessprogramming.org/Perft
 */
void perft_divide(Board& board, std::size_t depth);

}  // namespace Tools
