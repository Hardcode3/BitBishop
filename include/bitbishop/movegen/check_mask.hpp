#pragma once

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/square.hpp>

namespace bitbishop {

Bitboard compute_check_mask(Square king_sq, const Bitboard& checkers, const Board& board);

}  // namespace bitbishop