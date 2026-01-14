#pragma once

#include <bitbishop/movegen/legal_moves.hpp>
#include <bitbishop/moves/position.hpp>
#include <iomanip>
#include <iostream>

namespace Tools {

uint64_t perft(Board& board, std::size_t depth) {
  uint64_t nodes = 0;

  if (depth == 0) {
    return 1;
  }

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  Position p(board);
  for (const Move& move : moves) {
    p.apply_move(move);
    nodes += perft(board, depth - 1);
    p.revert_move();
  }
  return nodes;
}

// Perft divide - shows count for each root move
void perft_divide(Board& board, std::size_t depth) {
  uint64_t total_nodes = 0;

  std::vector<Move> moves;
  generate_legal_moves(moves, board);

  Position p(board);
  for (const Move& move : moves) {
    p.apply_move(move);
    uint64_t nodes = (depth == 1) ? 1 : perft(board, depth - 1);
    p.revert_move();

    std::cout << move.to_uci() << ": " << nodes << std::endl;
    total_nodes += nodes;
  }

  std::cout << "\nNodes searched: " << total_nodes << std::endl;
}

}  // namespace Tools