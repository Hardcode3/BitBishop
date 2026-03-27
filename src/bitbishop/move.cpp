#include <bitbishop/move.hpp>

[[nodiscard]] std::string Move::to_uci() const {
  std::string uci;
  uci.reserve(UCI_MOVE_PROMOTION_CHAR_REPR_SIZE);

  uci += from.to_string();
  uci += to.to_string();

  if (promotion && promotion->is_promotion()) {
    // Turn to lowecase by ORing the char with 0010 0000 = 0x20 = 32
    uci += (promotion->to_char() | 0x20);
  }

  return uci;
}

Move Move::from_uci(const std::string& str) {
  if (str.size() != UCI_MOVE_CHAR_REPR_SIZE && str.size() != UCI_MOVE_PROMOTION_CHAR_REPR_SIZE) {
    throw std::runtime_error(std::format("Invalid UCI move '{}', expected {} or {} characters", str,
                                         UCI_MOVE_CHAR_REPR_SIZE, UCI_MOVE_PROMOTION_CHAR_REPR_SIZE));
  }

  const Square from(str.substr(0, 2));
  const Square to(str.substr(2, 2));

  Move move = Move::make(from, to);

  using namespace Squares;
  if ((from == E1 && (to == G1 || to == C1)) || (from == E8 && (to == G8 || to == C8))) {
    move.is_castling = true;
  }

  if (str.size() == UCI_MOVE_PROMOTION_CHAR_REPR_SIZE) {
    if (move.is_castling) {
      throw std::runtime_error(std::format("Invalid UCI move '{}', castling cannot include promotion", str));
    }

    const Piece promo_piece = Piece(static_cast<char>(str.at(4)));

    if (!promo_piece.is_promotion()) {
      throw std::runtime_error(std::format("Invalid UCI move '{}', unknown promotion piece '{}'", str, str.at(4)));
    }

    if (to.rank() != Const::RANK_8_IND && to.rank() != Const::RANK_1_IND) {
      throw std::runtime_error(std::format("Invalid UCI move '{}', promotion must land on rank 1 or 8", str));
    }

    const Color color = (to.rank() == Const::RANK_8_IND) ? Color::WHITE : Color::BLACK;
    move.promotion = Piece(promo_piece.type(), color);
  }

  return move;
}

Move Move::make(Square from, Square to, bool is_capture) {
  return {.from = from,
          .to = to,
          .promotion = std::nullopt,
          .is_capture = is_capture,
          .is_en_passant = false,
          .is_castling = false};
}

Move Move::make_promotion(Square from, Square to, Piece piece, bool is_capture) {
  return {.from = from,
          .to = to,
          .promotion = piece,
          .is_capture = is_capture,
          .is_en_passant = false,
          .is_castling = false};
}

Move Move::make_en_passant(Square from, Square to) {
  return {.from = from,
          .to = to,
          .promotion = std::nullopt,
          .is_capture = true,
          .is_en_passant = true,
          .is_castling = false};
}

Move Move::make_castling(Square from, Square to) {
  return {.from = from,
          .to = to,
          .promotion = std::nullopt,
          .is_capture = false,
          .is_en_passant = false,
          .is_castling = true};
}