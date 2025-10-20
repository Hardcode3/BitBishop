#pragma once
#include <chess_engine/bitboard.hpp>
#include <chess_engine/color.hpp>
#include <chess_engine/piece.hpp>
#include <chess_engine/square.hpp>
#include <optional>

/**
 * @class Board
 * @brief Represents a complete chess position.
 *
 * Internally, each piece type for each color is stored in its own Bitboard:
 * - White: pawns, rooks, bishops, knights, king, queen
 * - Black: pawns, rooks, bishops, knights, king, queen
 *
 * Additional game state is tracked:
 * - Active color (white to move or black to move)
 * - En passant target square (if available)
 * - Castling rights (white/black kingside/queenside)
 * - Halfmove clock (for 50-move rule)
 * - Fullmove number (increments after Black’s move)
 *
 * The class provides utilities to query occupied squares,
 * retrieve/set/remove individual pieces, and print the board.
 */
class Board {
 private:
  // Piece bitboards
  Bitboard m_w_pawns, m_w_rooks, m_w_bishops, m_w_knights, m_w_king, m_w_queen;
  Bitboard m_b_pawns, m_b_rooks, m_b_bishops, m_b_knights, m_b_king, m_b_queen;

  // Game state
  bool m_is_white_turn;                   ///< True if it is White's turn
  std::optional<Square> m_en_passant_sq;  ///< En passant target square, or nullopt if none

  // Castling abilities
  bool m_white_castle_kingside;   ///< White may castle kingside
  bool m_white_castle_queenside;  ///< White may castle queenside
  bool m_black_castle_kingside;   ///< Black may castle kingside
  bool m_black_castle_queenside;  ///< Black may castle queenside

  // 50-move rule state
  int m_halfmove_clock;  ///< Counts halfmoves since last pawn move or capture

  // Move number (starts at 1, incremented after Black’s move)
  int m_fullmove_number;

 public:
  /**
   * @brief Constructs an empty starting board.
   *
   * By default, initializes to the standard chess opening position.
   *
   * @see https://www.chess.com/terms/fen-chess
   */
  Board();

  /**
   * @brief Constructs a board from a FEN string.
   * @param fen Forsyth–Edwards Notation describing a chess position.
   *
   * @see https://www.chess.com/terms/fen-chess
   */
  Board(const std::string& fen);

  /**
   * @brief Returns a bitboard containing all white pieces.
   */
  Bitboard white_pieces() const;

  /**
   * @brief Returns a bitboard containing all black pieces.
   */
  Bitboard black_pieces() const;

  /**
   * @brief Returns a bitboard containing all occupied squares (both sides).
   */
  Bitboard occupied() const;

  /**
   * @brief Retrieves the piece on a given square.
   * @param sq The square to query.
   * @return Piece located on `sq' of NO_PIECE ('.').
   */
  Piece get_piece(Square sq) const;

  /**
   * @brief (Re)Places a piece on a given square.
   * @param sq The square where the piece will be placed.
   * @param p The piece type (including color).
   */
  void set_piece(Square sq, Piece p);

  /**
   * @brief Removes any piece from a given square.
   * @param sq The square to clear.
   */
  void remove_piece(Square sq);

  /**
   * @brief Prints the board to the given output stream.
   *
   * Prints an ASCII board with ranks 8 → 1 and files A → H.
   * Example:
   * @code
   * r n b q k b n r
   * p p p p p p p p
   * . . . . . . . .
   * . . . . . . . .
   * . . . . . . . .
   * . . . . . . . .
   * P P P P P P P P
   * R N B Q K B N R
   * @endcode
   */
  void print() const;

  /**
   * @brief Returns a bitboard representing all pawns belonging to the given side to move.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard containing all squares occupied by that side's pawns.
   */
  Bitboard pawns(Color side) const { return (side == Color::WHITE) ? m_w_pawns : m_b_pawns; }

  /**
   * @brief Returns a bitboard of all enemy pieces relative to the given side to move.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard of all opposing pieces.
   */
  Bitboard enemy(Color side) const { return (side == Color::WHITE) ? black_pieces() : white_pieces(); }

  /**
   * @brief Returns a bitboard of all empty squares on the board.
   *
   * @return Bitboard with bits set where no piece occupies a square.
   */
  Bitboard unoccupied() const { return ~occupied(); }

  /**
   * @brief Returns the current en passant target square, if any.
   *
   * @return Optional square indicating where an en passant capture is possible,
   *         or `std::nullopt` if no en passant square is available.
   *
   * Example:
   * @code
   * if (auto sq = board.en_passant_square()) {
   *     std::cout << "En passant possible at " << *sq << "\n";
   * }
   * @endcode
   */
  std::optional<Square> en_passant_square() const noexcept { return m_en_passant_sq; }
};