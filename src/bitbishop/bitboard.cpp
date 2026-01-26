#include <bitbishop/bitboard.hpp>

void Bitboard::print() const {
  // defined in cpp to allow usage inside debugger

  using namespace Const;

  for (int rank = RANK_8_IND; rank >= RANK_1_IND; --rank) {
    for (int file = FILE_A_IND; file <= FILE_H_IND; ++file) {
      const Square square(file, rank);
      std::cout << (test(square) ? "1 " : ". ");
    }
    std::cout << "\n";
  }
}
