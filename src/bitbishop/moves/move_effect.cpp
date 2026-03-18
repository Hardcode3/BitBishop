#include <bitbishop/moves/move_effect.hpp>
#include <utility>

MoveEffect MoveEffect::place(Square sq, Piece piece) {
  MoveEffect effect{.type = Type::Place, .square = sq, .piece = piece, .prev_state = {}, .next_state = {}};
  return effect;
}

MoveEffect MoveEffect::remove(Square sq, Piece piece) {
  MoveEffect effect{.type = Type::Remove, .square = sq, .piece = piece, .prev_state = {}, .next_state = {}};
  return effect;
}

MoveEffect MoveEffect::state_change(const BoardState& prev, const BoardState& next) {
  MoveEffect effect{.type = Type::BoardState, .prev_state = prev, .next_state = next};
  return effect;
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
  }
}
