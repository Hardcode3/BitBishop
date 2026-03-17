#pragma once

#include <array>
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/random.hpp>
#include <bitbishop/square.hpp>
#include <cstdint>

// bitbishop/board.hpp uses Zobrist features and the current Zobrist implementation also uses Board & BoardState.
// Forward declaration below and include inside bitbishop/zobrist.cpp solves this circular dependency.
struct BoardState;
class Board;

namespace Zobrist {

using Key = uint64_t;

/**
 * Neutral hash value to start a fresh Zobrist hash.
 */
CX_INLINE Zobrist::Key NULL_HASH = 0ULL;

/**
 * @brief Zobrist random key tables used for chess position hashing.
 *
 * Zobrist hashing assigns a random 64-bit value to each independent
 * component of a chess position. A position hash is produced by XOR-
 * combining the values corresponding to the current board state.
 *
 * This structure stores the pre-generated random keys for all position
 * features required by the hash:
 *
 *  - Piece placement:
 *      One key for every (piece type, square) combination.
 *
 *  - Castling rights:
 *      One key for each of the 16 possible castling-rights states.
 *
 *  - En-passant file:
 *      One key for each file (a–h) that may contain an en-passant target.
 *
 *  - Side to move:
 *      A single key toggled when the side to move is black.
 *
 * These values are XOR-combined to compute a unique hash (with very high
 * probability) for a given chess position.
 */
struct Tables {
  std::array<std::array<Key, Const::BOARD_SIZE>, Piece::DISTINCT_PIECES_COUNT> pieces{};
  std::array<Key, Const::CASTLING_RIGHTS_STATES_COUNT> castling{};
  std::array<Key, Const::EN_PASSANT_STATES_COUNT> en_passant{};
  Key side{};
};

/**
 * @brief Generates deterministic Zobrist key tables.
 *
 * The tables are filled using the splitmix64 pseudo-random number generator.
 * A fixed seed is used so the same random values are produced across
 * program runs and builds, ensuring deterministic hashes.
 *
 * Each call to splitmix64 mutates the seed and returns a new random 64-bit value.
 *
 * @return Fully initialized Zobrist key tables.
 */
CX_FN Tables generate() {
  using namespace Random;
  using namespace Const;

  Tables zobrist_tbl{};
  uint64_t seed = 0x994c3ac5e71fafcbULL;  // NOLINT(readability-magic-numbers)

  for (int p_ind = 0; p_ind < Piece::DISTINCT_PIECES_COUNT; ++p_ind) {
    for (int sq = 0; sq < BOARD_SIZE; ++sq) {
      zobrist_tbl.pieces[p_ind][sq] = splitmix64(seed);
    }
  }

  for (int i = 0; i < CASTLING_RIGHTS_STATES_COUNT; ++i) {
    zobrist_tbl.castling[i] = splitmix64(seed);
  }

  for (int f = 0; f < EN_PASSANT_STATES_COUNT; ++f) {
    zobrist_tbl.en_passant[f] = splitmix64(seed);
  }

  zobrist_tbl.side = splitmix64(seed);

  return zobrist_tbl;
}

/**
 * @brief Global Zobrist tables instance.
 *
 * The tables are generated once and reused throughout the engine.
 * They are typically computed at compile time when supported by the compiler.
 *
 * All hashing operations reference this shared table.
 */
CX_INLINE Tables tables = generate();

/**
 * @brief Converts a Piece to its Zobrist table index.
 *
 * Zobrist piece keys are organized as:
 *
 *  White pieces:
 *    0  Pawn
 *    1  Knight
 *    2  Bishop
 *    3  Rook
 *    4  Queen
 *    5  King
 *
 *  Black pieces:
 *    6  Pawn
 *    7  Knight
 *    8  Bishop
 *    9  Rook
 *   10  Queen
 *   11  King
 *
 * This mapping ensures a unique index for every piece type and color.
 *
 * @param piece Piece to convert.
 * @return Index into Tables::pieces.
 */
CX_FN int piece_index(Piece piece) {
  const int base = (piece.color() == Color::WHITE) ? 0 : 6;  // NOLINT: (readability-magic-numbers)
  const int piece_type_index = static_cast<int>(piece.type());
  return base + piece_type_index;
}

/**
 * @brief XORs the Zobrist key corresponding to a piece on a square.
 *
 * This function toggles the hash contribution of a specific piece
 * occupying a given square.
 *
 * Because Zobrist hashing uses XOR, applying the same mutation twice
 * cancels the effect. This allows efficient incremental updates when
 * pieces are moved, captured, or restored.
 *
 * Typical usage:
 *
 *  - Removing a piece:
 *      mutate_piece(square, piece, hash)
 *
 *  - Placing the same piece again:
 *      mutate_piece(square, piece, hash)
 *
 * @param square Square occupied by the piece.
 * @param piece Piece whose contribution should be toggled.
 * @param key Zobrist hash key to mutate.
 */
void mutate_piece(Square square, Piece piece, Zobrist::Key& key);

/**
 * @brief Toggles the Zobrist key representing the side to move.
 *
 * A single random key distinguishes positions where the board layout
 * is identical but the active player differs.
 *
 * By convention, the key is XORed when the side to move becomes black.
 * Calling this function again reverts the change.
 *
 * @param key Zobrist hash key to mutate.
 */
void mutate_side_to_move(Zobrist::Key& key);

/**
 * @brief Applies the Zobrist hash delta between two board states.
 *
 * This function updates the hash by XOR-removing the contributions
 * of the previous state and XOR-adding the contributions of the next state.
 *
 * The following components are considered:
 *
 *  - Side to move
 *  - Castling rights
 *  - En-passant file
 *
 * Only fields that differ between the two states are mutated.
 * This allows efficient incremental hashing when applying or undoing moves.
 *
 * @param prev Previous board state.
 * @param next Next board state.
 * @param key Zobrist hash key to mutate.
 */
void mutate_board_state_diff(const BoardState& prev, const BoardState& next, Zobrist::Key& key);

/**
 * @brief XORs the Zobrist key encoding the board's castling rights.
 *
 * Castling rights are encoded into a 4-bit integer:
 *
 *   bit 0 → white kingside
 *   bit 1 → white queenside
 *   bit 2 → black kingside
 *   bit 3 → black queenside
 *
 * The resulting value ranges from 0–15 and indexes Tables::castling.
 *
 * Example encodings:
 *
 *   0000 → no castling rights
 *   0001 → white kingside
 *   0011 → white kingside + queenside
 *   1111 → all castling rights
 *
 * The corresponding random key is XORed into the hash.
 *
 * @param state Board state containing castling rights.
 * @param key Zobrist hash key to mutate.
 */
void mutate_castling_rights(const BoardState& state, Zobrist::Key& key);

/**
 * @brief XORs the Zobrist key representing the en-passant file.
 *
 * When an en-passant capture is possible, only the file (a–h) of the
 * en-passant square contributes to the hash. The rank is implied by
 * the side that performed the double pawn push.
 *
 * The file index (0–7) selects one of the eight keys stored in
 * Tables::en_passant.
 *
 * Applying the same mutation twice cancels the contribution.
 *
 * @param epsq En-passant square.
 * @param key Zobrist hash key to mutate.
 */
void mutate_en_passant_square(Square epsq, Zobrist::Key& key);

/**
 * @brief Computes the Zobrist hash of an entire board position.
 *
 * This function builds the hash from scratch by XOR-combining
 * the random keys associated with all components of the position:
 *
 *  1. Piece placement
 *     Every piece on the board contributes its (piece, square) key.
 *
 *  2. Side to move
 *     A dedicated key is toggled when the side to move is black.
 *
 *  3. Castling rights
 *     The current castling-rights mask selects one of 16 keys.
 *
 *  4. En-passant file
 *     If an en-passant square exists, its file selects one of 8 keys.
 *
 * The resulting 64-bit value uniquely identifies the position with
 * extremely high probability.
 *
 * This function is typically used:
 *
 *  - when initializing a position
 *  - when verifying incremental hash correctness
 *
 * During normal search, engines update the hash incrementally using
 * the mutation functions instead of recomputing it entirely.
 *
 * @param board Board position to hash.
 * @return Zobrist hash key representing the position.
 */
Zobrist::Key compute_hash(const Board& board);

}  // namespace Zobrist
