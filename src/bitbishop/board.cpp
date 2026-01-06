#include <bitbishop/board.hpp>
#include <bitbishop/constants.hpp>
#include <format>
#include <sstream>

Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

Board::Board(const std::string& fen) {
  /*
   * https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
   * https://www.chess.com/terms/fen-chess
   *
   * FEN (Forsyth-Edwards Notation) has 6 fields separated by slashes and spaces:
   *
   * 1. Piece placement (ranks 8 → 1, separated by /).
   * 2. Side to move (w or b).
   * 3. Castling availability (KQkq or -).
   * 4. En passant target square (like e3 or -).
   * 5. Halfmove clock (for 50-move rule).
   * 6. Fullmove number (starts at 1).
   *
   * Example FEN for starting position: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
   */
  using namespace Const;

  std::istringstream iss(fen);
  std::string token;
  iss >> token;

  int index = BOARD_SIZE - BOARD_WIDTH;
  for (unsigned char character : token) {
    if (character == '/') {
      index -= 2 * BOARD_WIDTH;
      continue;
    }
    if (std::isdigit(character) != 0) {
      const int skip = character - '0';
      index += skip;
      continue;
    }
    const Square square(index);
    Piece piece(character);
    set_piece(square, piece);
    index++;
  }

  // Second token: side to move
  iss >> token;
  m_is_white_turn = (token == "w");

  // Third token: Castling Rights
  iss >> token;
  m_white_castle_kingside = token.find('K') != std::string::npos;
  m_white_castle_queenside = token.find('Q') != std::string::npos;
  m_black_castle_kingside = token.find('k') != std::string::npos;
  m_black_castle_queenside = token.find('q') != std::string::npos;

  // Fourth token: en passant
  iss >> token;
  if (token == "-") {
    m_en_passant_sq = std::nullopt;
  } else {
    m_en_passant_sq = Square(token);
  }

  // Fifth token: Halfmove clock
  iss >> m_halfmove_clock;

  // Sixth token: Fullmove number
  iss >> m_fullmove_number;
}

Bitboard Board::white_pieces() const {
  Bitboard bitboard;
  bitboard |= m_w_pawns;
  bitboard |= m_w_rooks;
  bitboard |= m_w_bishops;
  bitboard |= m_w_knights;
  bitboard |= m_w_king;
  bitboard |= m_w_queens;
  return bitboard;
}

Bitboard Board::black_pieces() const {
  Bitboard bitboard;
  bitboard |= m_b_pawns;
  bitboard |= m_b_rooks;
  bitboard |= m_b_bishops;
  bitboard |= m_b_knights;
  bitboard |= m_b_king;
  bitboard |= m_b_queens;
  return bitboard;
}

Bitboard Board::occupied() const {
  Bitboard bitboard;
  bitboard |= m_b_pawns;
  bitboard |= m_b_rooks;
  bitboard |= m_b_bishops;
  bitboard |= m_b_knights;
  bitboard |= m_b_king;
  bitboard |= m_b_queens;
  bitboard |= m_w_pawns;
  bitboard |= m_w_rooks;
  bitboard |= m_w_bishops;
  bitboard |= m_w_knights;
  bitboard |= m_w_king;
  bitboard |= m_w_queens;
  return bitboard;
}

std::optional<Piece> Board::get_piece(Square square) const {
  // clang-format off
  if (m_w_pawns.test(square))   { return Pieces::WHITE_PAWN;    }
  if (m_w_knights.test(square)) { return Pieces::WHITE_KNIGHT;  }
  if (m_w_bishops.test(square)) { return Pieces::WHITE_BISHOP;  }
  if (m_w_rooks.test(square))   { return Pieces::WHITE_ROOK;    }
  if (m_w_queens.test(square))   { return Pieces::WHITE_QUEEN;   }
  if (m_w_king.test(square))    { return Pieces::WHITE_KING;    }

  if (m_b_pawns.test(square))   { return Pieces::BLACK_PAWN;    }
  if (m_b_knights.test(square)) { return Pieces::BLACK_KNIGHT;  }
  if (m_b_bishops.test(square)) { return Pieces::BLACK_BISHOP;  }
  if (m_b_rooks.test(square))   { return Pieces::BLACK_ROOK;    }
  if (m_b_queens.test(square))   { return Pieces::BLACK_QUEEN;   }
  if (m_b_king.test(square))    { return Pieces::BLACK_KING;    }
  // clang-format on

  return std::nullopt;  // NO_PIECE
}

void Board::move_piece(Square from, Square to) {
  if (from == to) {
    return;
  }

  if (auto captured = get_piece(to)) {
    remove_piece(to);
  }

  auto moving_piece = get_piece(from);
  if (!moving_piece) {
    return;
  }

  remove_piece(from);

  set_piece(to, moving_piece.value());
}

void Board::set_piece(Square square, Piece piece) {
  // Remove any existing piece if existent
  const std::optional<Piece> existing_piece = get_piece(square);
  if (existing_piece.has_value()) {
    remove_piece(square);
  }

  // Use color to select white or black bitboards, then type for the specific piece
  if (piece.is_white()) {
    switch (piece.type()) {
        // clang-format off
    case Piece::PAWN:   m_w_pawns.set(square);   return;
    case Piece::KNIGHT: m_w_knights.set(square); return;
    case Piece::BISHOP: m_w_bishops.set(square); return;
    case Piece::ROOK:   m_w_rooks.set(square);   return;
    case Piece::QUEEN:  m_w_queens.set(square);   return;
    case Piece::KING:   m_w_king.set(square);    return;
    default: break;  // clang-format on
    }
  } else {  // is_black()
    switch (piece.type()) {
        // clang-format off
      case Piece::PAWN:   m_b_pawns.set(square);   return;
      case Piece::KNIGHT: m_b_knights.set(square); return;
      case Piece::BISHOP: m_b_bishops.set(square); return;
      case Piece::ROOK:   m_b_rooks.set(square);   return;
      case Piece::QUEEN:  m_b_queens.set(square);   return;
      case Piece::KING:   m_b_king.set(square);    return;
      default: break;  // clang-format on
    }
  }

  // If we get here, something went wrong
  throw std::invalid_argument(
      std::format("Invalid piece type for piece {} on square {}", piece.to_char(), square.to_string()));
}

void Board::remove_piece(Square square) {
  // clear the square from ALL bitboards (only one will match)
  m_w_pawns.clear(square);
  m_w_knights.clear(square);
  m_w_bishops.clear(square);
  m_w_rooks.clear(square);
  m_w_queens.clear(square);
  m_w_king.clear(square);

  m_b_pawns.clear(square);
  m_b_knights.clear(square);
  m_b_bishops.clear(square);
  m_b_rooks.clear(square);
  m_b_queens.clear(square);
  m_b_king.clear(square);
}

void Board::print() const {
  using namespace Const;

  for (int rank = RANK_8_IND; rank >= RANK_1_IND; --rank) {
    std::cout << (rank + 1) << " ";  // rank numbers on the left
    for (int file = FILE_A_IND; file <= FILE_H_IND; ++file) {
      Square square(file, rank);
      std::optional<Piece> opt_p = get_piece(square);
      char character = (opt_p.has_value()) ? opt_p.value().to_char() : '.';
      std::cout << character << " ";
    }
    std::cout << "\n";
  }
  std::cout << "  a b c d e f g h\n";  // file labels
}

bool Board::can_castle_kingside(Color side) const noexcept {
  if (!this->has_kingside_castling_rights(side)) {
    return false;
  }

  Square king_sq = (side == Color::WHITE) ? Squares::E1 : Squares::E8;
  Square rook_sq = (side == Color::WHITE) ? Squares::H1 : Squares::H8;
  Square f_sq = (side == Color::WHITE) ? Squares::F1 : Squares::F8;
  Square g_sq = (side == Color::WHITE) ? Squares::G1 : Squares::G8;

  // Check if king is on the starting square
  if (!this->king(side).test(king_sq)) {
    return false;
  }

  // Check if rook is on the starting square
  if (!this->rooks(side).test(rook_sq)) {
    return false;
  }

  // Check if squares between king and rook are empty
  const Bitboard occupied = this->occupied();
  return !occupied.test(f_sq) && !occupied.test(g_sq);
}

bool Board::can_castle_queenside(Color side) const noexcept {
  if (!this->has_queenside_castling_rights(side)) {
    return false;
  }

  Square king_sq = (side == Color::WHITE) ? Squares::E1 : Squares::E8;
  Square rook_sq = (side == Color::WHITE) ? Squares::A1 : Squares::A8;
  Square b_sq = (side == Color::WHITE) ? Squares::B1 : Squares::B8;
  Square c_sq = (side == Color::WHITE) ? Squares::C1 : Squares::C8;
  Square d_sq = (side == Color::WHITE) ? Squares::D1 : Squares::D8;

  // Check that the king is on the starting square
  if (!this->king(side).test(king_sq)) {
    return false;
  }

  // Check if the rook is on the starting square
  if (!this->rooks(side).test(rook_sq)) {
    return false;
  }

  // Check if the squares between king and rook are empty
  const Bitboard occupied = this->occupied();
  return !occupied.test(b_sq) && !occupied.test(c_sq) && !occupied.test(d_sq);
}

void Board::make_move(const Move& move) {
  using namespace Squares;
  using namespace Const;

  Piece moving_piece = *get_piece(move.from);
  bool is_white = moving_piece.is_white();
  bool is_pawn_move = moving_piece.type() == Piece::Type::PAWN;

  // 1. Update halfmove clock BEFORE altering board state
  if (is_pawn_move || move.is_capture || move.is_en_passant) {
    m_halfmove_clock = 0;
  } else {
    m_halfmove_clock++;
  }

  // 2. En passant capture (remove pawn behind destination)
  if (move.is_en_passant) {
    int direction = is_white ? -BOARD_WIDTH : +BOARD_WIDTH;
    Square captured = Square(move.to.value() + direction);
    remove_piece(captured);
  }

  // 3. Move the piece
  move_piece(move.from, move.to);

  // 4. Promotion
  if (move.promotion) {
    set_piece(move.to, *move.promotion);
  }

  // 5. Castling rook movement
  if (move.is_castling) {
    // clang-format off
    if (move.to == C1) {move_piece(A1, D1);}
    if (move.to == G1) {move_piece(H1, F1);}
    if (move.to == C8) {move_piece(A8, D8);}
    if (move.to == G8) {move_piece(H8, F8);}
    // clang-format on
  }

  // 6. Reset en passant square
  m_en_passant_sq = std::nullopt;

  // 7. Double pawn push sets en passant square
  if (is_pawn_move && std::abs(move.to.rank() - move.from.rank()) == 2) {
    int ep_rank = (move.to.rank() + move.from.rank()) / 2;
    m_en_passant_sq = Square(move.from.file(), ep_rank);
  }

  // 8. Update castling rights
  // king moved
  if (moving_piece.type() == Piece::Type::KING) {
    if (is_white) {
      m_white_castle_kingside = false;
      m_white_castle_queenside = false;
    } else {
      m_black_castle_kingside = false;
      m_black_castle_queenside = false;
    }
  }

  // rook moved
  if (moving_piece.type() == Piece::Type::ROOK) {
    // clang-format off
    if (move.from == A1) {m_white_castle_queenside = false;}
    if (move.from == H1) {m_white_castle_kingside = false;}
    if (move.from == A8) {m_black_castle_queenside = false;}
    if (move.from == H8) {m_black_castle_kingside = false;}
    // clang-format on
  }

  // rook captured
  if (move.is_capture || move.is_en_passant) {
    // clang-format off
    if (move.to == A1) {m_white_castle_queenside = false;}
    if (move.to == H1) {m_white_castle_kingside = false;}
    if (move.to == A8) {m_black_castle_queenside = false;}
    if (move.to == H8) {m_black_castle_kingside = false;}
    // clang-format on
  }

  // 9. Flip side to move
  m_is_white_turn = !m_is_white_turn;

  // 10. Fullmove counter
  if (!m_is_white_turn) {
    m_fullmove_number++;
  }

  // 11. Save move information for undo
  UndoInfo undo = {
      .captured_piece = get_piece(move.to),
      .captured_square = move.to,
      .white_castle_kingside = m_white_castle_kingside,
      .white_castle_queenside = m_white_castle_queenside,
      .black_castle_kingside = m_black_castle_kingside,
      .black_castle_queenside = m_black_castle_queenside,
      .en_passant_sq = m_en_passant_sq,
      .halfmove_clock = m_halfmove_clock,
      .fullmove_number = m_fullmove_number,
      .was_pawn_promotion = move.promotion.has_value(),
  };
  m_undo_stack.push_back(undo);
}

void Board::unmake_move(const Move& move) {
  UndoInfo undo = m_undo_stack.back();
  m_undo_stack.pop_back();

  // 1. Flip side to move back
  m_is_white_turn = !m_is_white_turn;

  // 2. Restore halfmove clock and fullmove number
  m_halfmove_clock = undo.halfmove_clock;
  m_fullmove_number = undo.fullmove_number;

  // 3. Restore en passant square
  m_en_passant_sq = undo.en_passant_sq;

  // 4. Move piece back
  Piece moving_piece = *get_piece(move.to);

  // Undo promotion if any
  if (undo.was_pawn_promotion) {
    moving_piece = Piece(Piece::Type::PAWN, moving_piece.color());
  }

  move_piece(move.to, move.from);
  set_piece(move.from, moving_piece);

  // 5. Restore captured piece if any
  if (undo.captured_piece) {
    set_piece(undo.captured_square, undo.captured_piece.value());
  }

  // 6. Undo castling rook moves
  if (move.is_castling) {
    using namespace Squares;
    // clang-format off
    if (move.to == C1) {move_piece(D1, A1);}
    if (move.to == G1) {move_piece(F1, H1);}
    if (move.to == C8) {move_piece(D8, A8);}
    if (move.to == G8) {move_piece(F8, H8);}
    // clang-format on
  }

  // 7. Restore castling rights
  m_white_castle_kingside = undo.white_castle_kingside;
  m_white_castle_queenside = undo.white_castle_queenside;
  m_black_castle_kingside = undo.black_castle_kingside;
  m_black_castle_queenside = undo.black_castle_queenside;

  // 8. Undo en passant capture if applicable
  if (move.is_en_passant) {
    using namespace Const;
    int direction = m_is_white_turn ? BOARD_WIDTH : -BOARD_WIDTH;
    Square captured = Square(move.to.value() - direction);
    set_piece(captured, *undo.captured_piece);
    remove_piece(move.to);             // Remove the capturing pawn temporarily moved
    move_piece(move.from, move.from);  // fix positions if necessary
  }
}

bool Board::operator==(const Board& other) const {
  if (this == &other) {
    return true;
  }

  // Do not compare half-move clock and full-move number
  // This is not relevant for position identity and we don't care about game history equality
  return m_w_pawns == other.m_w_pawns && m_w_rooks == other.m_w_rooks && m_w_bishops == other.m_w_bishops &&
         m_w_knights == other.m_w_knights && m_w_king == other.m_w_king && m_w_queens == other.m_w_queens &&
         m_b_pawns == other.m_b_pawns && m_b_rooks == other.m_b_rooks && m_b_bishops == other.m_b_bishops &&
         m_b_knights == other.m_b_knights && m_b_king == other.m_b_king && m_b_queens == other.m_b_queens &&
         m_is_white_turn == other.m_is_white_turn && m_en_passant_sq == other.m_en_passant_sq &&
         m_white_castle_kingside == other.m_white_castle_kingside &&
         m_white_castle_queenside == other.m_white_castle_queenside &&
         m_black_castle_kingside == other.m_black_castle_kingside &&
         m_black_castle_queenside == other.m_black_castle_queenside;
}

bool Board::operator!=(const Board& other) const { return !this->operator==(other); }
