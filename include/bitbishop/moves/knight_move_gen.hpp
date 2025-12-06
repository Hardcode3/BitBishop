#include <bitbishop/board.hpp>
#include <bitbishop/move.hpp>
#include <vector>

namespace KnightMoveGenerator {

void generate_pseudo_legal_moves(std::vector<Move>& moves, const Board& board, Color side);

}