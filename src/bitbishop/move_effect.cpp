#include <bitbishop/move_effect.hpp>
#include <utility>

MoveEffect MoveEffect::place(Square sq, Piece p) {
  return MoveEffect{.type = Type::Place, .square = sq, .piece = p, .prev_state = {}, .next_state = {}};
}

MoveEffect MoveEffect::remove(Square sq, Piece p) {
  return MoveEffect{.type = Type::Remove, .square = sq, .piece = p, .prev_state = {}, .next_state = {}};
}

MoveEffect MoveEffect::state_change(const BoardState& prev, const BoardState& next) {
  return MoveEffect{.type = Type::BoardState,
                    .square = Squares::A1,        // dummy value cause no default init on this object
                    .piece = Pieces::WHITE_KING,  // dummy value cause no default init on this object
                    .prev_state = prev,
                    .next_state = next};
}

void MoveEffect::apply(Board& board) const {
  switch (type) {
    case Type::Place:
      board.set_piece(square, piece);
      break;
    case Type::Remove:
      board.remove_piece(square);
      break;
    case Type::BoardState:
      board.set_state(next_state);
      break;
    default:
      std::unreachable();
  }
}

void MoveEffect::revert(Board& board) const {
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
    default:
      std::unreachable();
  }
}
