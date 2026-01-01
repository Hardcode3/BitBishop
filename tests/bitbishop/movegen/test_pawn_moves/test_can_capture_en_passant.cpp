#include <gtest/gtest.h>

#include <bitbishop/movegen/pawn_moves.hpp>

/**
 * @brief Test case structure for en passant validation.
 */
struct EnPassantCase {
  Square from;
  Square en_passant;
  Color color;
  bool expected;
};

/**
 * @test Verifies can_capture_en_passant returns true for all valid White en passant captures.
 *
 * White pawns on rank 5 can capture en passant to rank 6 if the target square is
 * diagonally adjacent (one file away).
 */
TEST(PawnMoveGeneratorTest, WhiteCanCaptureEnPassant) {
  std::vector<EnPassantCase> cases{
      {Squares::A5, Squares::B6, Color::WHITE, true}, {Squares::B5, Squares::A6, Color::WHITE, true},
      {Squares::B5, Squares::C6, Color::WHITE, true}, {Squares::C5, Squares::B6, Color::WHITE, true},
      {Squares::C5, Squares::D6, Color::WHITE, true}, {Squares::D5, Squares::C6, Color::WHITE, true},
      {Squares::D5, Squares::E6, Color::WHITE, true}, {Squares::E5, Squares::D6, Color::WHITE, true},
      {Squares::E5, Squares::F6, Color::WHITE, true}, {Squares::F5, Squares::E6, Color::WHITE, true},
      {Squares::F5, Squares::G6, Color::WHITE, true}, {Squares::G5, Squares::F6, Color::WHITE, true},
      {Squares::G5, Squares::H6, Color::WHITE, true}, {Squares::H5, Squares::G6, Color::WHITE, true},
  };

  for (const EnPassantCase& c : cases) {
    ASSERT_EQ(can_capture_en_passant(c.from, c.en_passant, c.color), c.expected)
        << "from=" << c.from.to_string() << " en_passant=" << c.en_passant.to_string() << " color=WHITE";
  }
}

/**
 * @test Verifies can_capture_en_passant returns true for all valid Black en passant captures.
 *
 * Black pawns on rank 4 can capture en passant to rank 3 if the target square is
 * diagonally adjacent (one file away).
 */
TEST(PawnMoveGeneratorTest, BlackCanCaptureEnPassant) {
  std::vector<EnPassantCase> cases{
      {Squares::A4, Squares::B3, Color::BLACK, true}, {Squares::B4, Squares::A3, Color::BLACK, true},
      {Squares::B4, Squares::C3, Color::BLACK, true}, {Squares::C4, Squares::B3, Color::BLACK, true},
      {Squares::C4, Squares::D3, Color::BLACK, true}, {Squares::D4, Squares::C3, Color::BLACK, true},
      {Squares::D4, Squares::E3, Color::BLACK, true}, {Squares::E4, Squares::D3, Color::BLACK, true},
      {Squares::E4, Squares::F3, Color::BLACK, true}, {Squares::F4, Squares::E3, Color::BLACK, true},
      {Squares::F4, Squares::G3, Color::BLACK, true}, {Squares::G4, Squares::F3, Color::BLACK, true},
      {Squares::G4, Squares::H3, Color::BLACK, true}, {Squares::H4, Squares::G3, Color::BLACK, true},
  };

  for (const EnPassantCase& c : cases) {
    ASSERT_EQ(can_capture_en_passant(c.from, c.en_passant, c.color), c.expected)
        << "from=" << c.from.to_string() << " en_passant=" << c.en_passant.to_string() << " color=BLACK";
  }
}

/**
 * @test Verifies can_capture_en_passant returns false for invalid Black en passant scenarios.
 *
 * Tests edge cases: wrong ranks, same file (non-diagonal), too far away,
 * wrong direction, and board edges.
 */
TEST(PawnMoveGeneratorTest, BlackCannotCaptureEnPassant_InvalidCombinations) {
  std::vector<EnPassantCase> cases{
      // Wrong rank (must be rank 4 for Black)
      {Squares::E5, Squares::D4, Color::BLACK, false},
      {Squares::E3, Squares::D2, Color::BLACK, false},
      {Squares::E2, Squares::F1, Color::BLACK, false},
      {Squares::D6, Squares::C5, Color::BLACK, false},

      // Same file (not diagonal)
      {Squares::E4, Squares::E3, Color::BLACK, false},
      {Squares::C4, Squares::C3, Color::BLACK, false},

      // Too far away (more than one file)
      {Squares::E4, Squares::C3, Color::BLACK, false},
      {Squares::E4, Squares::G3, Color::BLACK, false},

      // Board edges (no adjacent file)
      {Squares::A4, Squares::A3, Color::BLACK, false},
      {Squares::H4, Squares::H3, Color::BLACK, false},
      {Squares::A4, Squares::H3, Color::BLACK, false},

      // Wrong direction (backward move)
      {Squares::E4, Squares::D5, Color::BLACK, false},
      {Squares::E4, Squares::F5, Color::BLACK, false},

      // Wrong color's rank (White positions)
      {Squares::E5, Squares::F6, Color::BLACK, false},
      {Squares::C5, Squares::B6, Color::BLACK, false},
  };

  for (const EnPassantCase& c : cases) {
    ASSERT_EQ(can_capture_en_passant(c.from, c.en_passant, c.color), c.expected)
        << "from=" << c.from.to_string() << " en_passant=" << c.en_passant.to_string() << " color=BLACK";
  }
}
