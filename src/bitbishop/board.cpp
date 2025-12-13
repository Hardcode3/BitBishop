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

std::optional<Piece> Board::get_piece(Square sq) const {
  // clang-format off
  if (m_w_pawns.test(sq))   { return Pieces::WHITE_PAWN;    }
  if (m_w_knights.test(sq)) { return Pieces::WHITE_KNIGHT;  }
  if (m_w_bishops.test(sq)) { return Pieces::WHITE_BISHOP;  }
  if (m_w_rooks.test(sq))   { return Pieces::WHITE_ROOK;    }
  if (m_w_queen.test(sq))   { return Pieces::WHITE_QUEEN;   }
  if (m_w_king.test(sq))    { return Pieces::WHITE_KING;    }

  if (m_b_pawns.test(sq))   { return Pieces::BLACK_PAWN;    }
  if (m_b_knights.test(sq)) { return Pieces::BLACK_KNIGHT;  }
  if (m_b_bishops.test(sq)) { return Pieces::BLACK_BISHOP;  }
  if (m_b_rooks.test(sq))   { return Pieces::BLACK_ROOK;    }
  if (m_b_queen.test(sq))   { return Pieces::BLACK_QUEEN;   }
  if (m_b_king.test(sq))    { return Pieces::BLACK_KING;    }
  // clang-format on

  return std::nullopt;  // NO_PIECE
}

void Board::set_piece(Square sq, Piece piece) {
  // Remove any existing piece if existent
  const std::optional<Piece> existing_piece = get_piece(sq);
  if (existing_piece.has_value()) {
    remove_piece(sq);
  }

  // Use color to select white or black bitboards, then type for the specific piece
  if (piece.is_white()) {
    switch (piece.type()) {
        // clang-format off
    case Piece::PAWN:   m_w_pawns.set(sq);   return;
    case Piece::KNIGHT: m_w_knights.set(sq); return;
    case Piece::BISHOP: m_w_bishops.set(sq); return;
    case Piece::ROOK:   m_w_rooks.set(sq);   return;
    case Piece::QUEEN:  m_w_queen.set(sq);   return;
    case Piece::KING:   m_w_king.set(sq);    return;
    default: break;  // clang-format on
    }
  } else {  // is_black()
    switch (piece.type()) {
        // clang-format off
      case Piece::PAWN:   m_b_pawns.set(sq);   return;
      case Piece::KNIGHT: m_b_knights.set(sq); return;
      case Piece::BISHOP: m_b_bishops.set(sq); return;
      case Piece::ROOK:   m_b_rooks.set(sq);   return;
      case Piece::QUEEN:  m_b_queen.set(sq);   return;
      case Piece::KING:   m_b_king.set(sq);    return;
      default: break;  // clang-format on
    }
  }

  // If we get here, something went wrong
  throw std::invalid_argument(
      std::format("Invalid piece type for piece {} on square {}", piece.to_char(), sq.to_string()));
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
  using namespace Const;

  for (int rank = RANK_8_IND; rank >= RANK_1_IND; --rank) {
    std::cout << (rank + 1) << " ";  // rank numbers on the left
    for (int file = FILE_A_IND; file <= FILE_H_IND; ++file) {
      Square sq(file, rank);
      std::optional<Piece> opt_p = get_piece(sq);
      char character = (opt_p.has_value()) ? opt_p.value().to_char() : '.';
      std::cout << character << " ";
    }
    std::cout << "\n";
  }
  std::cout << "  a b c d e f g h\n";  // file labels
}
