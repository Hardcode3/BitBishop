#include <gtest/gtest.h>

#include <chess_engine/movegen/pawn_move_gen.hpp>

TEST(PawnMoveGeneratorTest, IsStartingRankFromStartingRankForWhite) {
  for (int sq = Square::A2; sq <= Square::H2; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsStartingRankFromStartingRankForBlack) {
  for (int sq = Square::A7; sq <= Square::H7; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsStartingRankFromNotSartingRankForWhite) {
  // first rank is not a white pawn starting rank
  for (int sq = Square::A1; sq <= Square::H1; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
  // ranks 3 to 8 are not white pawn starting ranks
  for (int sq = Square::A3; sq <= Square::H8; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsStartingRankFromNotSartingRankForBlack) {
  // ranks 1 to 5 are not black pawn starting ranks
  for (int sq = Square::A1; sq <= Square::H6; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
  // rank 8 is not a black pawn starting rank
  for (int sq = Square::A8; sq <= Square::H8; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_starting_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsPromotionRankFromPromotionRankForWhite) {
  for (int sq = Square::A8; sq <= Square::H8; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsPromotionRankFromPromotionRankForBlack) {
  for (int sq = Square::A1; sq <= Square::H1; sq++) {
    ASSERT_TRUE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsPromotionRankNotFromPromotionRankForWhite) {
  for (int sq = Square::A1; sq <= Square::H7; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::WHITE))
        << "for square " << Square(sq).to_string();
  }
}

TEST(PawnMoveGeneratorTest, IsPromotionRankNotFromPromotionRankForBlack) {
  for (int sq = Square::A2; sq <= Square::H8; sq++) {
    ASSERT_FALSE(PawnMoveGenerator::is_promotion_rank(Square(sq), Color::BLACK))
        << "for square " << Square(sq).to_string();
  }
}

struct EnPassantCase {
  Square from;
  Square en_passant;
  Color color;
  bool expected;
};

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
        << "from=" << c.from.to_string() << " en_passant=" << c.en_passant.to_string() << " color=BLACK";
  }
}

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

TEST(PawnMoveGeneratorTest, BlackCannotCaptureEnPassant_InvalidCombinations) {
  std::vector<EnPassantCase> cases{
      // Wrong rank (should only be from rank 4 for black)
      {Square(Square::E5), Square(Square::D4), Color::BLACK, false},
      {Square(Square::E3), Square(Square::D2), Color::BLACK, false},
      {Square(Square::E2), Square(Square::F1), Color::BLACK, false},
      {Square(Square::D6), Square(Square::C5), Color::BLACK, false},

      // Correct rank, but same file (not diagonal)
      {Square(Square::E4), Square(Square::E3), Color::BLACK, false},
      {Square(Square::C4), Square(Square::C3), Color::BLACK, false},

      // Correct rank, but en passant square too far away
      {Square(Square::E4), Square(Square::C3), Color::BLACK, false},
      {Square(Square::E4), Square(Square::G3), Color::BLACK, false},

      // Edge of board (no capture possible)
      {Square(Square::A4), Square(Square::A3), Color::BLACK, false},
      {Square(Square::H4), Square(Square::H3), Color::BLACK, false},
      {Square(Square::A4), Square(Square::H3), Color::BLACK, false},

      // Wrong direction (moving up instead of down)
      {Square(Square::E4), Square(Square::D5), Color::BLACK, false},
      {Square(Square::E4), Square(Square::F5), Color::BLACK, false},

      // White pawn positions (rank 5/6, not valid for black)
      {Square(Square::E5), Square(Square::F6), Color::BLACK, false},
      {Square(Square::C5), Square(Square::B6), Color::BLACK, false},
  };

  for (const EnPassantCase& c : cases) {
    ASSERT_EQ(PawnMoveGenerator::can_capture_en_passant(c.from, c.en_passant, c.color), c.expected)
        << "from=" << c.from.to_string() << " en_passant=" << c.en_passant.to_string() << " color=BLACK";
  }
}
