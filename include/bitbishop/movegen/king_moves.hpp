#include <bitbishop/board.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/king_attacks.hpp>
#include <bitbishop/move.hpp>
#include <utility>
#include <vector>

void generate_legal_king_moves(std::vector<Move>& moves, const Board& board, Color us, Square king_sq,
                               const Bitboard& enemy_attacks, const Bitboard& check_mask) {
  const Bitboard own = board.friendly(us);
  const Bitboard enemy = board.enemy(us);

  Bitboard candidates = Lookups::KING_ATTACKS[king_sq.value()];

  candidates &= ~own;
  candidates &= ~enemy_attacks;
  candidates &= check_mask;

  for (Square to : candidates) {
    const bool is_capture = enemy.test(to);
    moves.emplace_back(king_sq, to, std::nullopt, is_capture, false, false);
  }
}
