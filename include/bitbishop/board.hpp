#pragma once

#include <bitbishop/bitboard.hpp>
#include <bitbishop/color.hpp>
#include <bitbishop/move.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>
#include <optional>
#include <vector>

struct BoardState {
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

  bool operator==(const BoardState& other) const {
    if (this == &other) {
      return true;
    }
    return m_is_white_turn == other.m_is_white_turn && m_en_passant_sq == other.m_en_passant_sq &&
           m_white_castle_kingside == other.m_white_castle_kingside &&
           m_white_castle_queenside == other.m_white_castle_queenside &&
           m_black_castle_kingside == other.m_black_castle_kingside &&
           m_black_castle_queenside == other.m_black_castle_queenside && m_halfmove_clock == other.m_halfmove_clock &&
           m_fullmove_number == other.m_fullmove_number;
  }

  bool operator!=(const BoardState& other) const { return !(*this == other); }
};

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
  Bitboard m_w_pawns, m_w_rooks, m_w_bishops, m_w_knights, m_w_king, m_w_queens;
  Bitboard m_b_pawns, m_b_rooks, m_b_bishops, m_b_knights, m_b_king, m_b_queens;

  // Game state
  BoardState m_state;

 public:
  /**
   * @brief Constructs an empty starting board.
   *
   * By default, initializes to the standard chess opening position.
   *
   * @see https://www.chess.com/terms/fen-chess
   */
  Board();

  Board(const Board&) noexcept = default;
  explicit Board(Board&& other) noexcept = default;

  /**
   * @brief Constructs a board from a FEN string.
   * @param fen Forsyth–Edwards Notation describing a chess position.
   *
   * @see https://www.chess.com/terms/fen-chess
   */
  Board(const std::string& fen);

  [[nodiscard]] static Board StartingPosition() noexcept {
    return {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
  }

  [[nodiscard]] static Board Empty() noexcept { return {"8/8/8/8/8/8/8/8 b - - 0 1"}; }

  /**
   * @brief Retrieves the piece on a given square.
   * @param square The square to query.
   * @return Piece located on `sq' or std::nullopt if no piece lays on that square.
   */
  [[nodiscard]] std::optional<Piece> get_piece(Square square) const;

  /**
   * @brief Moves a piece from one square to another.
   *
   * The function will move the piece of any type/color from `from` to `to`.
   * If a piece already exists on `to`, it is removed (captured).
   * If no piece exists on `from`, sinlently returns.
   * If `from` and `to` are identical, silently returns.
   *
   * @param from Source square
   * @param to Destination square
   */
  void move_piece(Square from, Square to);

  /**
   * @brief (Re)Places a piece on a given square.
   * @param square The square where the piece will be placed.
   * @param piece The piece type (including color).
   */
  void set_piece(Square square, Piece piece);

  /**
   * @brief Removes any piece from a given square.
   * @param square The square to clear.
   */
  void remove_piece(Square square);

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
   * @brief Returns a bitboard containing all white pieces.
   */
  [[nodiscard]] Bitboard white_pieces() const;

  /**
   * @brief Returns a bitboard containing all black pieces.
   */
  [[nodiscard]] Bitboard black_pieces() const;

  /**
   * @brief Returns a bitboard containing all occupied squares (both sides).
   */
  [[nodiscard]] Bitboard occupied() const;

  /**
   * @brief Returns a bitboard of all empty squares on the board.
   *
   * @return Bitboard with bits set where no piece occupies a square.
   */
  [[nodiscard]] Bitboard unoccupied() const { return ~occupied(); }

  /**
   * @brief Returns the square of the king for the given color.
   *
   * In standard chess, a king is always present. However, for tests, variants,
   * or incomplete board setups, the king may be missing. To handle this safely,
   * the result is returned as a std::optional.
   *
   * @param us Color of the king
   * @return std::optional<Square> containing the king's square if present, std::nullopt otherwise
   */
  [[nodiscard]] std::optional<Square> king_square(Color us) const { return king(us).pop_lsb(); }

  /**
   * @brief Returns a bitboard representing all pawns belonging to the given side to move.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard containing all squares occupied by that side's pawns.
   */
  [[nodiscard]] Bitboard pawns(Color side) const { return (side == Color::WHITE) ? m_w_pawns : m_b_pawns; }

  /**
   * @brief Returns a bitboard representing the king belonging to the given side to move.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard containing all squares occupied by that side's king.
   */
  [[nodiscard]] Bitboard king(Color side) const { return (side == Color::WHITE) ? m_w_king : m_b_king; }

  /**
   * @brief Returns a bitboard representing all rooks belonging to the given side.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard containing all squares occupied by that side's rooks.
   */
  [[nodiscard]] Bitboard rooks(Color side) const { return (side == Color::WHITE) ? m_w_rooks : m_b_rooks; }

  /**
   * @brief Returns a bitboard representing all knights belonging to the given side.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard containing all squares occupied by that side's knights.
   */
  [[nodiscard]] Bitboard knights(Color side) const { return (side == Color::WHITE) ? m_w_knights : m_b_knights; }

  /**
   * @brief Returns a bitboard representing all bishops belonging to the given side.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard containing all squares occupied by that side's bishop.
   */
  [[nodiscard]] Bitboard bishops(Color side) const { return (side == Color::WHITE) ? m_w_bishops : m_b_bishops; }

  /**
   * @brief Returns a bitboard representing the queen(s) belonging to the given side.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard containing all squares occupied by that side's queen(s).
   */
  [[nodiscard]] Bitboard queens(Color side) const { return (side == Color::WHITE) ? m_w_queens : m_b_queens; }

  /**
   * @brief Returns a bitboard of all enemy pieces relative to the given side to move.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard of all opposing pieces.
   */
  [[nodiscard]] Bitboard enemy(Color side) const { return (side == Color::WHITE) ? black_pieces() : white_pieces(); }

  /**
   * @brief Returns a bitboard of all friendly pieces relative to the given side to move.
   *
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return Bitboard of all friendly pieces.
   */
  [[nodiscard]] Bitboard friendly(Color side) const { return (side == Color::WHITE) ? white_pieces() : black_pieces(); }

  [[nodiscard]] BoardState get_state() const { return m_state; }
  void set_state(BoardState state) { m_state = state; }

  /**
   * @brief Returns the current en passant target square, if any.
   *
   * @return Optional square indicating where an en passant capture is possible,
   * or `std::nullopt` if no en passant square is available.
   *
   * Example:
   * @code
   * if (auto sq = board.en_passant_square()) {
   *     std::cout << "En passant possible at " << *sq << "\n";
   * }
   * @endcode
   */
  [[nodiscard]] std::optional<Square> en_passant_square() const noexcept { return m_state.m_en_passant_sq; }

  /**
   * @brief Checks if the given side has kingside castling rights.
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return true if kingside castling rights is available, false otherwise
   */
  [[nodiscard]] bool has_kingside_castling_rights(Color side) const {
    return (side == Color::WHITE) ? m_state.m_white_castle_kingside : m_state.m_black_castle_kingside;
  }

  /**
   * @brief Checks if the given side has queenside castling rights.
   * @param side The color corresponding to the side to move (Color::WHITE or Color::BLACK).
   * @return true if queenside castling rights is available, false otherwise
   */
  [[nodiscard]] bool has_queenside_castling_rights(Color side) const {
    return (side == Color::WHITE) ? m_state.m_white_castle_queenside : m_state.m_black_castle_queenside;
  }

  /**
   * @brief Checks if kingside castling is legal.
   *
   * Kingside castling (also called short castling or O-O) is legal when:
   * - Neither the king nor the kingside rook have previously moved
   * - All squares between the king and the kingside rook are empty
   * - The king is not currently in check
   * - The king does not pass through or land on a square that is under attack
   *
   * @param side Color of the side to check
   * @return true if kingside castling is legal, false otherwise
   */
  [[nodiscard]] bool can_castle_kingside(Color side) const noexcept;

  /**
   * @brief Checks if queenside castling is legal.
   *
   * Queenside castling (also called long castling or O-O-O) is legal when:
   * - Neither the king nor the queenside rook have previously moved
   * - All squares between the king and the queenside rook are empty
   * - The king is not currently in check
   * - The king does not pass through or land on a square that is under attack
   *
   * @param side Color of the side to check
   * @return true if queenside castling is legal, false otherwise
   */
  [[nodiscard]] bool can_castle_queenside(Color side) const noexcept;

  Board& operator=(const Board& other) noexcept = default;
  Board& operator=(Board&& other) noexcept = default;

  /**
   * @brief Checks if two boards represent the same chess position.
   *
   * Compares piece placement, side to move, en passant square, and castling rights.
   * Ignores half-move clock and full-move number.
   *
   * @param other The board to compare against
   * @return true if positions are identical, false otherwise
   */
  [[nodiscard]] bool operator==(const Board& other) const;

  /**
   * @brief Checks if two boards represent different positions.
   *
   * Logical negation of operator==().
   *
   * @param other The board to compare against
   * @return true if positions differ, false otherwise
   */
  [[nodiscard]] bool operator!=(const Board& other) const;
};
