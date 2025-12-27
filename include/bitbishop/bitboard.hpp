#pragma once
#include <bit>
#include <bitbishop/constants.hpp>
#include <bitbishop/square.hpp>
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

  /** @brief Constructs a bitboard from another bitboard by copy. */
  constexpr Bitboard(const Bitboard& bitboard) : m_bb(bitboard.value()) {}

  /** @brief Returns the raw 64-bit value of the bitboard. */
  [[nodiscard]] constexpr uint64_t value() const { return m_bb; }

  /** @brief Returns a Bitboard instance with all bits set to zero. */
  static constexpr Bitboard Zeros() noexcept { return {0ULL}; }

  /** @brief Returns a Bitbaord instance with all bits set to one. */
  static constexpr Bitboard Ones() noexcept { return {~0ULL}; }

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
  constexpr void set(Square square) { m_bb |= (1ULL << square.value()); }
  constexpr void set(Square::Value square) { m_bb |= (1ULL << square); }
  constexpr void set(std::uint8_t square) { m_bb |= (1ULL << square); }

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
  constexpr void clear(Square square) { m_bb &= ~(1ULL << square.value()); }
  constexpr void clear(Square::Value square) { m_bb &= ~(1ULL << square); }
  constexpr void clear(std::uint8_t square) { m_bb &= ~(1ULL << square); }

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
  [[nodiscard]] constexpr bool test(Square square) const { return ((m_bb >> square.value()) & 1ULL) != 0ULL; }
  [[nodiscard]] constexpr bool test(Square::Value square) const { return ((m_bb >> square) & 1ULL) != 0ULL; }
  [[nodiscard]] constexpr bool test(std::uint8_t square) const { return ((m_bb >> square) & 1ULL) != 0ULL; }

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
    using namespace Const;

    for (int rank = RANK_8_IND; rank >= RANK_1_IND; --rank) {
      for (int file = FILE_A_IND; file <= FILE_H_IND; ++file) {
        const Square square(file, rank);
        std::cout << (test(square) ? "1 " : ". ");
      }
      std::cout << "\n";
    }
  }

  constexpr bool operator==(const Bitboard& other) const { return m_bb == other.m_bb; }
  constexpr bool operator!=(const Bitboard& other) const { return m_bb != other.m_bb; }
  constexpr Bitboard operator|(const Bitboard& other) const { return m_bb | other.m_bb; }
  constexpr Bitboard operator&(const Bitboard& other) const { return m_bb & other.m_bb; }
  constexpr Bitboard operator~() const { return ~m_bb; }
  constexpr Bitboard operator<<(int shift) const { return m_bb << shift; }
  constexpr Bitboard operator>>(int shift) const { return m_bb >> shift; }
  constexpr Bitboard& operator|=(const Bitboard& other) {
    m_bb |= other.m_bb;
    return *this;
  }
  constexpr Bitboard& operator&=(const Bitboard& other) {
    m_bb &= other.m_bb;
    return *this;
  }
  constexpr operator bool() const noexcept { return m_bb != 0ULL; }

  /**
   * @brief Counts the number of set bits in the bitboard.
   *
   * @return The number of bits set to 1.
   *
   * @note Uses std::popcount (C++20 and later).
   */
  [[nodiscard]] constexpr int count() const noexcept { return std::popcount(m_bb); }

  /**
   * @brief Tells if any bit in the bitboard is set to one.
   *
   * @return True if at least one bit is set to one in the bitboard.
   *
   * @note Equivalent to `cout() > 0` and `bool()`
   */
  [[nodiscard]] constexpr bool any() const noexcept { return m_bb != 0ULL; }

  /**
   * @brief Tells if the bitboard is empty / has no bit set.
   *
   * @return True if no bit is set on that bitboard.
   *
   * @note Equivalent to '!any()' or 'count() == 0'
   */
  [[nodiscard]] constexpr bool empty() const noexcept { return m_bb == 0ULL; }

  /**
   * @brief Removes and returns the least significant set bit (LSB) from the bitboard.
   *
   * This method identifies the lowest-index bit currently set to 1 (the least significant bit),
   * converts it to a Square object, clears that bit from the bitboard, and returns the square.
   *
   * ### Example
   *
   * Suppose the bitboard has bits set for A1 (index 0) and C5 (index 34):
   *
   * ```
   * m_bb (binary) = 000...01000000000000000000000000000001
   * pop_lsb() → returns Square(0)  // A1
   * m_bb (after) = 000...01000000000000000000000000000000  // only C5 remains
   * ```
   *
   * ### Implementation Details
   *
   * - `std::countr_zero(m_bb)` efficiently finds the index of the lowest set bit.
   * - The expression `m_bb &= (m_bb - 1)` clears that bit in constant time.
   *
   * The reason `m_bb &= (m_bb - 1)` works is due to binary subtraction:
   * when subtracting 1 from a binary number, the rightmost set bit turns to 0,
   * and all bits to its right become 1. Bits to the left of the LSB stay unchanged:
   *
   * m_bb                 = 0b10110000
   * m_bb - 1             = 0b10101111
   * m_bb &= (m_bb - 1)   = 0b10100000
   *
   * ### Return Value
   *
   * - `std::optional<Square>` containing the lowest set square if one exists.
   * - `std::nullopt` if the bitboard is empty.
   *
   * @note This method **modifies** the bitboard by clearing the bit that it returns.
   *       Use `lsb()` if you want to inspect the least significant bit *without* modification.
   */
  constexpr std::optional<Square> pop_lsb() noexcept {
    if (!*this) {
      return std::nullopt;
    }
    int index = std::countr_zero(m_bb);
    m_bb &= (m_bb - 1);
    const Square square(index);
    return square;
  }

  /**
   * @brief Removes and returns the most significant set bit (MSB) from the bitboard.
   *
   * This method identifies the highest-index bit currently set to 1 (the most significant bit),
   * converts it to a Square object, clears that bit from the bitboard, and returns the square.
   *
   * ### Example
   *
   * Suppose the bitboard has bits set for A1 (index 0) and H8 (index 63):
   *
   * ```
   * m_bb (binary) = 1000000000000000000000000000000000000000000000000000000000000001
   * pop_msb() → returns Square(63)  // H8
   * m_bb (after) = 0000000000000000000000000000000000000000000000000000000000000001  // only A1 remains
   * ```
   *
   * ### Implementation Details
   *
   * - `std::countl_zero(m_bb)` efficiently counts the number of leading zero bits.
   * - The MSB index is calculated as `63 - std::countl_zero(m_bb)`.
   * - The expression `m_bb ^= (1ULL << index)` uses XOR to toggle (clear) the MSB.
   *
   * The XOR operation works because XORing a bit with 1 flips it:
   *
   * m_bb        = 0b10110000
   * 1ULL << 7   = 0b10000000
   * m_bb ^= ... = 0b00110000  // MSB cleared
   *
   * ### Return Value
   *
   * - `std::optional<Square>` containing the highest set square if one exists.
   * - `std::nullopt` if the bitboard is empty.
   *
   * @note This method **modifies** the bitboard by clearing the bit that it returns.
   *       Use `msb()` if you want to inspect the most significant bit *without* modification.
   */
  constexpr std::optional<Square> pop_msb() noexcept {
    using namespace Const;

    if (!*this) {
      return std::nullopt;
    }
    int index = BOARD_SIZE - 1 - std::countl_zero(m_bb);
    m_bb ^= (1ULL << index);
    const Square square(index);
    return square;
  }

  /**
   * @brief Returns the least significant set bit (LSB) without modifying the bitboard.
   *
   * This function identifies the lowest-index bit currently set to 1 and returns
   * it as a Square, without modifying the underlying bitboard.
   *
   * ### Example
   *
   * If bits are set at A1 (index 0) and H8 (index 63):
   *
   * ```
   * m_bb (binary) = 1000000000000000000000000000000000000000000000000000000000000001
   * lsb() → returns Square(0)  // A1
   * m_bb (unchanged)
   * ```
   *
   * ### Implementation Details
   *
   * - `std::countr_zero(m_bb)` counts the number of trailing zero bits.
   * - The LSB index is the result of `std::countr_zero(m_bb)`.
   *
   * For example, if the LSB is at position 0:
   * - `countr_zero` returns 0 (no trailing zeros)
   * - This directly gives the correct index
   *
   * ### Return Value
   *
   * - `std::optional<Square>` containing the lowest set square if one exists.
   * - `std::nullopt` if the bitboard is empty.
   *
   * @note This method is const and does not modify the underlying bitboard.
   */
  [[nodiscard]] constexpr std::optional<Square> lsb() const noexcept {
    if (!*this) {
      return std::nullopt;
    }
    int index = std::countr_zero(m_bb);
    const Square square(index);
    return square;
  }

  /**
   * @brief Returns the most significant set bit (MSB) without modifying the bitboard.
   *
   * This function identifies the highest-index bit currently set to 1 and returns
   * it as a Square, without modifying the underlying bitboard.
   *
   * ### Example
   *
   * If bits are set at A1 (index 0) and H8 (index 63):
   *
   * ```
   * m_bb (binary) = 1000000000000000000000000000000000000000000000000000000000000001
   * msb() → returns Square(63)  // H8
   * m_bb (unchanged)
   * ```
   *
   * ### Implementation Details
   *
   * - `std::countl_zero(m_bb)` counts the number of leading zero bits.
   * - The MSB index is calculated as `63 - std::countl_zero(m_bb)`.
   *
   * For example, if the MSB is at position 63:
   * - `countl_zero` returns 0 (no leading zeros)
   * - `63 - 0 = 63` gives the correct index
   *
   * ### Return Value
   *
   * - `std::optional<Square>` containing the highest set square if one exists.
   * - `std::nullopt` if the bitboard is empty.
   *
   * @note This method is const and does not modify the underlying bitboard.
   */
  [[nodiscard]] constexpr std::optional<Square> msb() const noexcept {
    using namespace Const;

    if (!*this) {
      return std::nullopt;
    }
    int index = BOARD_SIZE - 1 - std::countl_zero(m_bb);
    const Square square(index);
    return square;
  }

  /**
   * @brief Iterator for traversing all set bits (squares) in a Bitboard.
   *
   * This iterator provides a clean, efficient way to loop through all squares
   * where the bitboard has a 1 bit set — from least significant bit (A1) upward.
   *
   * Example:
   * @code
   * Bitboard bb;
   * bb.set(Square::A1);
   * bb.set(Square::C3);
   * bb.set(Square::H8);
   * for (Square sq : bb) {
   *   std::cout << sq << "\n"; // Iterates A1, C3, H8 (in LSB order)
   * }
   * @endcode
   *
   * Internally, iteration uses `std::countr_zero()` to find the index of the
   * least significant set bit (LSB), and the idiom `bits &= (bits - 1)` to
   * clear it efficiently.
   *
   * The iterator is designed to be lightweight and constexpr-compatible.
   */
  class Iterator {
    uint64_t bits;  ///< Internal copy of the bitboard bits being iterated.

   public:
    /**
     * @brief Constructs an iterator for a given bit pattern.
     * @param bitboard The 64-bit bitboard value to iterate over.
     */
    constexpr Iterator(uint64_t bitboard) noexcept : bits(bitboard) {}

    /**
     * @brief Inequality comparison for iterators.
     * @param other Another iterator to compare to.
     * @return true if this iterator does not equal `other`.
     */
    constexpr bool operator!=(const Iterator& other) const noexcept { return bits != other.bits; }

    /**
     * @brief Equality comparison for iterators.
     * @param other Another iterator to compare to.
     * @return true if this iterator equals `other`.
     */
    constexpr bool operator==(const Iterator& other) const noexcept { return bits == other.bits; }

    /**
     * @brief Dereferences the iterator to return the current square.
     * @return A `Square` corresponding to the current least significant set bit.
     */
    constexpr Square operator*() const noexcept { return {std::countr_zero(bits), std::in_place}; }

    /**
     * @brief Advances the iterator to the next set bit.
     *
     * Uses the fast bit trick `bits &= (bits - 1)` to clear the lowest set bit
     * and move to the next one.
     *
     * @return Reference to the incremented iterator.
     */
    constexpr Iterator& operator++() noexcept {
      bits &= (bits - 1);
      return *this;
    }
  };

  /**
   * @brief Returns an iterator pointing to the first set bit (if any).
   * @return Iterator positioned at the first set bit.
   */
  [[nodiscard]] constexpr Iterator begin() const noexcept { return {m_bb}; }

  /**
   * @brief Returns an iterator representing the end (no bits set).
   * @return Iterator with no bits remaining.
   */
  [[nodiscard]] static constexpr Iterator end() noexcept { return {0ULL}; }
};
