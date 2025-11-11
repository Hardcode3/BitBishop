#include <gtest/gtest.h>

#include <chess_engine/moves/pawn_move_gen.hpp>

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
      {Square(Square::A5), Square(Square::B6), Color::WHITE, true},
      {Square(Square::B5), Square(Square::A6), Color::WHITE, true},
      {Square(Square::B5), Square(Square::C6), Color::WHITE, true},
      {Square(Square::C5), Square(Square::B6), Color::WHITE, true},
      {Square(Square::C5), Square(Square::D6), Color::WHITE, true},
      {Square(Square::D5), Square(Square::C6), Color::WHITE, true},
      {Square(Square::D5), Square(Square::E6), Color::WHITE, true},
      {Square(Square::E5), Square(Square::D6), Color::WHITE, true},
      {Square(Square::E5), Square(Square::F6), Color::WHITE, true},
      {Square(Square::F5), Square(Square::E6), Color::WHITE, true},
      {Square(Square::F5), Square(Square::G6), Color::WHITE, true},
      {Square(Square::G5), Square(Square::F6), Color::WHITE, true},
      {Square(Square::G5), Square(Square::H6), Color::WHITE, true},
      {Square(Square::H5), Square(Square::G6), Color::WHITE, true},
  };

  for (const EnPassantCase& c : cases) {
    ASSERT_EQ(PawnMoveGenerator::can_capture_en_passant(c.from, c.en_passant, c.color), c.expected)
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
      {Square(Square::A4), Square(Square::B3), Color::BLACK, true},
      {Square(Square::B4), Square(Square::A3), Color::BLACK, true},
      {Square(Square::B4), Square(Square::C3), Color::BLACK, true},
      {Square(Square::C4), Square(Square::B3), Color::BLACK, true},
      {Square(Square::C4), Square(Square::D3), Color::BLACK, true},
      {Square(Square::D4), Square(Square::C3), Color::BLACK, true},
      {Square(Square::D4), Square(Square::E3), Color::BLACK, true},
      {Square(Square::E4), Square(Square::D3), Color::BLACK, true},
      {Square(Square::E4), Square(Square::F3), Color::BLACK, true},
      {Square(Square::F4), Square(Square::E3), Color::BLACK, true},
      {Square(Square::F4), Square(Square::G3), Color::BLACK, true},
      {Square(Square::G4), Square(Square::F3), Color::BLACK, true},
      {Square(Square::G4), Square(Square::H3), Color::BLACK, true},
      {Square(Square::H4), Square(Square::G3), Color::BLACK, true},
  };

  for (const EnPassantCase& c : cases) {
    ASSERT_EQ(PawnMoveGenerator::can_capture_en_passant(c.from, c.en_passant, c.color), c.expected)
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
      {Square(Square::E5), Square(Square::D4), Color::BLACK, false},
      {Square(Square::E3), Square(Square::D2), Color::BLACK, false},
      {Square(Square::E2), Square(Square::F1), Color::BLACK, false},
      {Square(Square::D6), Square(Square::C5), Color::BLACK, false},

      // Same file (not diagonal)
      {Square(Square::E4), Square(Square::E3), Color::BLACK, false},
      {Square(Square::C4), Square(Square::C3), Color::BLACK, false},

      // Too far away (more than one file)
      {Square(Square::E4), Square(Square::C3), Color::BLACK, false},
      {Square(Square::E4), Square(Square::G3), Color::BLACK, false},

      // Board edges (no adjacent file)
      {Square(Square::A4), Square(Square::A3), Color::BLACK, false},
      {Square(Square::H4), Square(Square::H3), Color::BLACK, false},
      {Square(Square::A4), Square(Square::H3), Color::BLACK, false},

      // Wrong direction (backward move)
      {Square(Square::E4), Square(Square::D5), Color::BLACK, false},
      {Square(Square::E4), Square(Square::F5), Color::BLACK, false},

      // Wrong color's rank (White positions)
      {Square(Square::E5), Square(Square::F6), Color::BLACK, false},
      {Square(Square::C5), Square(Square::B6), Color::BLACK, false},
  };

  for (const EnPassantCase& c : cases) {
    ASSERT_EQ(PawnMoveGenerator::can_capture_en_passant(c.from, c.en_passant, c.color), c.expected)
        << "from=" << c.from.to_string() << " en_passant=" << c.en_passant.to_string() << " color=BLACK";
  }
}
