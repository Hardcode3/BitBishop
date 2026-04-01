#pragma once

#include <bitbishop/config.hpp>

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
CX_INLINE int BOARD_WIDTH = 8;                         // Chess board is 8x8
CX_INLINE int BOARD_SIZE = BOARD_WIDTH * BOARD_WIDTH;  // Total 64 squares

// Rank indices (rows, numbered 1-8 from white's perspective)
CX_INLINE int RANK_1_IND = 0;
CX_INLINE int RANK_2_IND = 1;
CX_INLINE int RANK_3_IND = 2;
CX_INLINE int RANK_4_IND = 3;
CX_INLINE int RANK_5_IND = 4;
CX_INLINE int RANK_6_IND = 5;
CX_INLINE int RANK_7_IND = 6;
CX_INLINE int RANK_8_IND = 7;

// File indices (columns, labeled A-H from left to right)
CX_INLINE int FILE_A_IND = 0;
CX_INLINE int FILE_B_IND = 1;
CX_INLINE int FILE_C_IND = 2;
CX_INLINE int FILE_D_IND = 3;
CX_INLINE int FILE_E_IND = 4;
CX_INLINE int FILE_F_IND = 5;
CX_INLINE int FILE_G_IND = 6;
CX_INLINE int FILE_H_IND = 7;

/**
 * Number of possible encodings for castling right.
 * Used in Zobrist hashing.
 * There are 4 possible castling rights so 2^4 = 16 different states.
 */
CX_INLINE int CASTLING_RIGHTS_STATES_COUNT = 16;

/**
 * Number of possible encodings for en passant.
 * Used in Zobrist hashing.
 * En passant can only be described by the file (A to H aka. 8 files).
 * Only the file of the en-passant square matters because the rank
 * is implied by the side that performed the double pawn push.
 */
CX_INLINE int EN_PASSANT_STATES_COUNT = 8;

/**
 * Number of allowed half-moves before claiming a draw.
 * 50-move rule: draw can be claimed after 100 half-moves without a pawn move or capture.
 */
CX_INLINE int MAX_HALF_MOVES_BEFORE_DRAW = 100;

/** https://en.wikipedia.org/wiki/Threefold_repetition */
CX_INLINE int THREEFOLD_REPETITION_COUNT = 3;

/** https://en.wikipedia.org/wiki/Threefold_repetition#Fivefold_repetition */
CX_INLINE int FIVEFOLD_REPETITION_COUNT = 5;

/** Number of components (separated with spaces) of the FEN notation.
 *
 * Fen:
 * - rbbnknrq/pppppppp/8/8/8/8/PPPPPPPP/RBBNKNRQ w - - 0 1
 * Components:
 * - 1111111111111111111111111111111111111111111 2 3 4 5 6
 *
 * https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation
 */
CX_INLINE int FEN_NOTATION_COMPONENT_COUNT = 6;

/** Maximum number of characters a FEN string can have (for pre-allocation).
 * https://en.wikipedia.org/wiki/Forsyth–Edwards_Notation
 */
CX_INLINE int FEN_NOTATION_MAX_CHAR_COUNT = BOARD_SIZE  // 64 chessboard squares
                                            + 7         // slashes (separators between ranks)
                                            + 1         // color to play
                                            + 4         // castling rights
                                            + 1         // en passant square
                                            + 1         // halfmove
                                            + 1         // fullmove
    ;

}  // namespace Const
