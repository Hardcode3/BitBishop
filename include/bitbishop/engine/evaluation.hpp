
#include <bitbishop/config.hpp>
#include <bitbishop/constants.hpp>
#include <bitbishop/piece.hpp>

/**
 * @brief Material values in centipawns.
 */
enum CentiPawns : int { PAWN = 100, KNIGHT = 320, BISHOP = 330, ROOK = 500, QUEEN = 900, KING = 20'000 };

/**
 * @brief Piece-Square Tables for pawns.
 *
 * - Encourage pawn to go forward (increasing value towards opponent)
 * - Encourage pawns in front of the castling squares (a2, b2, c2, f2, g2, h2) protecting the king
 * - Discourage central (d2, e2) pawns to stay there, blocking the opening
 * - Discourage central (d2, e2) pawns moving one square forward, they could control the center with a double push
 */
CX_INLINE std::array<int, Const::BOARD_SIZE> PAWSN_PSQT = {
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

/**
 * @brief Piece-Square Tables for knights.
 *
 * - Encourage knights to stay at the center, where they controll more squares
 * - Discourage strongly corners of the board
 * - Discourage borders of the board
 */
CX_INLINE std::array<int, Const::BOARD_SIZE> KNIGHT_PSQT = {
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

/**
 * @brief Piece-Square Tables for bishops.
 *
 * - Encourage bishops in a triangle shape in front of the side to move
 * - Discourage strongly edges
 * - Discourage borders
 */
CX_INLINE std::array<int, Const::BOARD_SIZE> BISHOP_PSQT = {
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

/**
 * @brief Piece-Square Tables for rooks.
 *
 * - Encourage central rooks
 * - Encourage rooks near the enemy on rank 7
 * - Discourage rooks on file A and H
 */
CX_INLINE std::array<int, Const::BOARD_SIZE> ROOK_PSQT = {
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

/**
 * @brief Piece-Square Tables for queens.
 *
 * - Encourage central positions
 * - Discourage strongly corners
 * - Discourage borders
 */
CX_INLINE std::array<int, Const::BOARD_SIZE> QUEEN_PSQT = {
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

/**
 * @brief Piece-Square Tables for kings in the midgame.
 *
 * - Encourage king safety on first ranks on the edges (1, 2)
 * - Discourage king to go out during the midgame
 */
CX_INLINE std::array<int, Const::BOARD_SIZE> KING_MIDGAME_PSQT = {
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
