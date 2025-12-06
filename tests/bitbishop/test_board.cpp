#include <gtest/gtest.h>

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>
#include <sstream>

/**
 * @test BoardTest.DefaultStartingPosDefaultConstructor
 * @brief Verifies that a newly constructed board has the default FEN starting postion
 */
TEST(BoardTest, DefaultStartingPosDefaultConstructor) {
  Board board;

  // Check major pieces
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), Pieces::WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(0, 7)), Pieces::BLACK_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 7)), Pieces::BLACK_KING);

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), Pieces::WHITE_PAWN);
    EXPECT_EQ(board.get_piece(Square(file, 6)), Pieces::BLACK_PAWN);
  }

  EXPECT_EQ(board.white_pieces().value() & board.black_pieces().value(), 0ULL);  // no overlap
  EXPECT_EQ(board.occupied().value(), board.white_pieces().value() | board.black_pieces().value());
}

/**
 * @test BoardTest.FENConstructor
 * @brief Verifies constructing a board from FEN string.
 *
 * Example FEN: standard starting position.
 */
TEST(BoardTest, FENConstructor) {
  std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Board board(start_fen);

  // Check major pieces
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), Pieces::WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(0, 7)), Pieces::BLACK_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 7)), Pieces::BLACK_KING);

  // Pawns
  for (int file = 0; file < 8; ++file) {
    EXPECT_EQ(board.get_piece(Square(file, 1)), Pieces::WHITE_PAWN);
    EXPECT_EQ(board.get_piece(Square(file, 6)), Pieces::BLACK_PAWN);
  }

  EXPECT_EQ(board.white_pieces().value() & board.black_pieces().value(), 0ULL);  // no overlap
  EXPECT_EQ(board.occupied().value(), board.white_pieces().value() | board.black_pieces().value());
}

/**
 * @test BoardTest.SetAndGetPiece
 * @brief Verifies placing and retrieving pieces.
 *
 * Checks:
 * - Placing white rook, white king, black queen
 * - Pieces correctly returned via get_piece
 * - Piece presence reflected in color-specific bitboards
 * - Occupied bitboard updated
 */
TEST(BoardTest, SetAndGetPiece) {
  Board board;

  board.set_piece(Square(0, 0), Pieces::WHITE_ROOK);   // White rook
  board.set_piece(Square(4, 0), Pieces::WHITE_KING);   // White king
  board.set_piece(Square(3, 7), Pieces::BLACK_QUEEN);  // Black queen

  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(4, 0)), Pieces::WHITE_KING);
  EXPECT_EQ(board.get_piece(Square(3, 7)), Pieces::BLACK_QUEEN);

  EXPECT_TRUE(board.white_pieces().test(Square(0, 0)));
  EXPECT_TRUE(board.white_pieces().test(Square(4, 0)));
  EXPECT_TRUE(board.black_pieces().test(Square(3, 7)));

  EXPECT_TRUE(board.occupied().test(Square(0, 0)));
  EXPECT_TRUE(board.occupied().test(Square(4, 0)));
  EXPECT_TRUE(board.occupied().test(Square(3, 7)));
}

/**
 * @test BoardTest.RemovePiece
 * @brief Verifies removing a piece from a square.
 */
TEST(BoardTest, RemovePiece) {
  Board board;
  board.set_piece(Square(0, 0), Pieces::WHITE_ROOK);
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::WHITE_ROOK);

  board.remove_piece(Square(0, 0));
  EXPECT_EQ(board.get_piece(Square(0, 0)), std::nullopt);
  EXPECT_FALSE(board.occupied().test(Square(0, 0)));
}

/**
 * @test BoardTest.ReplacePiece
 * @brief Verifies replacing a piece with another of a different color.
 *
 * Checks:
 * - Original piece replaced
 * - Bitboards updated correctly (no ghost bits left)
 */
TEST(BoardTest, ReplacePiece) {
  Board board;

  board.set_piece(Square(0, 0), Pieces::BLACK_KNIGHT);  // replace with black knight
  EXPECT_EQ(board.get_piece(Square(0, 0)), Pieces::BLACK_KNIGHT);

  EXPECT_TRUE(board.black_pieces().test(Square(0, 0)));
  EXPECT_FALSE(board.white_pieces().test(Square(0, 0)));
}

/**
 * @test BoardTest.PrintBoard
 * @brief Verifies that the board prints in correct ASCII format.
 */
TEST(BoardTest, PrintBoard) {
  Board board;

  std::ostringstream oss;
  // Redirect std::cout to oss
  std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf());
  board.print();
  // Restore original buffer
  std::cout.rdbuf(old_buf);
  std::string output = oss.str();

  std::string expected =
      "8 r n b q k b n r \n"
      "7 p p p p p p p p \n"
      "6 . . . . . . . . \n"
      "5 . . . . . . . . \n"
      "4 . . . . . . . . \n"
      "3 . . . . . . . . \n"
      "2 P P P P P P P P \n"
      "1 R N B Q K B N R \n"
      "  a b c d e f g h\n";

  EXPECT_EQ(output, expected);
}

/**
 * @test BoardTest.PawnsBitboard
 * @brief Ensures that pawns() correctly returns the bitboard for each side.
 */
TEST(BoardTest, PawnsBitboard) {
  Board board;

  Bitboard white_pawns = board.pawns(Color::WHITE);
  Bitboard black_pawns = board.pawns(Color::BLACK);

  EXPECT_EQ(white_pawns.count(), 8);
  EXPECT_EQ(black_pawns.count(), 8);
  EXPECT_TRUE(white_pawns.test(Square::A2));
  EXPECT_FALSE(white_pawns.test(Square::H7));
  EXPECT_TRUE(black_pawns.test(Square::H7));
  EXPECT_FALSE(black_pawns.test(Square::A2));
}

/**
 * @test BoardTest.KingBitboard
 * @brief Ensures that king() correctly returns the bitboard for each side.
 */
TEST(BoardTest, KingBitboard) {
  Board board;

  Bitboard white_king = board.king(Color::WHITE);
  Bitboard black_king = board.king(Color::BLACK);

  EXPECT_EQ(white_king.count(), 1);
  EXPECT_EQ(black_king.count(), 1);
  EXPECT_TRUE(white_king.test(Square::E1));
  EXPECT_TRUE(black_king.test(Square::E8));
}

/**
 * @test BoardTest.RookBitboard
 * @brief Ensures that king() correctly returns the bitboard for each side.
 */
TEST(BoardTest, RookBitboard) {
  Board board;

  Bitboard white_rooks = board.rooks(Color::WHITE);
  Bitboard black_rooks = board.rooks(Color::BLACK);

  EXPECT_EQ(white_rooks.count(), 2);
  EXPECT_EQ(black_rooks.count(), 2);
  EXPECT_TRUE(white_rooks.test(Square::H1));
  EXPECT_TRUE(white_rooks.test(Square::A1));
  EXPECT_TRUE(black_rooks.test(Square::H8));
  EXPECT_TRUE(black_rooks.test(Square::A8));
}

/**
 * @test BoardTest.KnightsBitboard
 * @brief Ensures that knights() correctly returns the bitboard for each side.
 */
TEST(BoardTest, KnightsBitboard) {
  Board board;

  Bitboard white_knights = board.knights(Color::WHITE);
  Bitboard black_knights = board.knights(Color::BLACK);

  EXPECT_EQ(white_knights.count(), 2);
  EXPECT_EQ(black_knights.count(), 2);
  EXPECT_TRUE(white_knights.test(Squares::B1));
  EXPECT_TRUE(white_knights.test(Squares::G1));
  EXPECT_TRUE(black_knights.test(Square::B8));
  EXPECT_TRUE(black_knights.test(Square::G8));
}

TEST(BoardTest, WhiteKingsideCastlingRights) {
  // TODO
}

TEST(BoardTest, WhiteQueensideCastlingRights) {
  // TODO
}

TEST(BoardTest, BlackKingsideCastlingRights) {
  // TODO
}

TEST(BoardTest, BlackQueensideCastlingRights) {
  // TODO
}

/**
 * @test BoardTest.GetWhitePieces
 * @brief Confirms that white_pieces() returns the right squares using the standard fen opening.
 */
TEST(BoardTest, GetWhitePieces) {
  Board board;  // default opening position

  Bitboard white_pieces = board.white_pieces();

  // In the default opening position, white pieces are on rank 1 and 2
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(white_pieces.test(Square(sq_index)));
  }
}

/**
 * @test BoardTest.GetBlackPieces
 * @brief Confirms that black_pieces() returns the right squares using the standard fen opening.
 */
TEST(BoardTest, GetBlackPieces) {
  Board board;  // default opening position

  Bitboard black_pieces = board.black_pieces();

  // In the default opening position, black pieces are on rank 7 and 8
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(black_pieces.test(Square(sq_index)));
  }
}

/**
 * @test BoardTest.EnemyBitboard
 * @brief Confirms that enemy() returns the opposing side’s occupied squares.
 */
TEST(BoardTest, EnemyBitboard) {
  Board board;  // default opening position

  Bitboard white_enemy = board.enemy(Color::WHITE);
  Bitboard black_enemy = board.enemy(Color::BLACK);

  // In the default opening position, white pieces are on rank 1 and 2
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(black_enemy.test(Square(sq_index)));
  }

  // In the default opening position, black pieces are on rank 7 and 8
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(white_enemy.test(Square(sq_index)));
  }
}

/**
 * @test BoardTest.FriendlyBitboard
 * @brief Confirms that friendly() returns the current side’s occupied squares.
 */
TEST(BoardTest, FriendlyBitboard) {
  Board board;  // default opening position

  Bitboard white_friends = board.friendly(Color::WHITE);
  Bitboard black_friends = board.friendly(Color::BLACK);

  // In the default opening position, white pieces are on rank 1 and 2
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(white_friends.test(Square(sq_index)));
  }

  // In the default opening position, black pieces are on rank 7 and 8
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(black_friends.test(Square(sq_index)));
  }
}

/**
 * @test BoardTest.Occupied
 * @brief Validates that occupied() returns all populated squares.
 */
TEST(BoardTest, OccupiedBoardSquares) {
  Board board;  // default opening position

  Bitboard populated = board.occupied();

  // Occupied squares for the default starting position correspond to ranks 1, 2, 7, 8
  for (int sq_index = Square::A1; sq_index <= Square::H2; sq_index++) {
    EXPECT_TRUE(populated.test(Square(sq_index)));
  }
  for (int sq_index = Square::A7; sq_index <= Square::H8; sq_index++) {
    EXPECT_TRUE(populated.test(Square(sq_index)));
  }
}

/**
 * @test BoardTest.Unoccupied
 * @brief Validates that unoccupied() returns all unoccupied squares.
 */
TEST(BoardTest, UnoccupiedBoardSquares) {
  Board board;  // default opening position

  Bitboard empty = board.unoccupied();

  // Empty squares for the default starting position
  // corresponds to ranks 3, 4, 5, 6
  for (int sq_index = Square::A3; sq_index <= Square::H6; sq_index++) {
    EXPECT_TRUE(empty.test(Square(sq_index)));
  }
}

/**
 * @test BoardTest.EnPassantSquare
 * @brief Ensures en_passant_square() works as intented when en passant square is set.
 */
TEST(BoardTest, EnPassantSquareIsAvailable) {
  // FEN: White just played d2-d4, en passant target on d3
  Board board("8/8/8/8/3P4/8/8/8 b - d3 0 1");

  auto sq = board.en_passant_square();
  ASSERT_TRUE(sq.has_value());
  EXPECT_EQ(*sq, Squares::D3);
}

/**
 * @test BoardTest.EnPassantSquare
 * @brief Ensures en_passant_square() works as intented when en passant square is not set.
 */
TEST(BoardTest, EnPassantSquareNotAvailable) {
  // FEN with no en passant
  Board board2("8/8/8/8/8/8/8/8 w - - 0 1");
  EXPECT_FALSE(board2.en_passant_square().has_value());
}
