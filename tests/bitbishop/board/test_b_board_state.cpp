#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;

/**
 * @test Identical board states are equal.
 * @brief Confirms operator== returns true for identical states.
 */
TEST(BoardStateTest, IdenticalStatesEqual) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  EXPECT_TRUE(state1 == state2);
  EXPECT_FALSE(state1 != state2);
}

/**
 * @test State equals itself.
 * @brief Confirms operator== returns true when comparing state to itself.
 */
TEST(BoardStateTest, StateEqualsItself) {
  BoardState state{.m_is_white_turn = true,
                   .m_en_passant_sq = E3,
                   .m_white_castle_kingside = false,
                   .m_white_castle_queenside = true,
                   .m_black_castle_kingside = true,
                   .m_black_castle_queenside = false,
                   .m_halfmove_clock = 5,
                   .m_fullmove_number = 10};

  EXPECT_TRUE(state == state);
  EXPECT_FALSE(state != state);
}

/**
 * @test Different turn makes states unequal.
 * @brief Confirms states differ when m_is_white_turn differs.
 */
TEST(BoardStateTest, DifferentTurnUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_is_white_turn = false;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Different en passant square makes states unequal.
 * @brief Confirms states differ when m_en_passant_sq differs.
 */
TEST(BoardStateTest, DifferentEnPassantUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = E3,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_en_passant_sq = E6;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test En passant nullopt vs square makes states unequal.
 * @brief Confirms states differ when one has en passant and other doesn't.
 */
TEST(BoardStateTest, EnPassantNulloptVsSquareUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_en_passant_sq = E3;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Different white kingside castling makes states unequal.
 * @brief Confirms states differ when m_white_castle_kingside differs.
 */
TEST(BoardStateTest, DifferentWhiteKingsideCastlingUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_white_castle_kingside = false;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Different white queenside castling makes states unequal.
 * @brief Confirms states differ when m_white_castle_queenside differs.
 */
TEST(BoardStateTest, DifferentWhiteQueensideCastlingUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_white_castle_queenside = false;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Different black kingside castling makes states unequal.
 * @brief Confirms states differ when m_black_castle_kingside differs.
 */
TEST(BoardStateTest, DifferentBlackKingsideCastlingUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_black_castle_kingside = false;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Different black queenside castling makes states unequal.
 * @brief Confirms states differ when m_black_castle_queenside differs.
 */
TEST(BoardStateTest, DifferentBlackQueensideCastlingUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_black_castle_queenside = false;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Different halfmove clock makes states unequal.
 * @brief Confirms states differ when m_halfmove_clock differs.
 */
TEST(BoardStateTest, DifferentHalfmoveClockUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_halfmove_clock = 10;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Different fullmove number makes states unequal.
 * @brief Confirms states differ when m_fullmove_number differs.
 */
TEST(BoardStateTest, DifferentFullmoveNumberUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2 = state1;
  state2.m_fullmove_number = 20;

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Multiple differences make states unequal.
 * @brief Confirms states differ when multiple fields differ.
 */
TEST(BoardStateTest, MultipleDifferencesUnequal) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = std::nullopt,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 0,
                    .m_fullmove_number = 1};

  BoardState state2{.m_is_white_turn = false,
                    .m_en_passant_sq = E6,
                    .m_white_castle_kingside = false,
                    .m_white_castle_queenside = false,
                    .m_black_castle_kingside = false,
                    .m_black_castle_queenside = false,
                    .m_halfmove_clock = 50,
                    .m_fullmove_number = 100};

  EXPECT_FALSE(state1 == state2);
  EXPECT_TRUE(state1 != state2);
}

/**
 * @test Equality is symmetric.
 * @brief Confirms that if state1 == state2, then state2 == state1.
 */
TEST(BoardStateTest, EqualityIsSymmetric) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = E3,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = false,
                    .m_black_castle_kingside = false,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 5,
                    .m_fullmove_number = 10};

  BoardState state2 = state1;

  EXPECT_TRUE(state1 == state2);
  EXPECT_TRUE(state2 == state1);
}

/**
 * @test Inequality is symmetric.
 * @brief Confirms that if state1 != state2, then state2 != state1.
 */
TEST(BoardStateTest, InequalityIsSymmetric) {
  BoardState state1{.m_is_white_turn = true,
                    .m_en_passant_sq = E3,
                    .m_white_castle_kingside = true,
                    .m_white_castle_queenside = false,
                    .m_black_castle_kingside = false,
                    .m_black_castle_queenside = true,
                    .m_halfmove_clock = 5,
                    .m_fullmove_number = 10};

  BoardState state2 = state1;
  state2.m_is_white_turn = false;

  EXPECT_TRUE(state1 != state2);
  EXPECT_TRUE(state2 != state1);
}

/**
 * @test Copy produces equal state.
 * @brief Confirms copy-constructed state is equal to original.
 */
TEST(BoardStateTest, CopyProducesEqualState) {
  BoardState state1{.m_is_white_turn = false,
                    .m_en_passant_sq = D6,
                    .m_white_castle_kingside = false,
                    .m_white_castle_queenside = true,
                    .m_black_castle_kingside = true,
                    .m_black_castle_queenside = false,
                    .m_halfmove_clock = 25,
                    .m_fullmove_number = 50};

  BoardState state2 = state1;

  EXPECT_TRUE(state1 == state2);
  EXPECT_FALSE(state1 != state2);
}
