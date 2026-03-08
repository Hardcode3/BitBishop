#pragma once

#include <array>
#include <bitbishop/board.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/random.hpp>
#include <bitbishop/square.hpp>
#include <cstdint>

namespace Zobrist {

using Key = uint64_t;

/**
 * @brief Zobrist random key tables used for position hashing.
 *
 * Stores pre-generated random 64-bit keys for:
 * - each (piece, square) combination
 * - castling rights states
 * - en-passant file
 * - side to move
 *
 * These tables are XOR-combined to produce a unique hash for a board position.
 */
struct Tables {
  std::array<std::array<Key, Const::BOARD_SIZE>, Const::DISTINCT_PIECES_COUNT> pieces{};
  std::array<Key, 16> castling{};
  std::array<Key, 8> enpassant{};
  Key side{};
};

/**
 * @brief Generates deterministic Zobrist random tables.
 *
 * Uses the splitmix64 generator with a fixed seed so that the same
 * random keys are produced across program runs and builds.
 *
 * @return Fully initialized Zobrist key tables.
 */
CX_FN Tables generate() {
  using namespace Random;

  Tables t{};
  uint64_t seed = 0x994c3ac5e71fafcbULL;

  for (int p = 0; p < Const::DISTINCT_PIECES_COUNT; ++p) {
    for (int sq = 0; sq < Const::BOARD_SIZE; ++sq) {
      t.pieces[p][sq] = splitmix64(seed);
    }
  }

  for (int i = 0; i < 16; ++i) {
    t.castling[i] = splitmix64(seed);
  }

  for (int f = 0; f < 8; ++f) {
    t.enpassant[f] = splitmix64(seed);
  }

  t.side = splitmix64(seed);

  return t;
}

/**
 * @brief Global Zobrist tables instance.
 *
 * Generated once (typically at compile time) and reused for all hash
 * computations during program execution.
 */
CX_INLINE Tables tables = generate();

/**
 * @brief Converts a Piece to its Zobrist table index.
 *
 * Piece indices are arranged as:
 * - White pieces: 0–5
 * - Black pieces: 6–11
 *
 * Ordering within each color:
 * Pawn, Knight, Bishop, Rook, Queen, King.
 *
 * @param p Piece to convert.
 * @return Index into Tables::pieces.
 */
CX_FN int piece_index(Piece p) {
  int base = (p.color() == Color::WHITE) ? 0 : 6;

  switch (p.type()) {
    case Piece::Type::PAWN:
      return base + 0;
    case Piece::Type::KNIGHT:
      return base + 1;
    case Piece::Type::BISHOP:
      return base + 2;
    case Piece::Type::ROOK:
      return base + 3;
    case Piece::Type::QUEEN:
      return base + 4;
    case Piece::Type::KING:
      return base + 5;
  }

  return 0;
}

/**
 * @brief Computes the Zobrist hash for a board position.
 *
 * The hash is produced by XOR-combining keys for:
 * - all pieces on their squares
 * - current castling rights
 * - en-passant file (if present)
 * - side to move
 *
 * @param board Board position to hash.
 * @return 64-bit Zobrist hash key.
 */
Zobrist::Key compute_hash(const Board& board);

}  // namespace Zobrist
