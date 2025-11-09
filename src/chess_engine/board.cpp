#include <chess_engine/board.hpp>
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
  std::istringstream iss(fen);
  std::string token;
  iss >> token;

  int index = 64 - 8;
  for (auto c : token) {
    if (c == '/') {
      index -= 2 * 8;
      continue;
    } else if (isdigit(c)) {
      const int skip = c - '0';
      index += skip;
      continue;
    } else {
      const Square square(index);
      Piece piece(c);
      set_piece(square, piece);
      index++;
    }
  }

  // Second token: side to move
  iss >> token;
  m_is_white_turn = (token == "w") ? true : false;

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
  Bitboard bb;
  bb |= m_w_pawns;
  bb |= m_w_rooks;
  bb |= m_w_bishops;
  bb |= m_w_knights;
  bb |= m_w_king;
  bb |= m_w_queen;
  return bb;
}

Bitboard Board::black_pieces() const {
  Bitboard bb;
  bb |= m_b_pawns;
  bb |= m_b_rooks;
  bb |= m_b_bishops;
  bb |= m_b_knights;
  bb |= m_b_king;
  bb |= m_b_queen;
  return bb;
}

Bitboard Board::occupied() const {
  Bitboard bb;
  bb |= m_b_pawns;
  bb |= m_b_rooks;
  bb |= m_b_bishops;
  bb |= m_b_knights;
  bb |= m_b_king;
  bb |= m_b_queen;
  bb |= m_w_pawns;
  bb |= m_w_rooks;
  bb |= m_w_bishops;
  bb |= m_w_knights;
  bb |= m_w_king;
  bb |= m_w_queen;
  return bb;
}

Piece Board::get_piece(Square sq) const {
  if (m_w_pawns.test(sq)) return Piece('P');
  if (m_w_knights.test(sq)) return Piece('N');
  if (m_w_bishops.test(sq)) return Piece('B');
  if (m_w_rooks.test(sq)) return Piece('R');
  if (m_w_queen.test(sq)) return Piece('Q');
  if (m_w_king.test(sq)) return Piece('K');

  if (m_b_pawns.test(sq)) return Piece('p');
  if (m_b_knights.test(sq)) return Piece('n');
  if (m_b_bishops.test(sq)) return Piece('b');
  if (m_b_rooks.test(sq)) return Piece('r');
  if (m_b_queen.test(sq)) return Piece('q');
  if (m_b_king.test(sq)) return Piece('k');

  return Piece('.');  // NO_PIECE
}

void Board::set_piece(Square sq, Piece p) {
  // Remove any existing piece if existent
  const Piece existing_piece = get_piece(sq);
  if (!existing_piece.is_none()) {
    remove_piece(sq);
  }

  switch (p.type()) {
      // clang-format off
      case Piece::P:  m_w_pawns.set(sq);   return;
      case Piece::N:  m_w_knights.set(sq); return;
      case Piece::B:  m_w_bishops.set(sq); return;
      case Piece::R:  m_w_rooks.set(sq);   return;
      case Piece::Q:  m_w_queen.set(sq);   return;
      case Piece::K:  m_w_king.set(sq);    return;
      case Piece::p:  m_b_pawns.set(sq);   return;
      case Piece::n:  m_b_knights.set(sq); return;
      case Piece::b:  m_b_bishops.set(sq); return;
      case Piece::r:  m_b_rooks.set(sq);   return;
      case Piece::q:  m_b_queen.set(sq);   return;
      case Piece::k:  m_b_king.set(sq);    return;
      case Piece::NO_PIECE:                return;
    // clang-format on
    default:
      const std::string msg =
          std::format("Piece {} does not exist, cannot set a piece on square {}", p.to_char(), sq.to_string());
      throw std::invalid_argument(msg);
  }
}

void Board::remove_piece(Square sq) {
  // clear the square from ALL bitboards (only one will match)
  m_w_pawns.clear(sq);
  m_w_knights.clear(sq);
  m_w_bishops.clear(sq);
  m_w_rooks.clear(sq);
  m_w_queen.clear(sq);
  m_w_king.clear(sq);

  m_b_pawns.clear(sq);
  m_b_knights.clear(sq);
  m_b_bishops.clear(sq);
  m_b_rooks.clear(sq);
  m_b_queen.clear(sq);
  m_b_king.clear(sq);
}

void Board::print() const {
  for (int rank = 7; rank >= 0; --rank) {
    std::cout << (rank + 1) << " ";  // rank numbers on the left
    for (int file = 0; file < 8; ++file) {
      Square sq(file, rank);
      Piece p = get_piece(sq);
      char c = p.to_char();
      std::cout << c << " ";
    }
    std::cout << "\n";
  }
  std::cout << "  a b c d e f g h\n";  // file labels
}
