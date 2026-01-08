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
};

struct MoveExecution;

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
  std::vector<MoveExecution> m_move_history;

 public:
  /**
   * @brief Constructs an empty starting board.
   *
   * By default, initializes to the standard chess opening position.
   *
   * @see https://www.chess.com/terms/fen-chess
   */
  Board();

  Board(const Board&) = default;

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

  BoardState get_state() const { return m_state; }
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

  //   void Board::make_move(const Move& move) {
  //   MoveExecution exec = MoveExecutor::build_execution(*this, move);
  //   exec.apply(*this);
  //   m_move_history.push_back(exec);
  // }
  // not relevant in board
  void make_move(const Move& move);

  // void Board::unmake_move(const Move& move) {
  //   if (m_move_history.empty()) {
  //     throw std::runtime_error("No moves to unmake");
  //   }

  //   MoveExecution exec = m_move_history.back();
  //   m_move_history.pop_back();
  //   exec.revert(*this);
  // }
  // not relevant in board
  void unmake_move(const Move& move);

  Board& operator=(const Board& other) = default;

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

struct MoveEffect {
  enum class Type : uint8_t { Place, Remove, BoardState, Dummy };

  Type type;
  Square square;
  Piece piece;
  BoardState prev_state;
  BoardState new_state;

  MoveEffect(Type t, Square sq, Piece p, BoardState prev = BoardState{}, BoardState next = BoardState{})
      : type(t), square(sq), piece(p), prev_state(prev), new_state(next) {}

  MoveEffect() : type(Type::Dummy), square(Squares::A1), piece(Pieces::WHITE_KING), prev_state(), new_state() { ; }

  static MoveEffect place(Square sq, Piece p) { return MoveEffect(Type::Place, sq, p); }

  static MoveEffect remove(Square sq, Piece p) { return MoveEffect(Type::Remove, sq, p); }

  static MoveEffect state_change(BoardState prev, BoardState next) {
    return MoveEffect(Type::BoardState, Squares::A1, Pieces::WHITE_KING, prev, next);
  }

  inline void apply(Board& board) const {
    switch (type) {
      case Type::Place:
        board.set_piece(square, piece);
        break;
      case Type::Remove:
        board.remove_piece(square);
        break;
      case Type::BoardState:
        board.set_state(new_state);
        break;
      case Type::Dummy:
        break;
    }
  }

  inline void revert(Board& board) const {
    switch (type) {
      case Type::Place:
        board.remove_piece(square);
        break;
      case Type::Remove:
        board.set_piece(square, piece);
        break;
      case Type::BoardState:
        board.set_state(prev_state);
        break;
      case Type::Dummy:
        break;
    }
  }
};

struct MoveExecution {
  static constexpr int MAX_EFFECTS = 6;
  MoveEffect effects[MAX_EFFECTS];
  int count = 0;

  inline void add(const MoveEffect& e) { effects[count++] = e; }

  inline void apply(Board& board) const {
    for (int i = 0; i < count; ++i) {
      effects[i].apply(board);
    }
  }

  inline void revert(Board& board) const {
    for (int i = count - 1; i >= 0; --i) {
      effects[i].revert(board);
    }
  }
};

struct MoveContext {
  const Board& board;
  const Move& move;
  Piece moving_piece;
  bool is_white;
  bool is_pawn_move;
  BoardState prev_state;
};

// Rule function signature: takes context, generates effects into execution
using MoveRule = void (*)(const MoveContext& ctx, MoveExecution& exec);

namespace MoveRules {

inline void basic_movement(const MoveContext& ctx, MoveExecution& exec) {
  if (ctx.move.is_capture && !ctx.move.is_en_passant) {
    Piece captured = *ctx.board.get_piece(ctx.move.to);
    exec.add(MoveEffect::remove(ctx.move.to, captured));
  }

  exec.add(MoveEffect::remove(ctx.move.from, ctx.moving_piece));
  exec.add(MoveEffect::place(ctx.move.to, ctx.moving_piece));
}

inline void en_passant_capture(const MoveContext& ctx, MoveExecution& exec) {
  if (!ctx.move.is_en_passant) {
    return;
  }

  using namespace Const;
  int direction = ctx.is_white ? -BOARD_WIDTH : +BOARD_WIDTH;
  Square captured = Square(ctx.move.to.value() + direction);
  Piece captured_piece = *ctx.board.get_piece(captured);
  exec.add(MoveEffect::remove(captured, captured_piece));
}

inline void pawn_promotion(const MoveContext& ctx, MoveExecution& exec) {
  if (!ctx.move.promotion) {
    return;
  }

  exec.add(MoveEffect::remove(ctx.move.to, ctx.moving_piece));
  exec.add(MoveEffect::place(ctx.move.to, *ctx.move.promotion));
}

inline void castling_rook(const MoveContext& ctx, MoveExecution& exec) {
  if (!ctx.move.is_castling) {
    return;
  }

  using namespace Squares;

  // dummy inits
  Square rook_from = Squares::A1;
  Square rook_to = Squares::A1;

  if (ctx.move.to == C1) {
    rook_from = A1;
    rook_to = D1;
  } else if (ctx.move.to == G1) {
    rook_from = H1;
    rook_to = F1;
  } else if (ctx.move.to == C8) {
    rook_from = A8;
    rook_to = D8;
  } else if (ctx.move.to == G8) {
    rook_from = H8;
    rook_to = F8;
  } else {
    return;
  }

  Piece rook = *ctx.board.get_piece(rook_from);
  exec.add(MoveEffect::remove(rook_from, rook));
  exec.add(MoveEffect::place(rook_to, rook));
}

}  // namespace MoveRules

class BoardStateBuilder {
 private:
  BoardState state;

 public:
  explicit BoardStateBuilder(const BoardState& prev) : state(prev) {}

  // Halfmove clock
  void reset_halfmove_clock() { state.m_halfmove_clock = 0; }
  void increment_halfmove_clock() { state.m_halfmove_clock++; }

  // En passant
  void clear_en_passant() { state.m_en_passant_sq = std::nullopt; }
  void set_en_passant(Square sq) { state.m_en_passant_sq = sq; }

  // Castling rights
  void revoke_white_castling() {
    state.m_white_castle_kingside = false;
    state.m_white_castle_queenside = false;
  }

  void revoke_black_castling() {
    state.m_black_castle_kingside = false;
    state.m_black_castle_queenside = false;
  }

  void revoke_castling_if_rook_at(Square sq) {
    using namespace Squares;
    if (sq == A1) state.m_white_castle_queenside = false;
    if (sq == H1) state.m_white_castle_kingside = false;
    if (sq == A8) state.m_black_castle_queenside = false;
    if (sq == H8) state.m_black_castle_kingside = false;
  }

  // Turn
  void flip_turn() { state.m_is_white_turn = !state.m_is_white_turn; }

  // Fullmove
  void increment_fullmove_if_black_moved() {
    if (state.m_is_white_turn) {  // White is about to move = black just moved
      state.m_fullmove_number++;
    }
  }

  BoardState build() const { return state; }
};

namespace StateRules {

// Single function that builds the new state
inline void update_board_state(const MoveContext& ctx, MoveExecution& exec) {
  BoardStateBuilder builder(ctx.prev_state);

  // Halfmove clock
  if (ctx.is_pawn_move || ctx.move.is_capture || ctx.move.is_en_passant) {
    builder.reset_halfmove_clock();
  } else {
    builder.increment_halfmove_clock();
  }

  // En passant
  builder.clear_en_passant();
  if (ctx.is_pawn_move && std::abs(ctx.move.to.rank() - ctx.move.from.rank()) == 2) {
    int ep_rank = (ctx.move.to.rank() + ctx.move.from.rank()) / 2;
    builder.set_en_passant(Square(ctx.move.from.file(), ep_rank));
  }

  // Castling rights
  if (ctx.moving_piece.type() == Piece::Type::KING) {
    if (ctx.is_white) {
      builder.revoke_white_castling();
    } else {
      builder.revoke_black_castling();
    }
  }

  if (ctx.moving_piece.type() == Piece::Type::ROOK) {
    builder.revoke_castling_if_rook_at(ctx.move.from);
  }

  if (ctx.move.is_capture || ctx.move.is_en_passant) {
    builder.revoke_castling_if_rook_at(ctx.move.to);
  }

  // Turn and fullmove
  builder.flip_turn();
  builder.increment_fullmove_if_black_moved();

  // Add the state change effect
  exec.add(MoveEffect::state_change(ctx.prev_state, builder.build()));
}

}  // namespace StateRules

class MoveExecutor {
 public:
  static MoveExecution build_execution(const Board& board, const Move& move) {
    const MoveRule STANDARD_RULES[5] = {
        // clang-format off
      MoveRules::en_passant_capture,
      MoveRules::basic_movement,
      MoveRules::pawn_promotion,
      MoveRules::castling_rook,
      StateRules::update_board_state,  // Single state update at end
        // clang-format on
    };

    MoveContext ctx = {
        .board = board,
        .move = move,
        .moving_piece = *board.get_piece(move.from),
        .is_white = board.get_piece(move.from)->is_white(),
        .is_pawn_move = board.get_piece(move.from)->type() == Piece::Type::PAWN,
        .prev_state = board.get_state(),
    };

    MoveExecution exec{};
    for (MoveRule rule : STANDARD_RULES) {
      rule(ctx, exec);
    }

    return exec;
  }
};
