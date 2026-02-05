
#include <bitbishop/board.hpp>
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <bitbishop/piece.hpp>

namespace Eval {

using PieceSquareTable = std::array<int, Const::BOARD_SIZE>;

/**
 * @brief Flips a chess board index with respect to the ranks.
 * @note Chess board indexes belongs to [0,63].
 * @param index Chess board index to flip.
 * @return Horizontally symmetric index.
 */
CX_FN std::size_t flip_index_horizontally(std::size_t index) {
  constexpr std::size_t WIDTH = Const::BOARD_WIDTH;
  constexpr std::size_t SIZE = Const::BOARD_SIZE;
  return (SIZE - 1) - (index / WIDTH) * WIDTH - (SIZE - 1 - index) % WIDTH;
}

/**
 * @brief Flips a piece square table with respect to the ranks.
 * @param psqt piece square table to flip.
 * @return Horizontally symmetric piece square table.
 */
CX_FN PieceSquareTable flip_psqt(const PieceSquareTable& psqt) {
  using namespace Const;
  PieceSquareTable result;
  for (std::size_t i = 0; i < BOARD_SIZE; ++i) {
    result[flip_index_horizontally(i)] = psqt[i];
  }
  return result;
}

/**
 * @brief Material values in centipawns.
 */
enum MaterialValue : std::uint16_t { PAWN = 100, KNIGHT = 320, BISHOP = 330, ROOK = 500, QUEEN = 900, KING = 20'000 };

/**
 * @brief Piece-Square Tables for pawns.
 *
 * - Encourage pawn to go forward (increasing value towards opponent)
 * - Encourage pawns in front of the castling squares (a2, b2, c2, f2, g2, h2) protecting the king
 * - Discourage central (d2, e2) pawns to stay there, blocking the opening
 * - Discourage central (d2, e2) pawns moving one square forward, they could control the center with a double push
 */
CX_INLINE PieceSquareTable PAWN_PSQT_WHITE = {
    // clang-format off
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
    // clang-format on
};

CX_INLINE PieceSquareTable PAWN_PSQT_BLACK = flip_psqt(PAWN_PSQT_WHITE);

/**
 * @brief Piece-Square Tables for knights.
 *
 * - Encourage knights to stay at the center, where they controll more squares
 * - Discourage strongly corners of the board
 * - Discourage borders of the board
 */
CX_INLINE PieceSquareTable KNIGHT_PSQT_WHITE = {
    // clang-format off
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
    // clang-format on
};

CX_INLINE PieceSquareTable KNIGHT_PSQT_BLACK = flip_psqt(KNIGHT_PSQT_WHITE);

/**
 * @brief Piece-Square Tables for bishops.
 *
 * - Encourage bishops in a triangle shape in front of the side to move
 * - Discourage strongly edges
 * - Discourage borders
 */
CX_INLINE PieceSquareTable BISHOP_PSQT_WHITE = {
    // clang-format off
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
    // clang-format on
};

CX_INLINE PieceSquareTable BISHOP_PSQT_BLACK = flip_psqt(BISHOP_PSQT_WHITE);

/**
 * @brief Piece-Square Tables for rooks.
 *
 * - Encourage central rooks
 * - Encourage rooks near the enemy on rank 7
 * - Discourage rooks on file A and H
 */
CX_INLINE PieceSquareTable ROOK_PSQT_WHITE = {
    // clang-format off
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
    // clang-format on
};

CX_INLINE PieceSquareTable ROOK_PSQT_BLACK = flip_psqt(ROOK_PSQT_WHITE);

/**
 * @brief Piece-Square Tables for queens.
 *
 * - Encourage central positions
 * - Discourage strongly corners
 * - Discourage borders
 */
CX_INLINE PieceSquareTable QUEEN_PSQT_WHITE = {
    // clang-format off
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
    // clang-format on
};

CX_INLINE PieceSquareTable QUEEN_PSQT_BLACK = flip_psqt(QUEEN_PSQT_WHITE);

/**
 * @brief Piece-Square Tables for kings in the midgame.
 *
 * - Encourage king safety on first ranks on the edges (1, 2)
 * - Discourage king to go out during the midgame
 */
CX_INLINE PieceSquareTable KING_MIDGAME_PSQT_WHITE = {
    // clang-format off
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20
    // clang-format on
};

CX_INLINE PieceSquareTable KING_MIDGAME_PSQT_BLACK = flip_psqt(KING_MIDGAME_PSQT_WHITE);

/**
 * @brief Evaluates the material score.
 * @param board Board to evaluate material on
 * @param side Color to evaluate material for
 * @return Absolute (positive, same for black and white) score
 * @note No coefficient *(-1) applied to blacks
 */
[[nodiscard]] int evaluate_material(const Board& board, Color side) noexcept;

/**
 * @brief Evaluates the score from piece square tables
 * @param psqt Piece square table to use for evaluation
 * @param bitboard Bitboard representing occupied squares for the same piece's type than psqt
 * @return Absolute (positive or negative, same for black and white) score
 * @note No coefficient *(-1) applied to blacks
 */
[[nodiscard]] int compute_score_from_psqt(const PieceSquareTable& psqt, const Bitboard& bitboard) noexcept;

/**
 * @brief Evaluates the score from a board state
 * @param board Board to evaluate material on
 * @param side Color to evaluate material for
 * @return Absolute (positive or negative) score
 * @note No coefficient *(-1) applied to blacks
 */
[[nodiscard]] int evaluate_psqt(const Board& board, Color side) noexcept;

/**
 * @brief Provides a score for the current board state.
 * @param board Board to evaluate material on
 * @param side Color to evaluate material for
 * @return integer with negative scores being in favour of blacks, positive in favour of whites and zeo being
 * neutral.
 * @note Coefficient *(-1) applied to blacks and *(1) applied to whites
 */
[[nodiscard]] int evaluate(const Board& board, Color side) noexcept;

}  // namespace Eval
