#pragma once

/**
 * @namespace Const
 * @brief Core chess board constants for indexing and dimensions.
 *
 * Defines fundamental constants used throughout the chess engine for board
 * representation and square indexing. Ranks are numbered 1-8 from white's
 * perspective (rank 1 = white's back rank, rank 8 = black's back rank).
 * Files are labeled A-H from left to right when viewing from white's side.
 *
 * These indices are used to convert between square representations and to
 * access specific ranks/files in bitboard operations.
 *
 * @example Converting a square to rank and file indices
 * @code
 * Square e4(28);  // Square e4 is at index 28
 * int rank = e4.value() / Const::BOARD_WIDTH;  // rank = 3 (rank 4)
 * int file = e4.value() % Const::BOARD_WIDTH;  // file = 4 (file E)
 * @endcode
 */
namespace Const {

// Board dimensions
constexpr int BOARD_WIDTH = 8;                         // Chess board is 8x8
constexpr int BOARD_SIZE = BOARD_WIDTH * BOARD_WIDTH;  // Total 64 squares

// Rank indices (rows, numbered 1-8 from white's perspective)
constexpr int RANK_1_IND = 0;
constexpr int RANK_2_IND = 1;
constexpr int RANK_3_IND = 2;
constexpr int RANK_4_IND = 3;
constexpr int RANK_5_IND = 4;
constexpr int RANK_6_IND = 5;
constexpr int RANK_7_IND = 6;
constexpr int RANK_8_IND = 7;

// File indices (columns, labeled A-H from left to right)
constexpr int FILE_A_IND = 0;
constexpr int FILE_B_IND = 1;
constexpr int FILE_C_IND = 2;
constexpr int FILE_D_IND = 3;
constexpr int FILE_E_IND = 4;
constexpr int FILE_F_IND = 5;
constexpr int FILE_G_IND = 6;
constexpr int FILE_H_IND = 7;

}  // namespace Const
