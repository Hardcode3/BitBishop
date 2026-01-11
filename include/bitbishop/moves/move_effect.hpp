#pragma once

#include <bitbishop/board.hpp>
#include <bitbishop/piece.hpp>
#include <bitbishop/square.hpp>

struct MoveEffect {
  enum class Type : uint8_t { Place, Remove, BoardState };

  Type type;
  Square square = Squares::A1;       // place holder
  Piece piece = Pieces::WHITE_KING;  // place holder
  BoardState prev_state;
  BoardState next_state;

  static MoveEffect place(Square sq, Piece piece);
  static MoveEffect remove(Square sq, Piece piece);
  static MoveEffect state_change(const BoardState& prev, const BoardState& next);

  void apply(Board& board) const;
  void revert(Board& board) const;
};
