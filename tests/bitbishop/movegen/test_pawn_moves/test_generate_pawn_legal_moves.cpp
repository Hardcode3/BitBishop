#include <gtest/gtest.h>

#include <bitbishop/bitboard.hpp>
#include <bitbishop/board.hpp>
#include <bitbishop/helpers/moves.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/movegen/pawn_moves.hpp>
#include <bitbishop/movegen/pins.hpp>
#include <bitbishop/square.hpp>

using namespace Squares;
using namespace Pieces;

/**
 * @test Single white pawn push.
 * @brief Confirms generate_pawn_legal_moves() generates single forward move
 *        for white pawn.
 */
TEST(GeneratePawnLegalMovesTest, WhiteSinglePush) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E2, E3, std::nullopt, false, false, false}));
}

/**
 * @test Single black pawn push.
 * @brief Confirms generate_pawn_legal_moves() generates single forward move
 *        for black pawn.
 */
TEST(GeneratePawnLegalMovesTest, BlackSinglePush) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(E7, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::BLACK, E8, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E7, E6, std::nullopt, false, false, false}));
}

/**
 * @test White pawn double push from starting rank.
 * @brief Confirms generate_pawn_legal_moves() generates double push for
 *        white pawn on second rank.
 */
TEST(GeneratePawnLegalMovesTest, WhiteDoublePush) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E2, E3, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E4, std::nullopt, false, false, false}));
}

/**
 * @test Black pawn double push from starting rank.
 * @brief Confirms generate_pawn_legal_moves() generates double push for
 *        black pawn on seventh rank.
 */
TEST(GeneratePawnLegalMovesTest, BlackDoublePush) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(E7, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::BLACK, E8, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E7, E6, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, E5, std::nullopt, false, false, false}));
}

/**
 * @test Pawn blocked by friendly piece.
 * @brief Confirms generate_pawn_legal_moves() generates no moves when
 *        pawn is blocked by friendly piece.
 */
TEST(GeneratePawnLegalMovesTest, BlockedByFriendlyPiece) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E3, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Pawn blocked by enemy piece.
 * @brief Confirms generate_pawn_legal_moves() generates no forward moves
 *        when pawn is blocked by enemy piece.
 */
TEST(GeneratePawnLegalMovesTest, BlockedByEnemyPiece) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E3, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Double push blocked by piece on intermediate square.
 * @brief Confirms generate_pawn_legal_moves() does not generate double push
 *        when intermediate square is blocked.
 */
TEST(GeneratePawnLegalMovesTest, DoublePushBlockedIntermediate) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E3, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
  EXPECT_FALSE(contains_move(moves, {E2, E4, std::nullopt, false, false, false}));
}

/**
 * @test Double push blocked by piece on target square.
 * @brief Confirms generate_pawn_legal_moves() does not generate double push
 *        when target square is blocked.
 */
TEST(GeneratePawnLegalMovesTest, DoublePushBlockedTarget) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(E4, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E2, E3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E2, E4, std::nullopt, false, false, false}));
}

/**
 * @test Pawn captures enemy piece diagonally.
 * @brief Confirms generate_pawn_legal_moves() generates diagonal captures.
 */
TEST(GeneratePawnLegalMovesTest, DiagonalCapture) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_PAWN);
  board.set_piece(D5, BLACK_PAWN);
  board.set_piece(F5, BLACK_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, D5, std::nullopt, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E4, F5, std::nullopt, true, false, false}));
}

/**
 * @test Pawn does not capture friendly pieces.
 * @brief Confirms generate_pawn_legal_moves() does not generate captures
 *        of friendly pieces.
 */
TEST(GeneratePawnLegalMovesTest, NoFriendlyCapture) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E4, WHITE_PAWN);
  board.set_piece(D5, WHITE_PAWN);
  board.set_piece(F5, WHITE_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {E4, E5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {D5, D6, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, D5, std::nullopt, true, false, false}));
  EXPECT_FALSE(contains_move(moves, {E4, F5, std::nullopt, true, false, false}));
}

/**
 * @test White pawn promotion on eighth rank.
 * @brief Confirms generate_pawn_legal_moves() generates all four promotion
 *        moves for white pawn reaching eighth rank.
 */
TEST(GeneratePawnLegalMovesTest, WhitePawnPromotion) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E7, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_QUEEN, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_ROOK, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_BISHOP, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, E8, WHITE_KNIGHT, false, false, false}));
}

/**
 * @test Black pawn promotion on first rank.
 * @brief Confirms generate_pawn_legal_moves() generates all four promotion
 *        moves for black pawn reaching first rank.
 */
TEST(GeneratePawnLegalMovesTest, BlackPawnPromotion) {
  Board board = Board::Empty();
  board.set_piece(E8, BLACK_KING);
  board.set_piece(E2, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::BLACK, E8, check_mask, pins);

  EXPECT_EQ(moves.size(), 4);
  EXPECT_TRUE(contains_move(moves, {E2, E1, BLACK_QUEEN, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E1, BLACK_ROOK, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E1, BLACK_BISHOP, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {E2, E1, BLACK_KNIGHT, false, false, false}));
}

/**
 * @test Capture with promotion.
 * @brief Confirms generate_pawn_legal_moves() generates promotion captures.
 */
TEST(GeneratePawnLegalMovesTest, CaptureWithPromotion) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E7, WHITE_PAWN);
  board.set_piece(D8, BLACK_ROOK);
  board.set_piece(F8, BLACK_KNIGHT);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // 4 promotions forward + 4 capture promotions left + 4 capture promotions right = 12
  EXPECT_EQ(moves.size(), 12);

  // Check some capture promotions
  EXPECT_TRUE(contains_move(moves, {E7, D8, WHITE_QUEEN, true, false, false}));
  EXPECT_TRUE(contains_move(moves, {E7, F8, WHITE_QUEEN, true, false, false}));
}

/**
 * @test Check mask restricts pawn moves.
 * @brief Confirms generate_pawn_legal_moves() only generates moves within
 *        check mask.
 */
TEST(GeneratePawnLegalMovesTest, CheckMaskRestriction) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(E3);

  std::vector<Move> moves;
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E2, E3, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E2, E4, std::nullopt, false, false, false}));
}

/**
 * @test Empty check mask generates no moves.
 * @brief Confirms generate_pawn_legal_moves() generates no moves when
 *        check mask is empty.
 */
TEST(GeneratePawnLegalMovesTest, EmptyCheckMaskNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Zeros();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Pinned pawn moves along pin ray only.
 * @brief Confirms generate_pawn_legal_moves() restricts pinned pawn to
 *        moves along pin ray.
 */
TEST(GeneratePawnLegalMovesTest, PinnedPawnAlongRay) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);
  board.set_piece(E8, BLACK_ROOK);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // Pawn can move forward along pin ray
  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E3, E4, std::nullopt, false, false, false}));
}

/**
 * @test Diagonally pinned pawn cannot move.
 * @brief Confirms generate_pawn_legal_moves() generates no moves when
 *        pawn is pinned diagonally.
 */
TEST(GeneratePawnLegalMovesTest, DiagonallyPinnedPawnNoMoves) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_PAWN);
  board.set_piece(H4, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // Pawn pinned diagonally cannot move forward
  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Diagonally pinned pawn can capture attacker.
 * @brief Confirms generate_pawn_legal_moves() allows pinned pawn to
 *        capture along pin ray.
 */
TEST(GeneratePawnLegalMovesTest, DiagonallyPinnedPawnCapturesAttacker) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(F2, WHITE_PAWN);
  board.set_piece(G3, BLACK_BISHOP);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {F2, G3, std::nullopt, true, false, false}));
}

/**
 * @test Multiple pawns generate moves independently.
 * @brief Confirms generate_pawn_legal_moves() generates moves for all pawns.
 */
TEST(GeneratePawnLegalMovesTest, MultiplePawns) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(D2, WHITE_PAWN);
  board.set_piece(F2, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // Each pawn has 2 moves (single and double push) = 6 total
  EXPECT_EQ(moves.size(), 6);
}

/**
 * @test Pawn on edge of board.
 * @brief Confirms generate_pawn_legal_moves() correctly handles pawns on
 *        board edges (only one capture available).
 */
TEST(GeneratePawnLegalMovesTest, PawnOnEdge) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(A4, WHITE_PAWN);
  board.set_piece(B5, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 2);
  EXPECT_TRUE(contains_move(moves, {A4, A5, std::nullopt, false, false, false}));
  EXPECT_TRUE(contains_move(moves, {A4, B5, std::nullopt, true, false, false}));
}

/**
 * @test No pawns on board.
 * @brief Confirms generate_pawn_legal_moves() handles empty pawn bitboard.
 */
TEST(GeneratePawnLegalMovesTest, NoPawnsOnBoard) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 0);
}

/**
 * @test Moves vector accumulates correctly.
 * @brief Confirms generate_pawn_legal_moves() appends to existing moves.
 */
TEST(GeneratePawnLegalMovesTest, MovesVectorAccumulates) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);

  std::vector<Move> moves;
  moves.emplace_back(Move::make(A1, A2));

  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 3);
  EXPECT_TRUE(contains_move(moves, Move::make(A1, A2)));
}

/**
 * @test Starting position pawn moves.
 * @brief Confirms generate_pawn_legal_moves() generates correct moves from
 *        starting position.
 */
TEST(GeneratePawnLegalMovesTest, StartingPositionPawns) {
  Board board = Board::StartingPosition();

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // 8 pawns, each with 2 moves (single and double push) = 16 total
  EXPECT_EQ(moves.size(), 16);
}

/**
 * @test Move properties are correct.
 * @brief Confirms generate_pawn_legal_moves() sets all move properties
 *        correctly.
 */
TEST(GeneratePawnLegalMovesTest, MovePropertiesCorrect) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E2, WHITE_PAWN);
  board.set_piece(D3, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  for (const Move& move : moves) {
    EXPECT_EQ(move.from, E2);
    EXPECT_FALSE(move.is_castling);
    EXPECT_FALSE(move.is_en_passant);

    if (move.to == D3) {
      EXPECT_TRUE(move.is_capture);
    } else {
      EXPECT_FALSE(move.is_capture);
    }
  }
}

/**
 * @test Pawn cannot move off starting rank without double push option.
 * @brief Confirms pawn moved off starting rank loses double push ability.
 */
TEST(GeneratePawnLegalMovesTest, NoDoublePushOffStartingRank) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E3, WHITE_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  EXPECT_EQ(moves.size(), 1);
  EXPECT_TRUE(contains_move(moves, {E3, E4, std::nullopt, false, false, false}));
  EXPECT_FALSE(contains_move(moves, {E3, E5, std::nullopt, false, false, false}));
}

/**
 * @test White en passant capture.
 * @brief Confirms generate_pawn_legal_moves() generates en passant capture
 *        for white pawn.
 */
TEST(GeneratePawnLegalMovesTest, WhiteEnPassantCapture) {
  Board board("rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // Should include en passant capture
  EXPECT_TRUE(contains_move(moves, {D5, E6, std::nullopt, true, true, false}));
}

/**
 * @test Black en passant capture.
 * @brief Confirms generate_pawn_legal_moves() generates en passant capture
 *        for black pawn.
 */
TEST(GeneratePawnLegalMovesTest, BlackEnPassantCapture) {
  Board board("rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E8, board, Color::BLACK);

  generate_pawn_legal_moves(moves, board, Color::BLACK, E8, check_mask, pins);

  // Should include en passant capture
  EXPECT_TRUE(contains_move(moves, {D4, E3, std::nullopt, true, true, false}));
}

/**
 * @test En passant on both sides.
 * @brief Confirms generate_pawn_legal_moves() generates en passant when
 *        two pawns can capture same target.
 */
TEST(GeneratePawnLegalMovesTest, EnPassantBothSides) {
  Board board("rnbqkbnr/ppp1pppp/8/2PpP3/8/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 1");

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // Both pawns can capture en passant
  EXPECT_TRUE(contains_move(moves, {C5, D6, std::nullopt, true, true, false}));
  EXPECT_TRUE(contains_move(moves, {E5, D6, std::nullopt, true, true, false}));
}

/**
 * @test No en passant without target square.
 * @brief Confirms generate_pawn_legal_moves() does not generate en passant
 *        when no en passant square is set.
 */
TEST(GeneratePawnLegalMovesTest, NoEnPassantWithoutTarget) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(D5, WHITE_PAWN);
  board.set_piece(E5, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // No en passant without target square
  EXPECT_FALSE(contains_move(moves, {D5, E6, std::nullopt, true, true, false}));
}

/**
 * @test En passant blocked by check mask.
 * @brief Confirms generate_pawn_legal_moves() does not generate en passant
 *        when target square not in check mask.
 */
TEST(GeneratePawnLegalMovesTest, EnPassantBlockedByCheckMask) {
  Board board("rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

  Bitboard check_mask = Bitboard::Zeros();
  check_mask.set(D6);  // En passant square E6 not in mask

  std::vector<Move> moves;
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // En passant not allowed
  EXPECT_FALSE(contains_move(moves, {D5, E6, std::nullopt, true, true, false}));
}

/**
 * @test En passant blocked by pin.
 * @brief Confirms generate_pawn_legal_moves() does not generate en passant
 *        when pawn is pinned perpendicular to en passant direction.
 */
TEST(GeneratePawnLegalMovesTest, EnPassantBlockedByPin) {
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(E5, WHITE_PAWN);
  board.set_piece(D5, BLACK_PAWN);
  board.set_piece(E8, BLACK_ROOK);

  // Manually set en passant square
  Board board_with_ep("8/8/8/3pP3/8/8/8/4K2r b - e6 0 1");

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board_with_ep, Color::WHITE);

  generate_pawn_legal_moves(moves, board_with_ep, Color::WHITE, E1, check_mask, pins);

  // Pinned pawn cannot capture en passant
  EXPECT_FALSE(contains_move(moves, {E5, D6, std::nullopt, true, true, false}));
}

/**
 * @test En passant reveals check (horizontal pin).
 * @brief Confirms generate_pawn_legal_moves() does not generate en passant
 *        when capturing would expose king to horizontal attack.
 */
TEST(GeneratePawnLegalMovesTest, EnPassantRevealsHorizontalCheck) {
  // White king on E5, white pawn on D5, black pawn on E5, black rook on A5
  // If white captures en passant on E6, removes both pawns and exposes king
  Board board("8/8/8/r2PpK2/8/8/8/8 w - e6 0 1");

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(F5, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, F5, check_mask, pins);

  // En passant not allowed as it would expose king
  EXPECT_FALSE(contains_move(moves, {D5, E6, std::nullopt, true, true, false}));
}

/**
 * @test En passant legal when safe.
 * @brief Confirms generate_pawn_legal_moves() generates en passant when
 *        it doesn't expose king to check.
 */
TEST(GeneratePawnLegalMovesTest, EnPassantLegalWhenSafe) {
  Board board("rnbqkbnr/ppp2ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // En passant is safe
  EXPECT_TRUE(contains_move(moves, {D5, E6, std::nullopt, true, true, false}));
}

/**
 * @test En passant with promotion (edge case).
 * @brief Confirms en passant is only available on correct ranks
 *        (5th for white, 4th for black).
 */
TEST(GeneratePawnLegalMovesTest, EnPassantOnlyOnCorrectRank) {
  // White pawn on 6th rank - cannot do en passant from here
  Board board = Board::Empty();
  board.set_piece(E1, WHITE_KING);
  board.set_piece(D6, WHITE_PAWN);
  board.set_piece(E6, BLACK_PAWN);

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins;

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // No en passant from 6th rank
  EXPECT_FALSE(contains_move(moves, {D6, E7, std::nullopt, true, true, false}));
}

/**
 * @test En passant move properties correct.
 * @brief Confirms en passant move has correct flags set.
 */
TEST(GeneratePawnLegalMovesTest, EnPassantMoveProperties) {
  Board board("rnbqkbnr/pppp1ppp/8/3Pp3/8/8/PPP1PPPP/RNBQKBNR w KQkq e6 0 1");

  std::vector<Move> moves;
  Bitboard check_mask = Bitboard::Ones();
  PinResult pins = compute_pins(E1, board, Color::WHITE);

  generate_pawn_legal_moves(moves, board, Color::WHITE, E1, check_mask, pins);

  // Find en passant move
  bool found = false;
  for (const Move& move : moves) {
    if (move.from == D5 && move.to == E6) {
      EXPECT_TRUE(move.is_capture);
      EXPECT_TRUE(move.is_en_passant);
      EXPECT_FALSE(move.is_castling);
      EXPECT_FALSE(move.promotion.has_value());
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);
}
