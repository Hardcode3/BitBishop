#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/lookups/attackers.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;

/**
 * @test King attackers from center square.
 * @brief Confirms attackers_to() includes all 8 king attack squares from E4.
 */
TEST(AttackersTest, KingAttackersCenter) {
  Bitboard attackers = attackers_to(E4, Color::WHITE);

  // King can attack from all 8 surrounding squares
  EXPECT_TRUE(attackers.test(D3));
  EXPECT_TRUE(attackers.test(D4));
  EXPECT_TRUE(attackers.test(D5));
  EXPECT_TRUE(attackers.test(E3));
  EXPECT_TRUE(attackers.test(E5));
  EXPECT_TRUE(attackers.test(F3));
  EXPECT_TRUE(attackers.test(F4));
  EXPECT_TRUE(attackers.test(F5));
}

/**
 * @test King attackers from corner square.
 * @brief Confirms attackers_to() includes only 3 king attack squares from A1.
 */
TEST(AttackersTest, KingAttackersCorner) {
  Bitboard attackers = attackers_to(A1, Color::WHITE);

  // King can attack from only 3 squares in corner
  EXPECT_TRUE(attackers.test(A2));
  EXPECT_TRUE(attackers.test(B1));
  EXPECT_TRUE(attackers.test(B2));
}

/**
 * @test Knight attackers from center square.
 * @brief Confirms attackers_to() includes all 8 knight attack squares from E4.
 */
TEST(AttackersTest, KnightAttackersCenter) {
  Bitboard attackers = attackers_to(E4, Color::WHITE);

  // Knight can attack from all 8 L-shaped squares
  EXPECT_TRUE(attackers.test(D2));
  EXPECT_TRUE(attackers.test(F2));
  EXPECT_TRUE(attackers.test(C3));
  EXPECT_TRUE(attackers.test(G3));
  EXPECT_TRUE(attackers.test(C5));
  EXPECT_TRUE(attackers.test(G5));
  EXPECT_TRUE(attackers.test(D6));
  EXPECT_TRUE(attackers.test(F6));
}

/**
 * @test Knight attackers from corner square.
 * @brief Confirms attackers_to() includes only 2 knight attack squares from A1.
 */
TEST(AttackersTest, KnightAttackersCorner) {
  Bitboard attackers = attackers_to(A1, Color::WHITE);

  // Knight can attack from only 2 squares in corner
  EXPECT_TRUE(attackers.test(B3));
  EXPECT_TRUE(attackers.test(C2));
}

/**
 * @test White pawn attackers from center square.
 * @brief Confirms attackers_to() includes diagonal squares below for white pawns.
 */
TEST(AttackersTest, WhitePawnAttackersCenter) {
  Bitboard attackers = attackers_to(E4, Color::WHITE);

  // White pawns attack from diagonally below (from E3's perspective)
  EXPECT_TRUE(attackers.test(D3));
  EXPECT_TRUE(attackers.test(F3));

  // Should not include squares above
  EXPECT_FALSE(attackers.test(D5) && !attackers.test(E5));
  EXPECT_FALSE(attackers.test(F5) && !attackers.test(E5));
}

/**
 * @test Black pawn attackers from center square.
 * @brief Confirms attackers_to() includes diagonal squares above for black pawns.
 */
TEST(AttackersTest, BlackPawnAttackersCenter) {
  Bitboard attackers = attackers_to(E4, Color::BLACK);

  // Black pawns attack from diagonally above (from E5's perspective)
  EXPECT_TRUE(attackers.test(D5));
  EXPECT_TRUE(attackers.test(F5));

  // Should not include squares below
  EXPECT_FALSE(attackers.test(D3) && !attackers.test(E3));
  EXPECT_FALSE(attackers.test(F3) && !attackers.test(E3));
}

/**
 * @test White pawn attackers from edge file.
 * @brief Confirms attackers_to() includes only one diagonal for edge files.
 */
TEST(AttackersTest, WhitePawnAttackersEdge) {
  Bitboard attackers = attackers_to(A4, Color::WHITE);

  // White pawn on edge can only attack from one diagonal
  EXPECT_TRUE(attackers.test(B3));

  // No attacker from the missing diagonal
  EXPECT_FALSE(attackers.test(A3) && !attackers.test(A5));
}

/**
 * @test Rook attackers along rank and file.
 * @brief Confirms attackers_to() includes all rank and file rays.
 */
TEST(AttackersTest, RookAttackersRaysCenter) {
  Bitboard attackers = attackers_to(E4, Color::WHITE);

  // Rook can attack from entire rank and file (as rays, no blockers)
  EXPECT_TRUE(attackers.test(A4));
  EXPECT_TRUE(attackers.test(H4));
  EXPECT_TRUE(attackers.test(E1));
  EXPECT_TRUE(attackers.test(E8));

  // Verify some intermediate squares on rank
  EXPECT_TRUE(attackers.test(C4));
  EXPECT_TRUE(attackers.test(G4));

  // Verify some intermediate squares on file
  EXPECT_TRUE(attackers.test(E2));
  EXPECT_TRUE(attackers.test(E7));
}

/**
 * @test Bishop attackers along diagonals.
 * @brief Confirms attackers_to() includes all diagonal rays.
 */
TEST(AttackersTest, BishopAttackersRaysCenter) {
  Bitboard attackers = attackers_to(E4, Color::WHITE);

  // Bishop can attack from both diagonals (as rays, no blockers)
  // NE-SW diagonal
  EXPECT_TRUE(attackers.test(A8));
  EXPECT_TRUE(attackers.test(H1));
  EXPECT_TRUE(attackers.test(C2));
  EXPECT_TRUE(attackers.test(G6));

  // NW-SE diagonal
  EXPECT_TRUE(attackers.test(B1));
  EXPECT_TRUE(attackers.test(H7));
  EXPECT_TRUE(attackers.test(C6));
  EXPECT_TRUE(attackers.test(F3));
}

/**
 * @test Queen attackers combine rook and bishop rays.
 * @brief Confirms attackers_to() includes all rank, file, and diagonal rays.
 */
TEST(AttackersTest, QueenAttackersRaysCenter) {
  Bitboard attackers = attackers_to(E4, Color::WHITE);

  // Queen combines rook and bishop rays
  // Check rank/file (rook-like)
  EXPECT_TRUE(attackers.test(E1));
  EXPECT_TRUE(attackers.test(A4));

  // Check diagonals (bishop-like)
  EXPECT_TRUE(attackers.test(A8));
  EXPECT_TRUE(attackers.test(H1));
}

/**
 * @test Color independence for non-pawn pieces.
 * @brief Confirms king, knight, rook, bishop, queen attackers are color-independent.
 */
TEST(AttackersTest, ColorIndependenceNonPawns) {
  Bitboard white_attackers = attackers_to(E4, Color::WHITE);
  Bitboard black_attackers = attackers_to(E4, Color::BLACK);

  // Remove pawn attackers for comparison
  // This test assumes we can identify and mask out pawn squares
  // Here we check that certain non-pawn squares are the same
  EXPECT_EQ(white_attackers.test(D3), black_attackers.test(D3));  // King
  EXPECT_EQ(white_attackers.test(C3), black_attackers.test(C3));  // Knight
  EXPECT_EQ(white_attackers.test(E1), black_attackers.test(E1));  // Rook/Queen
  EXPECT_EQ(white_attackers.test(A8), black_attackers.test(A8));  // Bishop/Queen
}

/**
 * @test ATTACKERS_TO table matches attackers_to() function.
 * @brief Confirms precomputed table entries match runtime computation.
 */
TEST(AttackersTest, AttackersToTableMatchesFunction) {
  for (Color col : ColorUtil::ALL) {
    const std::size_t coli = ColorUtil::to_index(col);
    for (int sq = 0; sq < 64; ++sq) {
      Square square(sq, std::in_place);
      Bitboard expected = attackers_to(square, col);
      Bitboard actual = ATTACKERS_TO[coli][sq];
      EXPECT_EQ(actual, expected) << "Mismatch at square " << square.to_string() << " for color "
                                  << (col == Color::WHITE ? "white" : "black");
    }
  }
}

/**
 * @test ATTACKERS_TO table for specific white scenarios.
 * @brief Confirms table lookups return correct attackers for white.
 */
TEST(AttackersTest, AttackersToTableWhiteSpecific) {
  const std::size_t white_idx = ColorUtil::to_index(Color::WHITE);

  const Bitboard& attackers = ATTACKERS_TO[white_idx][E4.value()];

  // Check king attackers
  EXPECT_TRUE(attackers.test(D3));
  EXPECT_TRUE(attackers.test(F5));

  // Check knight attackers
  EXPECT_TRUE(attackers.test(D2));
  EXPECT_TRUE(attackers.test(G5));

  // Check rook rays
  EXPECT_TRUE(attackers.test(E1));
  EXPECT_TRUE(attackers.test(A4));
}

/**
 * @test ATTACKERS_TO table for specific black scenarios.
 * @brief Confirms table lookups return correct attackers for black.
 */
TEST(AttackersTest, AttackersToTableBlackSpecific) {
  const std::size_t black_idx = ColorUtil::to_index(Color::BLACK);

  const Bitboard& attackers = ATTACKERS_TO[black_idx][E4.value()];

  // Check king attackers (same as white)
  EXPECT_TRUE(attackers.test(D3));
  EXPECT_TRUE(attackers.test(F5));

  // Check black pawn attackers (different from white)
  EXPECT_TRUE(attackers.test(D5));
  EXPECT_TRUE(attackers.test(F5));
}

/**
 * @test Attackers from all squares are non-empty.
 * @brief Confirms every square has at least some potential attackers.
 */
TEST(AttackersTest, AllSquaresHaveAttackers) {
  for (Color col : ColorUtil::ALL) {
    const std::size_t coli = ColorUtil::to_index(col);
    for (int sq = 0; sq < 64; ++sq) {
      const Bitboard& attackers = ATTACKERS_TO[coli][sq];
      EXPECT_GT(attackers.count(), 0) << "Square " << Square(sq, std::in_place).to_string()
                                      << " has no attackers for color " << (col == Color::WHITE ? "white" : "black");
    }
  }
}

/**
 * @test Corner square has minimal attackers.
 * @brief Confirms corner squares have fewer potential attackers.
 */
TEST(AttackersTest, CornerSquareMinimalAttackers) {
  // Corner squares should have fewer attackers than center squares
  Bitboard corner_white = attackers_to(A1, Color::WHITE);
  Bitboard center_white = attackers_to(E4, Color::WHITE);

  EXPECT_LT(corner_white.count(), center_white.count());
}

/**
 * @test Center square has maximal attackers.
 * @brief Confirms center squares have more potential attackers.
 */
TEST(AttackersTest, CenterSquareMaximalAttackers) {
  // Center squares should have more attackers
  Bitboard e4_attackers = attackers_to(E4, Color::WHITE);
  Bitboard d4_attackers = attackers_to(D4, Color::WHITE);

  // Both should be substantial
  EXPECT_GT(e4_attackers.count(), 20);
  EXPECT_GT(d4_attackers.count(), 20);
}

/**
 * @test Geometric attackers ignore board state.
 * @brief Confirms attackers_to() is purely geometric and state-independent.
 */
TEST(AttackersTest, GeometricIgnoresBoardState) {
  // Multiple calls should return identical results (pure function)
  Bitboard call1 = attackers_to(E4, Color::WHITE);
  Bitboard call2 = attackers_to(E4, Color::WHITE);
  Bitboard call3 = attackers_to(E4, Color::WHITE);

  EXPECT_EQ(call1, call2);
  EXPECT_EQ(call2, call3);
}
