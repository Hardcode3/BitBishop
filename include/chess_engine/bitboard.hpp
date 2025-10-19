#pragma once
#include <bit>
#include <chess_engine/square.hpp>
#include <cstdint>
#include <iostream>
#include <optional>

/**
 * @class Bitboard
 * @brief Wrapper around a 64-bit unsigned integer to represent a bitboard.
 *
 * - Bit = 0 -> empty square
 * - Bit = 1 -> occupied square
 *
 *   Bit indices (uint64_t bits):        Chessboard squares:
 *
 *   63 62 61 60 59 58 57 56             H8 G8 F8 E8 D8 C8 B8 A8
 *   55 54 53 52 51 50 49 48             H7 G7 F7 E7 D7 C7 B7 A7
 *   47 46 45 44 43 42 41 40             H6 G6 F6 E6 D6 C6 B6 A6
 *   39 38 37 36 35 34 33 32             H5 G5 F5 E5 D5 C5 B5 A5
 *   31 30 29 28 27 26 25 24             H4 G4 F4 E4 D4 C4 B4 A4
 *   23 22 21 20 19 18 17 16             H3 G3 F3 E3 D3 C3 B3 A3
 *   15 14 13 12 11 10  9  8             H2 G2 F2 E2 D2 C2 B2 A2
 *    7  6  5  4  3  2  1  0             H1 G1 F1 E1 D1 C1 B1 A1
 *
 * Notes:
 * - Bit 0 = A1 (least significant bit).
 * - Bit 63 = H8 (most significant bit).
 * - Each rank = 8 consecutive bits.
 *
 * This layout makes pawn moves intuitive:
 * - White pawn push = shift north (+8 bits)
 * - Black pawn push = shift south (−8 bits)
 */
class Bitboard {
 private:
  uint64_t m_bb;  ///< Raw 64-bit bitboard value

 public:
  /** @brief Constructs an empty bitboard (all bits = 0).
   * Zero initialization of uint64_t m_bb granted by C++ standard
   */
  constexpr Bitboard() : m_bb() {}

  /** @brief Constructs a bitboard from a raw 64-bit value. */
  constexpr Bitboard(uint64_t value) : m_bb(value) {}

  /** @brief Returns the raw 64-bit value of the bitboard. */
  constexpr uint64_t value() { return m_bb; }

  /**
   * @brief Sets a bit (places a piece) on a given square.
   *
   * Internals:
   * - (1ULL << sq) shifts the value `1` left by `sq` positions.
   *   Example: if sq = 0 → mask = 000...0001 (A1)
   *            if sq = 4 → mask = 000...10000 (E1)
   * - 'bitwise or' |= sets that bit to `1` while leaving others unchanged.
   *
   * Example: set(E2) marks E2 as occupied.
   *
   * Start bitboard: 00000000 00000000 ... 00000000 00000000
   * 1ULL = 00000000 00000000 ... 00000000 00000001
   * 1ULL << E2 = 00000000 00000000 ... 00010000 00000000
   * m_bb |= (1ULL << sq) = 00000000 00000000 ... 00010000 00000000
   */
  constexpr void set(Square sq) { m_bb |= (1ULL << sq.value()); }
  constexpr void set(Square::Value sq) { m_bb |= (1ULL << sq); }

  /**
   * @brief Clears a bit (removes a piece) on a given square.
   *
   * Internals:
   * - (1ULL << sq) creates a mask with a `1` only at position `sq`.
   * - ~(1ULL << sq) flips bits: all `1`s except position `sq` = `0`.
   * - 'bitwise and' &= with this mask forces that square to `0` (cleared).
   *
   * Example: clear(E2) empties square E2.
   *
   * Start bitboard: 00000000 00000000 ... 00010000 00000000
   * 1ULL = 00000000 00000000 ... 00000000 00000001
   * 1ULL << E2 = 00000000 00000000 ... 00010000 00000000
   * ~(1ULL << E2) = 11111111 11111111 ... 11101111 11111111
   * m_bb &= ~(1ULL << sq) = 00000000 00000000 ... 00000000 00000000
   */
  constexpr void clear(Square sq) { m_bb &= ~(1ULL << sq.value()); }
  constexpr void clear(Square::Value sq) { m_bb &= ~(1ULL << sq); }

  /**
   * @brief Checks if a square is occupied.
   *
   * Internals:
   * - (m_bb >> sq) shifts the bitboard right so that the target bit
   *   moves into the least significant position.
   * - & 1ULL masks out all other bits except the least significant one.
   *
   * Example: test(E2) → returns true if E2 bit = 1.
   *
   * Start bitboard: 00000000 00000000 ... 00010000 00000000
   * 1ULL = 00000000 00000000 ... 0000000 00000001
   * m_bb >> E2 = 00000000 00000000 ... 0000000 00000001
   * (m_bb >> E2) & 1ULL = true
   *
   */
  constexpr bool test(Square sq) const { return (m_bb >> sq.value()) & 1ULL; }
  constexpr bool test(Square::Value sq) const { return (m_bb >> sq) & 1ULL; }

  /** @brief Clears the whole bitboard (all bits = 0). */
  constexpr void reset() { m_bb = 0ULL; }

  /**
   * @brief Prints the bitboard as an 8×8 grid.
   *
   * - "1" means the square is occupied
   * - "." means the square is empty
   *
   * The output starts from rank 8 down to rank 1.
   */
  void print() const {
    for (int rank = 7; rank >= 0; --rank) {
      for (int file = 0; file < 8; ++file) {
        const Square sq(file, rank);
        std::cout << (test(sq) ? "1 " : ". ");
      }
      std::cout << "\n";
    }
  }

  constexpr bool operator==(const Bitboard& other) const { return m_bb == other.m_bb; }
  constexpr bool operator!=(const Bitboard& other) const { return m_bb != other.m_bb; }
  constexpr Bitboard operator|(const Bitboard& other) const { return m_bb | other.m_bb; }
  constexpr Bitboard operator&(const Bitboard& other) const { return m_bb & other.m_bb; }
  constexpr Bitboard operator~() const { return ~m_bb; }
  constexpr Bitboard operator<<(int n) const { return m_bb << n; }
  constexpr Bitboard operator>>(int n) const { return m_bb >> n; }
  constexpr Bitboard& operator|=(const Bitboard& other) {
    m_bb |= other.m_bb;
    return *this;
  }
  constexpr Bitboard& operator&=(const Bitboard& other) {
    m_bb &= other.m_bb;
    return *this;
  }

  // NOT TESTED
  constexpr operator bool() const noexcept { return m_bb != 0ULL; }

  // NOT TESTED
  constexpr int count() const noexcept { return std::popcount(m_bb); }

  // NOT TESTED
  constexpr std::optional<Square> pop_lsb() noexcept {
    if (!*this) return std::nullopt;
    int index = std::countr_zero(m_bb);
    const Square sq(index);
    clear(sq);
    return sq;
  }

  // NOT TESTED
  constexpr std::optional<Square> lsb() const noexcept {
    if (!*this) return std::nullopt;
    int index = std::countr_zero(m_bb);
    const Square sq(index);
    return sq;
  }

  // NOT TESTED
  class Iterator {
    uint64_t bits;

   public:
    constexpr Iterator(uint64_t b) noexcept : bits(b) {}
    constexpr bool operator!=(const Iterator& other) const noexcept { return bits != other.bits; }
    constexpr Square operator*() const noexcept { return Square(std::countr_zero(bits)); }
    constexpr Iterator& operator++() noexcept {
      bits &= (bits - 1);
      return *this;
    }
  };
  constexpr Iterator begin() const noexcept { return Iterator(m_bb); }
  constexpr Iterator end() const noexcept { return Iterator(0ULL); }
};
